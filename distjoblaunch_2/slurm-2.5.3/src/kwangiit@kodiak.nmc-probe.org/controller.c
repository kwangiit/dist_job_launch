#include "datastr.h"
#include <unistd.h>
#include <pthread.h>
#include "pro_req.h"
#include "slurm/slurm_errno.h"
#include "src/common/log.h"
#include "src/common/slurm_protocol_defs.h"
#include "src/common/timers.h"
#include "src/common/net.h"
#include "src/common/read_config.h"
#include "slurmctld.h"
#include "slurmuse.h"

int partition_size;

int num_controller;
char **mem_list_file;
int _LOOK_UP_SIZE = 1024 * 7;
char* controller_id;

int num_job;
int num_job_fin = 0;
int num_job_fail = 0;
pthread_mutex_t num_job_fin_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t num_job_fail_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t compare_and_swap_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t opt_mutex = PTHREAD_MUTEX_INITIALIZER;

char* node_count_list;
queue* job_queue = NULL;

int num_regist_recv = 0;
int ready = 0;

char *slurm_conf_filename; 

pthread_mutex_t regist_mutex = PTHREAD_MUTEX_INITIALIZER;

struct timeval start;
struct timeval end;

static void		_read_memlist(char*, char*);
static void*	_recv_msg_proc(void *no_data);
static void		_read_workload(char*);
static void*	_service_connection(void *arg);
static void		_controller_req(slurm_msg_t*);
static void*	_k_job_proc(void*);

typedef struct connection_arg
{
    int newsockfd;
} connection_arg_t;

int main(int argc, char *argv[])
{
    slurm_conf_reinit(slurm_conf_filename);
    controller_id = (char*)calloc(100, sizeof(char));
    node_count_list = (char*)calloc(100 * 100, sizeof(char));
    strcpy(controller_id, slurmctld_conf.control_machine);
    char **end = 0;
    partition_size = strtol(argv[3], end, 10);
    strcat(node_count_list, argv[3]); strcat(node_count_list, ",");
    if (argc < 8)
    {
		fprintf(stderr, "usage:./controller numController memList partitionSize "
				"workload zht_mem_list zht_config_file TCP\n");
		exit(-1);
    }
    _zht_client_init(argv[5], argv[6], argv[7]);
    _read_memlist(argv[1], argv[2]);
    job_queue = init_queue();
    _read_workload(argv[4]);
    num_job = job_queue->queue_length;
    pthread_t* recv_msg_thread = (pthread_t*)malloc(sizeof(pthread_t));
    while (pthread_create(recv_msg_thread, NULL, _recv_msg_proc, NULL))
    {
		fprintf(stderr, "pthread_create error!");
		sleep(1);
    }
    while (!ready)
    {
    	sleep(0.001);
    }
    gettimeofday(&start, 0x0);
    while (job_queue->queue_length > 0)
    {
		queue_item* job = del_first_queue(job_queue);
		pthread_t* job_proc_thread = (pthread_t*)malloc(sizeof(pthread_t));
		while (pthread_create(job_proc_thread, NULL, _k_job_proc, (void*)job))
		{
			fprintf(stderr, "pthread_create error!");
			sleep(1);
		}
    }
    while (num_job_fin + num_job_fail < num_job)
    {
    	sleep(0.001);
    }
    gettimeofday(&end, 0x0);
    long long time_diff = timeval_diff(NULL, &end, &start);
    double time_s = time_diff / 1000000.0;
    double throughput = num_job_fin / time_s;
    printf("I finished all the jobs in %.16f sec, and "
    		"the throughput is:%.9f jobs/sec\n", time_s, throughput);
    sleep(10000);
}

static void _read_memlist(char* _num_controller, char* mem_file_path)
{
	char **end = 0;
    num_controller = strtol(_num_controller, end, 10);
    mem_list_file = (char**)malloc(sizeof(char*) * num_controller);
    int i = 0;
    size_t ln = 0;
    FILE* file = fopen(mem_file_path, "r");
    if (file != NULL)
    {
		char line[128];
		while (fgets(line, sizeof(line), file) != NULL)
		{
			ln = strlen(line);
			if (line[ln - 1] == '\n')
			{
				line[ln - 1] = '\0';
			}
			mem_list_file[i++] = line;
		}
    }
}

static void _read_workload(char* file_path)
{
    FILE *file = fopen(file_path, "r");
    size_t ln = 0;
    if (file != NULL)
    {
		char line[128];
		while (fgets(line, sizeof(line), file) != NULL)
		{
			ln = strlen(line);
			if (line[ln - 1] == '\n')
			{
				line[ln - 1] = '\0';
			}
			append_queue(job_queue, line);
		}
    }
}

void *_recv_msg_proc(void *no_data)
{
    slurm_fd_t sock_fd, new_sock_fd;
    slurm_addr_t client_addr;
    connection_arg_t *conn_arg = (connection_arg_t*)malloc(sizeof(connection_arg_t));
    sock_fd = slurm_init_msg_engine_addrname_port
    					(
    						slurmctld_conf.control_addr,
    						slurmctld_conf.slurmctld_port
    					);
    if (sock_fd == SLURM_SOCKET_ERROR)
    {
    	fatal("slurm_init_msg_engine_addrname_port error %m");
    }
    while (1)
    {
		if ((new_sock_fd = slurm_accept_msg_conn(sock_fd, &client_addr)) == SLURM_SOCKET_ERROR)
		{
			error("slurm_accept_msg_conn: %m");
			continue;
		}
		conn_arg->newsockfd = new_sock_fd;
		pthread_t* serv_thread = (pthread_t*)malloc(sizeof(pthread_t));
		if (pthread_create(serv_thread, NULL, _service_connection, (void*)conn_arg))
		{
			error("pthread_create error:%m");
		}
    }
    return NULL;
}

static void *_service_connection(void* arg)
{
    connection_arg_t *conn = (connection_arg_t*)arg;
    slurm_msg_t *msg = (slurm_msg_t*)malloc(sizeof(slurm_msg_t));
    slurm_msg_t_init(msg);
    if (slurm_receive_msg(conn->newsockfd, msg, 0) != 0)
    {
		error("slurm_receive_msg: %m");
		slurm_close_accepted_conn(conn->newsockfd);
		goto cleanup;
    }
    _controller_req(msg);
    /*if ((conn->newsockfd >= 0) && slurm_close_accepted_conn(conn->newsockfd)< 0)
    {
    	error("close(%d): %m", conn->newsockfd);
    }*/
    
cleanup:
    //slurm_free_msg(msg);
    //free(arg);
    return NULL;
}

static void _controller_req(slurm_msg_t* msg)
{
    if (msg == NULL)
    {
    	return;
    }
    if (msg->msg_type == MESSAGE_NODE_REGISTRATION_STATUS)
    {
		slurm_send_rc_msg(msg, SLURM_SUCCESS);
		pthread_mutex_lock(&regist_mutex);
		num_regist_recv++;
		strcat(node_count_list, ((slurm_node_registration_status_msg_t *)
												msg->data)->node_name);
		printf("The registration host name is:%s\n", ((slurm_node_registration_status_msg_t *)
												msg->data)->node_name);
		if (num_regist_recv == partition_size)
		{
			c_zht_insert2(controller_id, node_count_list);
			ready = 1;
		}
		else
		{
			strcat(node_count_list, ",");
		}
		pthread_mutex_unlock(&regist_mutex);
    }
    else if (msg->msg_type == REQUEST_STEP_COMPLETE)
    {
    	slurm_send_rc_msg(msg, SLURM_SUCCESS);
    	step_complete_msg_t *complete = (step_complete_msg_t*)msg->data;
    	uint32_t job_id = complete->job_id;
    	size_t ln;
    	char str[20] = {0};
    	sprintf(str, "%u", job_id);
    	char* ctl_ids_one = (char*)calloc(10 * 100, sizeof(char));
    	c_zht_lookup2(str, ctl_ids_one, &ln);
    	int num_ctl = 0; int i = 0; char *ctl_ids[10];
    	ctl_ids[num_ctl] = strtok(ctl_ids_one, ",");
    	while (ctl_ids[num_ctl] != NULL)
    	{
    		num_ctl++;
    		ctl_ids[num_ctl] = strtok(NULL, ",");

    	}
    	if (strcmp(controller_id, ctl_ids[0]))
    	{
    		strcat(str, ",");
    		c_zht_insert2(str, "Finished!");
    	}

    	else
    	{
    		pthread_mutex_lock(&num_job_fin_mutex);
    		num_job_fin++;
    		pthread_mutex_unlock(&num_job_fin_mutex);
    	}
    	char *pre_node_ass = (char*)calloc(100 * 100, sizeof(char));
    	char *pre_node_ass_copy = (char*)calloc(100 * 100, sizeof(char));
    	char *node_ass = (char*)calloc(100 * 100, sizeof(char));
    	char *node_ass_new = (char*)calloc(100 * 100, sizeof(char));
    	char *job_key = (char*)calloc(100, sizeof(char));
    	int j = 0, k = 0;
    	for (i = 0; i < num_ctl; i++)
    	{
    		memset(job_key, '\0', 100);
    		strcpy(job_key, str); strcat(job_key, "+"); strcat(job_key, ctl_ids[i]);
again:
			j = 0; k = 0;
			memset(pre_node_ass, '\0', 100 * 100); memset(pre_node_ass_copy, '\0', 100 * 100);
			memset(node_ass, '\0', 100 * 100); memset(node_ass_new, '\0', 100 * 100);
    		c_zht_lookup2(ctl_ids[i], pre_node_ass, &ln);
    		strcpy(pre_node_ass_copy, pre_node_ass);
    		c_zht_lookup2(job_key, node_ass, &ln);
    		char *pre[100];
    		pre[j] = strtok(pre_node_ass, ",");
    		while (pre[j] != NULL)
    		{
    			j++;
    			pre[j] = strtok(NULL, ",");
    		}
    		char *add[100];
    		add[k] = strtok(node_ass, ",");
    		while (add[k] != NULL)
    		{
    			k++;
    			add[k] = strtok(NULL, ",");
    		}
    		int num = j + k - 1;
        	char str_1[20]; memset(str_1, '\0', 20);
        	sprintf(str_1, "%d", num);
        	strcat(node_ass_new, str_1); strcat(node_ass_new, ",");
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
    		if (!c_zht_compare_and_swap(ctl_ids[i], pre_node_ass_copy, node_ass_new))
    		{
    			sleep(1);
    			goto again;
    		}
    	}
    }
}

static void *_k_job_proc(void* data)
{
    queue_item* job = (queue_item*)data;
    char* job_origin_desc = (char*)(job->job_description);
    char** job_desc = (char**)malloc(sizeof(char*) * 10);
    int i = 0;
    job_desc[i] = strtok(job_origin_desc, " ");
    while (job_desc[i] != NULL)
    {
    	i++;
    	job_desc[i] = strtok(NULL, " ");
    }
    if (!strcmp(job_desc[0], "srun"))
    {
    	drun_proc(i, job_desc);
    	free(job_desc);
    }
    else if (!strcmp(job_desc[0], "dcancel"))
    {
    	//dcancel_proc(job_origin_desc_left);
    }
    else if (!strcmp(job_desc[0], "dinfo"))
    {
    	//dinfo_proc(job_origin_desc_left);
    }
    pthread_exit(NULL);
    return NULL;
}
