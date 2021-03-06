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
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <grp.h>
#include "slurmuse.h";

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

	if (pipe(shepard_pipe))
	{
		error("pipe: %m");
		return -1;
	}

	shepard_pid = fork();
	if (shepard_pid == -1)
	{
		error("fork: %m");
		return -1;
	}
	if (shepard_pid != 0)
	{
		close(shepard_pipe[0]);
		return shepard_pipe[1];
	}

	/* Wait for parent to notify of completion or I/O error on abort */
	close(shepard_pipe[1]);
	while (1)
	{
		rc = read(shepard_pipe[0], buf, 1);
		if (rc == 1)
		{
			exit(0);
		}
		else if (rc == 0)
		{
			break;	/* EOF */
		}
		else if (rc == -1)
		{
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

int _get_index(char *ctl_id)
{
    int i = 0;
    for (i = 0; i < num_controller; i++)
    {
    	if (!strcmp(ctl_id, mem_list_file[i]))
    	{
    		break;
    	}
    }
    return i;
}

void _create_srun_job(srun_job_t **p_job, env_t *env,
		slurm_step_launch_callbacks_t *step_callbacks, struct srun_options *opt_1)
{
    unsigned int iseed = (unsigned int)time(NULL);
    srand(iseed);
    srun_job_t *job = NULL;
    uint32_t job_id = 0;

    char *node_list_count = c_calloc(100 * 100);
    char *node_list_count_copy = c_calloc(100 * 100);
    char *job_node_list = c_calloc(100 * 100);

    char *jobid_origin_ctlid = c_calloc(100);
    char *jobid_origin_ctlid_ctls = c_calloc(100);

    char *jobid_origin_ctlid_selfid = c_calloc(100);

    char *ctl_ids_one = c_calloc(20 * 100);
    char **ctl_ids = c_malloc_2(20, 100);
    char **node_ass = c_malloc_2(20, 100 * 100);

    int *ctl_sel = (int*)malloc(sizeof(int) * num_controller);

    size_t ln;
    int i = 0, num_ctl = 0, num_node_allocate = 0;
    int self = 0, poll_interval = 1;

    int num_insert_msg_local = 0, num_lookup_msg_local = 0, num_comswap_msg_local = 0;

    int flag = 0;
    char* query_value = c_calloc(100 * 100);

again:
	for (i = 0; i < num_controller; i++)
	{
	    ctl_sel[i] = -1;
	}
	ln = 0; num_ctl = 0; num_node_allocate = 0; self = 0; poll_interval = 0;
	c_memset(node_list_count, 100 * 100);
	c_memset(node_list_count_copy, 100 * 100);
	c_memset(job_node_list, 100 * 100);
	c_memset(ctl_ids_one, 20 * 100);
	for (i = 0; i < 20; i++)
	{
		c_memset(ctl_ids[i], 100);
		c_memset(node_ass[i], 100 * 100);
	}
	if (!flag)
	{
		_c_zht_lookup2(controller_id, node_list_count);
		num_lookup_msg_local++;
	}
	else
	{
		strcpy(node_list_count, query_value);
	}
	strcpy(node_list_count_copy, node_list_count);
    char *tmp_ptr = NULL;
    if (node_list_count != NULL)
    {
    	char *num_node = strtok(node_list_count, ",");
    	int _num_node = str_to_int(num_node);
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
    				free(node_list_count); free(node_list_count_copy); free(job_node_list);
    				free(jobid_origin_ctlid); free(jobid_origin_ctlid_selfid);
    				free(node_ass); free(ctl_ids);
    				pthread_exit(NULL);
    			}
    			else
    			{
    				flag = 0;
			    	usleep(100000);
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
    				node_list_count_copy, p, num_node_allocate, _num_node, &query_value);
    		num_comswap_msg_local++;
    		if (tmp_char)
    		{
    			strcat(job_node_list, tmp_char);
    			strcat(ctl_ids_one, controller_id); strcat(ctl_ids_one, ",");
    			strcat(ctl_ids[num_ctl], controller_id);
    			strcat(node_ass[num_ctl], tmp_char); strcat(node_ass[num_ctl], ",");
    			int idx = _get_index(controller_id);
    			ctl_sel[idx] = num_ctl;
    			num_ctl++;
    			self = 1;
    		}
    		else
    		{
    			flag = 1;
    			goto again;
    		}
    	}
    	else if (num_controller == 1)
    	{
    		usleep(100000);
    		flag = 0;
    	    goto again;
    	}
    }
    else
    {
    	usleep(100000);
    	goto again;
    }
    c_memset(query_value, 100 * 100);
    int controller_id_selected = -1;
    char* controller_id_remote = c_calloc(100);
    char* node_list_count_remote = c_calloc(100 * 100);
    char* node_list_count_remote_copy = c_calloc(100 * 100);
    int num_node_allocate_remote = 0;
    //char *remote_nodelist = c_calloc(100 * 100);
    while (num_node_allocate < opt_1->min_nodes)
    {
    	//memset(remote_nodelist, '\0', 100 * 100);
    	c_memset(controller_id_remote, 100);
    	c_memset(node_list_count_remote, 100 * 100);
    	c_memset(node_list_count_remote_copy, 100 * 100);
    	//memset(remote_nodelist, '\0', 100 * 100);
    	controller_id_selected = _gen_random_value(num_controller);
    	strcpy(controller_id_remote, mem_list_file[controller_id_selected]);
	again_2:
		c_memset(node_list_count_remote, 100 * 100);
		c_memset(node_list_count_remote_copy, 100 * 100);
		_c_zht_lookup2(controller_id_remote, node_list_count_remote);
    	num_lookup_msg_local++;
    	if (node_list_count_remote == NULL)
    	{
    		usleep(100000);
    		continue;
    	}
    	strcpy(node_list_count_remote_copy, node_list_count_remote);
    	char* num_node = strtok(node_list_count_remote, ",");
    	int _num_node = strtol(num_node, &tmp_ptr, 10);
    	if (_num_node > 0)
    	{
    		poll_interval = 0;
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
    	    char *remote_nodelist = _allocate_node(controller_id_remote,
    	    		    	node_list_count_remote_copy, q, num_node_allocate_remote, _num_node, &query_value);
    	    num_comswap_msg_local++;
    	    if (remote_nodelist != NULL)
    	    {
    	    	strcat(job_node_list, ",");
    	    	strcat(job_node_list, remote_nodelist);
    	    	num_node_allocate += num_node_allocate_remote;
    	    	if (ctl_sel[controller_id_selected] == -1)
    	    	{
    	    		strcat(ctl_ids_one, controller_id_remote); strcat(ctl_ids_one, ",");
    	    		strcat(ctl_ids[num_ctl], controller_id_remote);
    	    		strcat(node_ass[num_ctl], remote_nodelist); strcat(node_ass[num_ctl], ",");
    	    		ctl_sel[controller_id_selected] = num_ctl;
    	    		num_ctl++;
    	    	}
    	    	else
    	    	{
    	    		strcat(node_ass[ctl_sel[controller_id_selected]], remote_nodelist);
    	    		strcat(node_ass[ctl_sel[controller_id_selected]], ",");
    	    	}
    	    	int idx = _get_index(controller_id);
    	    	if (controller_id_selected < idx)
    	    	{
    	    		self = 0;
    	    	}
    	    }
    	    else
    	    {
    	    	goto again_2;
    	    }
    	}
    	else
    	{
	    	usleep(100000);
    		poll_interval++;
    		if (poll_interval > 2)
    		{
    			char *node_ass_copy = c_calloc(100 * 100);
    			char *pre_node_ass = c_calloc(100 * 100);
    			char *pre_node_ass_copy = c_calloc(100 * 100);
    			char *node_ass_new = c_calloc(100 * 100);
    			for (i = 0; i < num_ctl; i++)
    			{
    				flag = 0;
    				c_memset(query_value, 100 * 100);
    				int j, k;
    			again_1:
    				j = k = 0;
    				c_memset(pre_node_ass, 100 * 100); c_memset(node_ass_copy, 100 * 100);
    				c_memset(pre_node_ass_copy, 100 * 100); c_memset(node_ass_new, 100 * 100);
    				if (!flag)
    				{
    					_c_zht_lookup2(ctl_ids[i], pre_node_ass);
    					num_lookup_msg_local++;
    				}
    				else
    				{
    					strcpy(pre_node_ass, query_value);
    				}
    				strcpy(pre_node_ass_copy, pre_node_ass);
    				strcpy(node_ass_copy, node_ass[i]);
    				char **pre = split_str(pre_node_ass, ",", 100, 100);
    				char **add = split_str(node_ass_copy, ",", 100, 100);
    				j = get_size(pre); k = get_size(add);
    				char *str_1 = int_to_str(j + k -1);
    				strcat(node_ass_new, str_1); strcat(node_ass_new, ","); free(str_1);
    				int idx = 1;
    				for (; idx < j; idx++)
    				{
    					strcat(node_ass_new, pre[idx]);
    				    strcat(node_ass_new, ",");
    				}
    				for (idx = 0; idx < k; idx++)
    				{
    				    strcat(node_ass_new, add[idx]);
    				    if (idx != k - 1)
    				    {
    				    	strcat(node_ass_new, ",");
    				    }
    				}
    				free(pre); free(add);
    				num_comswap_msg_local++;
    				if (!c_zht_compare_and_swap(ctl_ids[i], pre_node_ass_copy, node_ass_new))
    				{
    					flag = 1;
    					goto again_1;
    				}
    			}
    			free(node_ass_copy); free(pre_node_ass);
    			free(pre_node_ass_copy); free(node_ass_new);
    			//sleep(poll_interval);
    			flag = 0;
    			goto again;
    		}
    	}
    }
    opt_1->nodelist = c_calloc(100 * 100);
    strcpy(opt_1->nodelist, job_node_list);
    if (!opt_1->nodelist)
    {
    	goto again;
    }
    printf("The node list is:%s\n", opt_1->nodelist);
    job = _job_create_1(opt_1);
    create_job_step_1(job, false, opt_1);
    if (_become_user(opt_1) < 0)
    info("Warning: Unable to assume uid=%u", opt_1->uid);
    //_shepard_spawn(job, false);
    *p_job = job;
    job_id = job->jobid;
    printf("The node list is:%s, and job id is:%u\n", opt_1->nodelist, job_id);
    free(node_list_count); free(node_list_count_copy);
    free(controller_id_remote); free(node_list_count_remote);
    free(node_list_count_remote_copy); //free(remote_nodelist);

    char str[20] = {0}; sprintf(str, "%u", job_id);
    _c_zht_insert2(str, controller_id);
    num_insert_msg_local++;
    strcat(jobid_origin_ctlid, str); strcat(jobid_origin_ctlid, controller_id);
    //c_zht_insert2(jobid_origin_ctlid, "this is a job");
    strcat(jobid_origin_ctlid_ctls, jobid_origin_ctlid);
    strcat(jobid_origin_ctlid_ctls, "ctls");
    _c_zht_insert2(jobid_origin_ctlid_ctls, ctl_ids_one);
    num_insert_msg_local++;
    if (!strcmp(ctl_ids[0], controller_id))
    {
    	printf("OK, my self participate!\n");
    }
    if (self)
    {
    	_c_zht_insert2(jobid_origin_ctlid, "I am here");
    	num_insert_msg_local++;
    }

    for (i = 0; i < num_ctl; i++)
    {
    	c_memset(jobid_origin_ctlid_selfid, 100);
    	strcat(jobid_origin_ctlid_selfid, jobid_origin_ctlid);
    	strcat(jobid_origin_ctlid_selfid, ctl_ids[i]);
    	_c_zht_insert2(jobid_origin_ctlid_selfid, node_ass[i]);
    	num_insert_msg_local++;
    }
    free(job_node_list); free(jobid_origin_ctlid);
	free(jobid_origin_ctlid_selfid);
	free(jobid_origin_ctlid_ctls); free(node_ass); free(ctl_ids);
	pthread_mutex_lock(&lookup_msg_mutex);
	num_lookup_msg += num_lookup_msg_local;
	pthread_mutex_unlock(&lookup_msg_mutex);
	pthread_mutex_lock(&insert_msg_mutex);
	num_insert_msg += num_insert_msg_local;
	pthread_mutex_unlock(&insert_msg_mutex);
	pthread_mutex_lock(&comswap_msg_mutex);
	num_comswap_msg += num_comswap_msg_local;
	pthread_mutex_unlock(&comswap_msg_mutex);
}

int _slurm_debug_env_val (void)
{
	long int level = 0;
	const char *val;

	if ((val = getenv ("SLURM_DEBUG")))
	{
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

	if ((val = getenv("SLURM_EXIT_ERROR")))
	{
		i = atoi(val);
		if (i == 0)
			error("SLURM_EXIT_ERROR has zero value");
		else
			error_exit = i;
	}

	if ((val = getenv("SLURM_EXIT_IMMEDIATE")))
	{
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
	if (opt_1->propagate && parse_rlimits( opt_1->propagate, PROPAGATE_RLIMITS))
	{
		error( "--propagate=%s is not valid.", opt_1->propagate );
		exit(error_exit);
	}

	for (rli = get_slurm_rlimits_info(); rli->name != NULL; rli++ )
	{

		if (rli->propagate_flag != PROPAGATE_RLIMITS)
			continue;

		if (getrlimit (rli->resource, rlim) < 0)
		{
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

		if (setenvf (NULL, name, format, cur) < 0)
		{
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

	if (rlim->rlim_cur < rlim->rlim_max)
	{
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

	if ((retval = getpriority (PRIO_PROCESS, 0)) == -1)
	{
		if (errno)
		{
			error ("getpriority(PRIO_PROCESS): %m");
			return;
		}
	}

	if (setenvf (NULL, "SLURM_PRIO_PROCESS", "%d", retval) < 0)
	{
		error ("unable to set SLURM_PRIO_PROCESS in environment");
		return;
	}

	debug ("propagating SLURM_PRIO_PROCESS=%d", retval);
}

int _set_umask_env(struct srun_options *opt_1)
{
	if (!getenv("SRUN_DEBUG"))
	{	/* do not change current value */
		/* NOTE: Default debug level is 3 (info) */
		int log_level = LOG_LEVEL_INFO + _verbose - opt_1->quiet;

		if (setenvf(NULL, "SRUN_DEBUG", "%d", log_level) < 0)
			error ("unable to set SRUN_DEBUG in environment");
	}

	if (!getenv("SLURM_UMASK"))
	{	/* do not change current value */
		char mask_char[5];
		mode_t mask;

		mask = (int)umask(0);
		umask(mask);

		sprintf(mask_char, "0%d%d%d",
			((mask>>6)&07), ((mask>>3)&07), mask&07);
		if (setenvf(NULL, "SLURM_UMASK", "%s", mask_char) < 0)
		{
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

	if ((getcwd(buf, MAXPATHLEN)) == NULL)
	{
		error("getcwd failed: %m");
		exit(error_exit);
	}

	if (setenvf(NULL, "SLURM_SUBMIT_DIR", "%s", buf) < 0)
	{
		error("unable to set SLURM_SUBMIT_DIR in environment");
		return;
	}
}

extern void drun_proc(int argc, char **job_char_desc)
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

	pthread_mutex_lock(&global_mutex);
	debug_level = _slurm_debug_env_val();
	logopt.stderr_level += debug_level;
	log_init(xbasename(job_char_desc[0]), logopt, 0, NULL);

	_set_exit_code();

	if (slurm_select_init(1) != SLURM_SUCCESS )
		fatal( "failed to initialize node selection plugin" );

	if (switch_init() != SLURM_SUCCESS )
		fatal("failed to initialize switch plugin");

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

    pthread_mutex_unlock(&global_mutex);
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
    opt_1->spank_job_env = NULL;
    opt_1->spank_job_env_size = 0;
    if (job)
    {
    	_enhance_env(env, job, &step_callbacks, opt_1);
    	//pre_launch_srun_job(job, 0, 1);
    	//launch_common_set_stdio_fds_1(job, &cio_fds, opt_1);
    	launch_g_step_launch_1(job, &cio_fds, &global_rc, &step_callbacks, opt_1);
    	char *exist = c_calloc(100);
    	char str[20] = {0};
    	sprintf(str, "%u", job->jobid);
    	char *key = c_calloc(100);
    	strcat(key, str); strcat(key, controller_id);
    	size_t ln; int num_lookup_msg_local = 0;
    	_c_zht_lookup2(key, exist);
    	num_lookup_msg_local++;
    	if (strcmp(exist, "I am here"))
    	{
    		free(exist);
    		strcat(key, "Fin");
    		char *fin = c_calloc(100);
    		printf("The key is of the other side:%s\n", key);
    		_c_zht_lookup2(key, fin);
    		num_lookup_msg_local++;
    		while (1)
    		{
    			if (!strcmp(fin, "Finished"))
    			{
    				free(fin);
    				free(key);
    				break;
    			}
    			else
    			{
			    	printf("Failed again!, the key is:%s\n", key);
			    	usleep(100000);
    				c_memset(fin, 100);
    				_c_zht_lookup2(key, fin);
    				num_lookup_msg_local++;
    			}
    		}
    		pthread_mutex_lock(&num_job_fin_mutex);
    		num_job_fin++;
    		pthread_mutex_unlock(&num_job_fin_mutex);
    	}
    	pthread_mutex_lock(&lookup_msg_mutex);
    	num_lookup_msg += num_lookup_msg_local;
    	pthread_mutex_unlock(&lookup_msg_mutex);
    }
    //fini_srun(job, got_alloc, &global_rc, 0);
}

void dcancel_proc(char *job_char_desc)
{
}

void dinfo_proc(char *job_char_desc)
{
}
