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
char mem_list_file[20][100] = {'\0'};
char* controller_id = NULL;

int num_job = 0;
int num_job_fin = 0;
int num_job_fail = 0;
pthread_mutex_t num_job_fin_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t num_job_fail_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t opt_mutex = PTHREAD_MUTEX_INITIALIZER;

long long num_insert_msg = 0LL;
long long num_lookup_msg = 0LL;
long long num_comswap_msg = 0LL;

pthread_mutex_t insert_msg_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lookup_msg_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t comswap_msg_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;

int ntime = 0;
pthread_mutex_t time_mutex = PTHREAD_MUTEX_INITIALIZER;

char* node_count_list = NULL;
queue* job_queue = NULL;

int num_regist_recv = 0;
int ready = 0;

char *slurm_conf_filename; 

pthread_mutex_t regist_mutex = PTHREAD_MUTEX_INITIALIZER;

struct timeval start;
struct timeval end;

void		_read_memlist(char*);
void		_read_workload(char*);
void*		_recv_msg_proc(void *no_data);
void*		_service_connection(void *arg);
void		_controller_req(slurm_msg_t*);
void*		_job_proc(void*);
void		_regist_msg_proc(slurm_msg_t*);
void		_step_complete_msg_proc(slurm_msg_t*);

typedef struct connection_arg
{
    int newsockfd;
} connection_arg_t;

typedef struct fin_check_arg
{
	char* key;
	int numCtl;
} fin_check_arg_t;

int main(int argc, char *argv[])
{
    slurm_conf_reinit(slurm_conf_filename);
    controller_id = c_calloc(100);
    node_count_list = c_calloc(100 * 100);
    strcat(controller_id, slurmctld_conf.control_machine);
    partition_size = str_to_int(argv[3]);
    strcat(node_count_list, argv[3]); strcat(node_count_list, ",");
    if (argc < 8)
    {
		fprintf(stderr, "usage:./controller numController memList partitionSize "
				"workload zht_mem_list zht_config_file TCP\n");
		exit(-1);
    }
    _zht_client_init(argv[5], argv[6], argv[7]);
    num_controller = str_to_int(argv[1]);
    _read_memlist(argv[2]);
    job_queue = init_queue();
    _read_workload(argv[4]);
    num_job = job_queue->queue_length;
    pthread_t *recv_msg_thread = (pthread_t*)malloc(sizeof(pthread_t));
    while (pthread_create(recv_msg_thread, NULL, _recv_msg_proc, NULL))
    {
		fprintf(stderr, "pthread_create error!");
		sleep(1);
    }
    while (!ready)
    {
    	usleep(1);
    }
    sleep(30);
    gettimeofday(&start, 0x0);
    while (job_queue->queue_length > 0)
    {
		queue_item *job = del_first_queue(job_queue);
		pthread_t *job_proc_thread = (pthread_t*)malloc(sizeof(pthread_t));
		while (pthread_create(job_proc_thread, NULL, _job_proc, (void*)job))
		{
			fprintf(stderr, "pthread_create error!");
			sleep(1);
		}
    }
    while (num_job_fin + num_job_fail < num_job)
    {
    	usleep(1);
    }
    gettimeofday(&end, 0x0);
    long long time_diff = timeval_diff(NULL, &end, &start);
    double time_s = time_diff / 1000000.0;
    double throughput = num_job_fin / time_s;
    printf("I finished all the jobs in %.16f sec, and "
    		"the throughput is:%.16f jobs/sec\n", time_s, throughput);
    printf("The number of insert message is:%lld\n", num_insert_msg);
    printf("The number of lookup message is:%lld\n", num_lookup_msg);
    printf("The number of compare_and_swap message is:%lld\n", num_comswap_msg);
    printf("The number of all message to ZHT is:%lld\n",
    		num_insert_msg + num_lookup_msg + num_comswap_msg);
    sleep(10000);
}

void _read_memlist(char *mem_file_path)
{
    int i = 0;
    size_t ln = 0;
    FILE* file = fopen(mem_file_path, "r");
    if (file != NULL)
    {
		char line[100];
		while (fgets(line, sizeof(line), file) != NULL)
		{
			ln = strlen(line);
			if (line[ln - 1] == '\n')
			{
				line[ln - 1] = '\0';
			}
			strcpy(mem_list_file[i++], line);
		}
    }
}

void _read_workload(char *file_path)
{
    FILE *file = fopen(file_path, "r");
    size_t ln = 0;
    if (file != NULL)
    {
		char line[100];
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

void *_service_connection(void* arg)
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
    if ((conn->newsockfd >= 0) && slurm_close_accepted_conn(conn->newsockfd)< 0)
    {
    	error("close(%d): %m", conn->newsockfd);
    }
    
cleanup:
    free(msg);
	pthread_exit(NULL);
    return NULL;
}

void _controller_req(slurm_msg_t* msg)
{
    if (msg == NULL)
    {
    	return;
    }
    if (msg->msg_type == MESSAGE_NODE_REGISTRATION_STATUS)
    {
    	_regist_msg_proc(msg);
    }
    else if (msg->msg_type == REQUEST_STEP_COMPLETE)
    {
    	_step_complete_msg_proc(msg);
    }

}

void *_job_proc(void* data)
{
    queue_item* job = (queue_item*)data;
    char** job_desc = split_str(job->job_description, " ", 10, 100);
    int argc = get_size(job_desc);
    if (argc == 0)
    {
    	fprintf(stderr, "This job has some problem!\n");
    }
    else if (!strcmp(job_desc[0], "srun"))
    {
    	drun_proc(argc, job_desc);
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

void _regist_msg_proc(slurm_msg_t* msg)
{
	slurm_send_rc_msg(msg, SLURM_SUCCESS);
	pthread_mutex_lock(&regist_mutex);
	num_regist_recv++;
	int num_insert_msg_local = 0;
	strcat(node_count_list, ((slurm_node_registration_status_msg_t *)
											msg->data)->node_name);
	if (num_regist_recv == partition_size)
	{
		_c_zht_insert2(controller_id, node_count_list);
		ready = 1;
		num_insert_msg_local++;
	}
	else
	{
		strcat(node_count_list, ",");
	}
	pthread_mutex_unlock(&regist_mutex);
	if (num_insert_msg_local > 0)
	{
		pthread_mutex_lock(&insert_msg_mutex);
		num_insert_msg++;
		pthread_mutex_unlock(&insert_msg_mutex);
	}
}

void _step_complete_msg_proc(slurm_msg_t* msg)
{
	slurm_send_rc_msg(msg, SLURM_SUCCESS);
	step_complete_msg_t *complete = (step_complete_msg_t*)msg->data;
	size_t ln; char str[20] = {0};
	sprintf(str, "%u", complete->job_id);
	char *origin_ctlid = c_calloc(100);
	char *jobid_origin_ctlid = c_calloc(100);
	char *jobid_origin_ctlid_ctls = c_calloc(100);
	int num_lookup_msg_local = 0, num_insert_msg_local = 0, num_comswap_msg_local = 0;
	_c_zht_lookup2(str, origin_ctlid);
	strcat(jobid_origin_ctlid, str); strcat(jobid_origin_ctlid, origin_ctlid);
	strcat(jobid_origin_ctlid_ctls, jobid_origin_ctlid);
	strcat(jobid_origin_ctlid_ctls, "ctls");
	char *ctl_ids_one = c_calloc(20 * 100);
	_c_zht_lookup2(jobid_origin_ctlid_ctls, ctl_ids_one);
	char **ctl_ids = split_str(ctl_ids_one, ",", 20, 100);
	int num_ctl = get_size(ctl_ids), i;
	char *pre_node_ass = c_calloc(100 * 100);
	char *pre_node_ass_copy = c_calloc(100 * 100);
	char *node_ass = c_calloc(100 * 100);
	char *node_ass_copy = c_calloc(100 * 100);
	char *node_ass_new = c_calloc(100 * 100);
	char *jobid_origin_ctlid_selfid = c_calloc(100);
	num_lookup_msg_local += 2;
	int flag = 0;
	char* query_value = c_calloc(100 * 100);

	for (i = 0; i < num_ctl; i++)
	{
		flag = 0;
		c_memset(query_value, 100 * 100);
		c_memset(jobid_origin_ctlid_selfid, 100);
		c_memset(node_ass_copy, 100 * 100);
		c_memset(node_ass, 100 * 100);
		strcat(jobid_origin_ctlid_selfid, jobid_origin_ctlid);
		strcat(jobid_origin_ctlid_selfid, ctl_ids[i]);
		_c_zht_lookup2(jobid_origin_ctlid_selfid, node_ass);
		num_lookup_msg_local++;
		strcpy(node_ass_copy, node_ass);
		int j, k;
	again:
		j = k = 0;
		c_memset(pre_node_ass, 100 * 100); c_memset(node_ass, 100 * 100);
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
		strcpy(node_ass, node_ass_copy);
		char **pre = split_str(pre_node_ass, ",", 100, 100);
		char **add = split_str(node_ass, ",", 100, 100);
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
		if (!c_zht_compare_and_swap(ctl_ids[i], pre_node_ass_copy, node_ass_new, &query_value))
		{
			flag = 1;
			goto again;
		}
	}
	free(pre_node_ass); free(pre_node_ass_copy); free(node_ass); free(node_ass_copy);
	free(node_ass_new); free(jobid_origin_ctlid_selfid); free(query_value);

	if (!strcmp(controller_id, origin_ctlid))
	{
		pthread_mutex_lock(&num_job_fin_mutex);
		num_job_fin++;
		pthread_mutex_unlock(&num_job_fin_mutex);
	}
	else
	{
		char *key = c_calloc(100);
		strcat(key, jobid_origin_ctlid); strcat(key, "Fin");
		_c_zht_insert2(key, "Finished");
		num_insert_msg_local++;
		free(jobid_origin_ctlid); free(key);
	}
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
