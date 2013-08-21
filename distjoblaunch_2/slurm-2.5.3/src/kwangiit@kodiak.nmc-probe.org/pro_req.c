#include <sys/param.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <grp.h>
#include "slurmuse.h";

const int _size = 1024;
const int _max_controller = 20;
int self = 0;
void* _send_job_thr(void*);

int _gen_random_value(int upper_bound)
{
    return rand() % upper_bound;
}

int _become_user(struct srun_options *opt_1)
{
	char *user = uid_to_string(opt_1->uid);
	gid_t gid = gid_from_uid(opt_1->uid);

	if (strcmp(user, "nobody") == 0)
	{
		xfree(user);
		return (error ("Invalid user id %u: %m", opt_1->uid));
	}

	if (opt_1->uid == getuid ())
	{
		xfree(user);
		return (0);
	}

	if ((opt_1->egid != (gid_t) -1) && (setgid (opt_1->egid) < 0))
	{
		xfree(user);
		return (error ("setgid: %m"));
	}

	initgroups (user, gid); /* Ignore errors */
	xfree(user);

	if (setuid (opt_1->uid) < 0)
		return (error ("setuid: %m"));

	return (0);
}

int _shepard_spawn(srun_job_t *job, bool got_alloc)
{
	int shepard_pipe[2], rc;
	pid_t shepard_pid;
	char buf[1];

	if (pipe(shepard_pipe)) {
		error("pipe: %m");
		return -1;
	}

	shepard_pid = fork();
	if (shepard_pid == -1) {
		error("fork: %m");
		return -1;
	}
	if (shepard_pid != 0) {
		close(shepard_pipe[0]);
		return shepard_pipe[1];
	}

	/* Wait for parent to notify of completion or I/O error on abort */
	close(shepard_pipe[1]);
	while (1) {
		rc = read(shepard_pipe[0], buf, 1);
		if (rc == 1) {
			exit(0);
		} else if (rc == 0) {
			break;	/* EOF */
		} else if (rc == -1) {
			if ((errno == EAGAIN) || (errno == EINTR))
				continue;
			break;
		}
	}

	(void) slurm_kill_job_step(job->jobid, job->stepid, SIGKILL);

	if (got_alloc)
		slurm_complete_job(job->jobid, NO_VAL);
	exit(0);
	return -1;
}

void _create_srun_job(srun_job_t **p_job, env_t *env,
		slurm_step_launch_callbacks_t *step_callbacks, struct srun_options *opt_1)
{
    unsigned int iseed = (unsigned int)time(NULL);
    srand(iseed);
    int set_free = 0;
    srun_job_t *job = NULL;
    uint32_t job_id = 0;
    char* node_list_count = (char*)calloc(100 * 100, sizeof(char));
    char* node_list_count_copy = (char*)calloc(100 * 100, sizeof(char));
    size_t ln; int i = 0; int num_ctl = 0;
    char* job_key = (char*)calloc(100, sizeof(char));
    char* job_node_list = (char*)calloc(100 * 100, sizeof(char));
    char** ctl_ids = (char**)malloc(sizeof(char*) * 10);
    for (i = 0; i < 10; i++)
    {
    	ctl_ids[i] = (char*)calloc(100, sizeof(char));
    }
    char* ctl_ids_one = (char*)calloc(10 * 100, sizeof(char));
    char** nodes_ass = (char**)malloc(sizeof(char*) * 10);
    for (i = 0; i < 10; i++)
    {
    	nodes_ass[i] = (char*)calloc(100 * 100, sizeof(char));
    }
    int num_node_allocate = 0;

again:
	ln = 0;
	memset(node_list_count, '\0', 100 * 100);
	memset(node_list_count_copy, '\0', 100 * 100);
	memset(job_node_list, '\0', 100 * 100);
	c_zht_lookup2(controller_id, node_list_count, &ln);

	strcpy(node_list_count_copy, node_list_count);
    char* tmp_ptr = NULL;
    if (node_list_count != NULL)
    {
    	char* num_node = strtok(node_list_count, ",");
    	int _num_node = strtol(num_node, &tmp_ptr, 10);
    	if (_num_node > 0)
    	{
    		num_node_allocate = _num_node > opt_1->min_nodes ? opt_1->min_nodes : _num_node;
    		if (num_node_allocate < opt_1->min_nodes && num_controller == 1)
    		{
    			if (partition_size < opt_1->min_nodes)
    			{
    				pthread_mutex_lock(&num_job_fail_mutex);
    				num_job_fail++;
    				pthread_mutex_unlock(&num_job_fail_mutex);
    				pthread_exit(NULL);
    			}
    			else
    			{
    				sleep(1);
    				goto again;
    			}
    		}
    		char *p[100];
    		i = 0;
    		p[i] = strtok(NULL, ",");
    		while (p[i] != NULL)
    		{
    			i++;
    			p[i] = strtok(NULL, ",");
    		}
    		char* tmp_char = _allocate_node(controller_id,
    				node_list_count_copy, p, num_node_allocate, _num_node);
    		if (tmp_char)
    		{
    			strcat(job_node_list, tmp_char);
    			strcat(ctl_ids_one, controller_id); strcat(ctl_ids_one, ",");
    			strcat(ctl_ids[num_ctl], controller_id);
    			strcat(nodes_ass[num_ctl], tmp_char); strcat(nodes_ass[num_ctl], ",");
    			num_ctl++;
    			self = 1;
    		}
    		else if (num_controller == 1)
    		{
    			sleep(1);
    			goto again;
    		}
    	}
    	else if (num_controller == 1)
    	{
    		sleep(1);
    		goto again;
    	}
    }
    int controller_id_selected = -1;
    char* controller_id_remote = (char*)calloc(100, sizeof(char));;
    char* node_list_count_remote = (char*)calloc(100 * 100, sizeof(char));
    char* node_list_count_remote_copy = (char*)calloc(100 * 100, sizeof(char));
    int num_node_allocate_remote = 0;
    char *remote_nodelist = (char*)calloc(100 * 100, sizeof(char));
    while (num_node_allocate < opt_1->min_nodes)
    {
    	memset(remote_nodelist, '\0', 100 * 100);
    	controller_id_selected = _gen_random_value(partition_size);
    	strcpy(controller_id_remote, mem_list_file[controller_id_selected]);
    	node_list_count_remote = c_zht_lookup2(controller_id_remote, node_list_count_remote, &ln);
    	strcpy(node_list_count_remote_copy, node_list_count_remote);
    	char* num_node = strtok(node_list_count_remote, ",");
    	int _num_node = strtol(num_node, &tmp_ptr, 10);
    	if (_num_node > 0)
    	{
    		num_node_allocate_remote = _num_node >=
    				    					(opt_1->min_nodes - num_node_allocate) ?
    				    					(opt_1->min_nodes - num_node_allocate) :
    				    					_num_node;
    	    char *q[100];
    	    i = 0;
    	    q[i] = strtok(NULL, ",");
    	    while (q[i] != NULL)
    	    {
    	    	i++;
    	    	q[i] = strtok(NULL, ",");
    	    }
    	    remote_nodelist = _allocate_node(controller_id_remote,
    	    		    	node_list_count_remote_copy, q, num_node_allocate_remote, _num_node);
    	    if (remote_nodelist)
    	    {
    	    	strcat(job_node_list, ",");
    	    	strcat(job_node_list, remote_nodelist);
    	    	num_node_allocate += num_node_allocate_remote;
    			strcat(ctl_ids_one, controller_id_remote); strcat(ctl_ids_one, ",");
    			strcat(ctl_ids[num_ctl], controller_id_remote);
    			strcat(nodes_ass[num_ctl], remote_nodelist); strcat(nodes_ass[num_ctl], ",");
    			num_ctl++;
    	    }
    	}
    }
    opt_1->nodelist = (char*)calloc(100 * 100, sizeof(char));
    strcpy(opt_1->nodelist, job_node_list);
    if (!opt_1->nodelist)
    {
    	sleep(1);
    	goto again;
    }
    job = _job_create_1(opt_1);
    create_job_step_1(job, false, opt_1);
    if (_become_user(opt_1) < 0)
    info("Warning: Unable to assume uid=%u", opt_1->uid);
    _shepard_spawn(job, false);
    *p_job = job;
    job_id = job->jobid;

    free(node_list_count); free(node_list_count_copy);
    free(controller_id_remote); free(node_list_count_remote);
    free(node_list_count_remote_copy); free(remote_nodelist);

    char str[20] = {0};
    sprintf(str, "%u", job_id); c_zht_insert2(str, ctl_ids_one);
    for (i = 0; i < num_ctl; i++)
    {
    	memset(job_key, '\0', 100);
    	strcat(job_key, str); strcat(job_key, "+"); strcat(job_key, ctl_ids[i]);
    	c_zht_insert2(job_key, nodes_ass[i]);
    }

    free(job_key); free(job_node_list);
    free(ctl_ids); free(ctl_ids_one);
    free(nodes_ass);
}

void* _send_job_thr(void* arg)
{
	slurm_msg_t *msg = (slurm_msg_t*)arg;
	int fd = slurm_open_msg_conn(&msg->address);
	if (fd >= 0)
	{
		slurm_send_node_msg(fd, msg);
	}
	return NULL;
}

int _slurm_debug_env_val (void)
{
	long int level = 0;
	const char *val;

	if ((val = getenv ("SLURM_DEBUG"))) {
		char *p;
		if ((level = strtol (val, &p, 10)) < -LOG_LEVEL_INFO)
			level = -LOG_LEVEL_INFO;
		if (p && *p != '\0')
			level = 0;
	}
	return ((int) level);
}

void _set_exit_code(void)
{
	int i;
	char *val;

	if ((val = getenv("SLURM_EXIT_ERROR"))) {
		i = atoi(val);
		if (i == 0)
			error("SLURM_EXIT_ERROR has zero value");
		else
			error_exit = i;
	}

	if ((val = getenv("SLURM_EXIT_IMMEDIATE"))) {
		i = atoi(val);
		if (i == 0)
			error("SLURM_EXIT_IMMEDIATE has zero value");
		else
			immediate_exit = i;
	}
}

int _set_rlimit_env(struct srun_options *opt_1)
{
	int                  rc = SLURM_SUCCESS;
	struct rlimit        rlim[1];
	unsigned long        cur;
	char                 name[64], *format;
	slurm_rlimits_info_t *rli;

	/* Modify limits with any command-line options */
	if (opt_1->propagate && parse_rlimits( opt_1->propagate, PROPAGATE_RLIMITS)){
		error( "--propagate=%s is not valid.", opt_1->propagate );
		exit(error_exit);
	}

	for (rli = get_slurm_rlimits_info(); rli->name != NULL; rli++ ) {

		if (rli->propagate_flag != PROPAGATE_RLIMITS)
			continue;

		if (getrlimit (rli->resource, rlim) < 0) {
			error ("getrlimit (RLIMIT_%s): %m", rli->name);
			rc = SLURM_FAILURE;
			continue;
		}

		cur = (unsigned long) rlim->rlim_cur;
		snprintf(name, sizeof(name), "SLURM_RLIMIT_%s", rli->name);
		if (opt_1->propagate && rli->propagate_flag == PROPAGATE_RLIMITS)
			/*
			 * Prepend 'U' to indicate user requested propagate
			 */
			format = "U%lu";
		else
			format = "%lu";

		if (setenvf (NULL, name, format, cur) < 0) {
			error ("unable to set %s in environment", name);
			rc = SLURM_FAILURE;
			continue;
		}

		debug ("propagating RLIMIT_%s=%lu", rli->name, cur);
	}

	/*
	 *  Now increase NOFILE to the max available for this srun
	 */
	if (getrlimit (RLIMIT_NOFILE, rlim) < 0)
		return (error ("getrlimit (RLIMIT_NOFILE): %m"));

	if (rlim->rlim_cur < rlim->rlim_max) {
		rlim->rlim_cur = rlim->rlim_max;
		if (setrlimit (RLIMIT_NOFILE, rlim) < 0)
			return (error ("Unable to increase max no. files: %m"));
	}

	return rc;
}

void  _set_prio_process_env(void)
{
	int retval;

	errno = 0; /* needed to detect a real failure since prio can be -1 */

	if ((retval = getpriority (PRIO_PROCESS, 0)) == -1)  {
		if (errno) {
			error ("getpriority(PRIO_PROCESS): %m");
			return;
		}
	}

	if (setenvf (NULL, "SLURM_PRIO_PROCESS", "%d", retval) < 0) {
		error ("unable to set SLURM_PRIO_PROCESS in environment");
		return;
	}

	debug ("propagating SLURM_PRIO_PROCESS=%d", retval);
}

int _set_umask_env(struct srun_options *opt_1)
{
	if (!getenv("SRUN_DEBUG")) {	/* do not change current value */
		/* NOTE: Default debug level is 3 (info) */
		int log_level = LOG_LEVEL_INFO + _verbose - opt_1->quiet;

		if (setenvf(NULL, "SRUN_DEBUG", "%d", log_level) < 0)
			error ("unable to set SRUN_DEBUG in environment");
	}

	if (!getenv("SLURM_UMASK")) {	/* do not change current value */
		char mask_char[5];
		mode_t mask;

		mask = (int)umask(0);
		umask(mask);

		sprintf(mask_char, "0%d%d%d",
			((mask>>6)&07), ((mask>>3)&07), mask&07);
		if (setenvf(NULL, "SLURM_UMASK", "%s", mask_char) < 0) {
			error ("unable to set SLURM_UMASK in environment");
			return SLURM_FAILURE;
		}
		debug ("propagating UMASK=%s", mask_char);
	}

	return SLURM_SUCCESS;
}

void _set_submit_dir_env(void)
{
	char buf[MAXPATHLEN + 1];

	if ((getcwd(buf, MAXPATHLEN)) == NULL) {
		error("getcwd failed: %m");
		exit(error_exit);
	}

	if (setenvf(NULL, "SLURM_SUBMIT_DIR", "%s", buf) < 0) {
		error("unable to set SLURM_SUBMIT_DIR in environment");
		return;
	}
}

void drun_proc(int argc, char **job_char_desc)
{
	int debug_level;
	env_t *env = xmalloc(sizeof(env_t));
	log_options_t logopt = LOG_OPTS_STDERR_ONLY;
	bool got_alloc = false;
	slurm_step_io_fds_t cio_fds = SLURM_STEP_IO_FDS_INITIALIZER;
	slurm_step_launch_callbacks_t step_callbacks;

	env->stepid = -1;
	env->procid = -1;
	env->localid = -1;
	env->nodeid = -1;
	env->cli = NULL;
	env->env = NULL;
	env->ckpt_dir = NULL;

	debug_level = _slurm_debug_env_val();
	logopt.stderr_level += debug_level;
	log_init(xbasename(job_char_desc[0]), logopt, 0, NULL);
	_set_exit_code();

	if (slurm_select_init(1) != SLURM_SUCCESS )
		fatal( "failed to initialize node selection plugin" );

	if (switch_init() != SLURM_SUCCESS )
		fatal("failed to initialize switch plugin");
    //env_t *env = xmalloc(sizeof(env_t));
    
    struct srun_options *opt_1 = (struct srun_options*)malloc(sizeof(struct srun_options));
    int global_rc = 0;

	if (xsignal_block(sig_array) < 0)
		error("Unable to block signals");
	init_spank_env();
    if (spank_init(NULL) < 0)
    {
    	error("Plug-in initialization failed");
    	exit(error_exit);
    }

    /* Be sure to call spank_fini when srun exits.
    	 */
    if (atexit((void (*) (void)) spank_fini) < 0)
    	error("Failed to register atexit handler for plugins: %m");

    /* set default options, process commandline arguments, and
    	 * verify some basic values
    	 */

    initialize_and_process_args_1(argc, job_char_desc, opt_1);

    record_ppid();

    if (spank_init_post_opt() < 0)
    {
    		error("Plugin stack post-option processing failed.");
    		exit(error_exit);
    }

    /* reinit log with new verbosity (if changed by command line)
    	 */
    if (&logopt && (_verbose || opt_1->quiet))
    {
    	/* If log level is already increased, only increment the
    		 *   level to the difference of _verbose an LOG_LEVEL_INFO
    		 */
    	if ((_verbose -= (logopt.stderr_level - LOG_LEVEL_INFO)) > 0)
    		logopt.stderr_level += _verbose;
    	logopt.stderr_level -= opt_1->quiet;
    	logopt.prefix_level = 1;
    	log_alter(logopt, 0, NULL);
    }
    else
    	_verbose = debug_level;
    (void) _set_rlimit_env(opt_1);
    _set_prio_process_env();
    (void) _set_umask_env(opt_1);
    _set_submit_dir_env();
    srun_job_t *job = NULL;
    _create_srun_job(&job, env, &step_callbacks, opt_1);
    if (job)
    {
    	_enhance_env(env, job, &step_callbacks, opt_1);
    	pre_launch_srun_job(job, 0, 1);
    	launch_common_set_stdio_fds_1(job, &cio_fds, opt_1);
    	launch_g_step_launch_1(job, &cio_fds, &global_rc, &step_callbacks, opt_1);
    	if (!self)
    	{
    	    char str[20] = {0};
    	    sprintf(str, "%u", job->jobid);
    	    strcat(str, ",");
    	    char* value = (char*)calloc(1024, sizeof(char));
    	    size_t ln;
    	    c_zht_lookup2(str, value, &ln);
    	    while (!value)
    	    {
    	    	memset(value, '\0', 1024);
    	    	sleep(1);
    	    	c_zht_lookup2(str, value, &ln);
    	    }
    		pthread_mutex_lock(&num_job_fin_mutex);
    		num_job_fin++;
    		pthread_mutex_unlock(&num_job_fin_mutex);
    	}
    }
    sleep(1000);
    //fini_srun(job, got_alloc, &global_rc, 0);
}

void dcancel_proc(char *job_char_desc)
{
}

void dinfo_proc(char *job_char_desc)
{
}
