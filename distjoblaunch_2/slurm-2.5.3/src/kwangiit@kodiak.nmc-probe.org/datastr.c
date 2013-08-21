#include "datastr.h"
#include "malloc.h"

extern queue* init_queue()
{
	queue* new_queue = (queue*)malloc(sizeof(queue));
	new_queue->head = NULL;
	new_queue->tail = NULL;
	new_queue->queue_length = 0;
	return new_queue;
}

extern void append_queue(queue* q, char* job_description)
{
	queue_item *new_item = (queue_item*)malloc(sizeof(queue_item));
	if(new_item == NULL)
	{
		printf("malloc() failed when adding element to the queue!\n");
		return;
	}
	new_item->job_description = (char*)malloc(100);
	memset(new_item->job_description, '\0', 100);
	strcpy(new_item->job_description, job_description);
	new_item->next = NULL;
	if(q == NULL)
	{
		printf("Queue has not yet been initialized!\n");
		return;
	}
	else if(q->head == NULL && q->tail == NULL)
	{
		q->head = new_item;
		q->tail = new_item;
		q->queue_length += 1;
		return;
	}
	else if(q->head == NULL || q->tail == NULL)
	{
		printf("The queue is not in the correct format, please check!\n");
		return;
	}
	else
	{
		q->tail->next = new_item;
		q->tail = new_item;
		q->queue_length += 1;
	}
}

extern queue_item* del_first_queue(queue* q)
{
	queue_item *h = NULL;
	queue_item *p = NULL;
	if(q == NULL)
	{
		printf("The queue does not exist!\n");
		return NULL;
	}
	else if(q->head == NULL && q->tail == NULL)
	{
		printf("The queue is empty!\n");
		return NULL;
	}
	else if(q->head == NULL || q->tail == NULL)
	{
		printf("The queue is not in the correct format, please check!\n");
		return NULL;
	}
	h = q->head;
	p = h->next;
	q->head = p;
	q->queue_length -= 1;
	if(q->head == NULL)
	{
		q->tail = q->head;
	}
	return h;
}

/*extern KVStr* _init_kv_str(int has_job_id, uint32_t job_id,
		int has_num_item, int num_item, int n_name)
{
	KVStr *kv_str = (KVStr*)malloc(sizeof(KVStr));
	kvstr__init(kv_str);

	kv_str->has_job_id = has_job_id;
	if (has_job_id)
	{
		kv_str->job_id = job_id;
	}
	kv_str->has_num_item = has_num_item;
	if (has_num_item)
	{
		kv_str->num_item = num_item;
	}
	kv_str->n_name = n_name;
	if (n_name > 0)
	{
		kv_str->name = (char**)malloc(sizeof(char*) * n_name);
		int i = 0;
		for (; i < n_name; i++)
		{
			kv_str->name[i] = (char*)calloc(1024, sizeof(char));
		}
	}
	else
	{
		kv_str->name = (char**)malloc(sizeof(char*) * 1);
		kv_str->name[0] = (char*)calloc(1024, sizeof(char));
		strcpy(kv_str->name[0], "localhost");
	}
	return kv_str;
}*/

extern int _zht_client_init(char* memlist, char* config, char* tcp_flag)
{
	int use_tcp = 0;
	if (!strcmp("TCP", tcp_flag))
	{
		use_tcp = 1;
	}
	else
	{
		use_tcp = 0;
	}

	return c_zht_init(memlist, config, use_tcp); //neighbor zht.cfg TCP
}

/*extern char* _packing(KVStr* kv_str)
{
	unsigned len = kvstr__get_packed_size(kv_str);
	char* buf = (char*)calloc(len, sizeof(char));
	kvstr__pack(kv_str, buf);
	return buf;
}

extern KVStr* _unpacking(char* value, int len)
{
	KVStr* cn_str = (KVStr*)malloc(sizeof(KVStr));
	char* data = (char*)calloc(len, sizeof(char));
	strncpy(data, value, len);
	cn_str = kvstr__unpack(NULL, len, data);
	return cn_str;
}

extern void _packing_insert_zht(KVStr* kv_str_key, KVStr* kv_str_value)
{
	char* buf_key = _packing(kv_str_key);
	char* buf_value = _packing(kv_str_value);
	int res = c_zht_insert2(buf_key, buf_value);
}

extern KVStr* _lookup_zht_unpacking(KVStr* kv_str_key)
{
	char* buf_key = _packing(kv_str_key);
	int len = -1;
	char* value = (char*)calloc(10240, sizeof(char));
	int res = c_zht_lookup2(buf_key, value, &len);
	printf("The value is:%s, and the strlen(value) is:%d\n", value, strlen(value));
	if (!res && len > 0)
	{
		printf("Beforing unpacking, the length is:%d\n", len);
		return _unpacking(value, len);
	}
	else
	{
		return NULL;
	}
}*/

extern char* _allocate_node(char* key, char* seen_value, char** seen_value_array,
		int num_node_allocate, int num_node_before)
{
	int i;
	char* nodelist = (char*)calloc(100 * 100, sizeof(char));
	char* new_value = (char*)calloc(100 * 100, sizeof(char));
	int num_node = num_node_before - num_node_allocate;
	char _num_node[10]; memset(_num_node, "\0", 10);
	sprintf(_num_node, "%d", num_node);
	strcat(new_value, _num_node);
	strcat(new_value, ",");
	for (i = 0; i < num_node_allocate; i++)
	{
		strcat(nodelist, seen_value_array[i]);
		if (i != num_node_allocate - 1)
		{
			strcat(nodelist, ",");
		}
	}
	for (i = num_node_allocate; i < num_node_before; i++)
	{
		strcat(new_value, seen_value_array[i - num_node_allocate]);
		if (i != num_node_before - 1)
		{
			strcat(new_value, ",");
		}
	}
	int res = c_zht_compare_and_swap(key, seen_value, new_value);
	free(new_value);
	if (res)
	{
		printf("OK, I got the resource!\n");
		return nodelist;
	}
	else
	{
		printf("Bump, I lose the resource!\n");
		return NULL;
	}
}

/*extern char* _allocate_node(KVStr* kv_str_key, KVStr* kv_str_value,
							int num_node_allocate, int size)
{
	int i;
	char* nodelist = (char*)calloc(size, sizeof(char));

	printf("The number of item after done:%d\n", kv_str_value->num_item - num_node_allocate);
	KVStr* kv_str_new_value = _init_kv_str(kv_str_value->has_job_id,
										   kv_str_value->job_id,
										   kv_str_value->has_num_item,
										   kv_str_value->num_item - num_node_allocate,
										   kv_str_value->num_item - num_node_allocate);
	char* tmp_packing = _packing(kv_str_new_value);
	printf("Fuck, what is the hell!, %s\t%d\n", tmp_packing, strlen(tmp_packing));
	KVStr* kv_str_tmp_value = _unpacking(tmp_packing, strlen(tmp_packing));
	for (i = 0; i < num_node_allocate; i++)
	{
		strcat(nodelist, kv_str_value->name[i]);
		if (i != num_node_allocate - 1)
		{
			strcat(nodelist, ",");
		}
	}
	for (i = num_node_allocate; i < kv_str_value->num_item; i++)
	{
		strcpy(kv_str_new_value->name[i - num_node_allocate], kv_str_value->name[i]);
	}
	printf("Beforing dong compare and swap!\n");
	int res = _compare_and_swap(kv_str_key, kv_str_value, kv_str_new_value);
	if (res)
	{
		printf("I win, I get the resource!\n");
		return nodelist;
	}
	else
	{
		printf("Bump, I lose!\n");
		return NULL;
	}
}*/

/*extern int _compare_and_swap(KVStr* kv_str_key, KVStr* kv_str_seen_value, KVStr* kv_str_new_value)
{
	char* key = _packing(kv_str_key);
	char* seen_value = _packing(kv_str_seen_value);
	char* new_value = _packing(kv_str_new_value);
	printf("the new_value is:%s, and the strlen(new_value) is:%d\n", new_value, strlen(new_value));
	return c_zht_compare_and_swap(key, seen_value, new_value);
}*/

extern char* c_calloc(int size)
{
	char* str = (char*)calloc(size, sizeof(char));
	while (!str)
	{
		sleep(1);
		str = (char*)calloc(size, sizeof(char));
	}
	return str;
}

extern char* intstr(int num)
{
	char *str = c_calloc(20);
	sprintf(str, "%d", num);
	return str;
}

extern int strtin(char* str)
{
	char **end = 0;
	int num = strtol(str, end, 20);
	free(end);
	return num;
}

extern char** split_str(char* str, char* delim)
{
	char** res = (char**)malloc(sizeof(char*) * 100);
	int i = 0;
	for (i = 0; i < 100 ;i++)
	{
		res[i] = (char*)malloc(100);
	}
	res[i] = strtok(str, delim);
	while (res[i])
	{
		i++;
		res[i] = strtok(NULL, delim);
	}
	return res;
}

extern int get_size(char** str)
{
	int size = 0;
	char* tmp = str[size];
	while (tmp)
	{
		size++;
		tmp = str[size];
	}
	return size;
}

long long
timeval_diff(struct timeval *difference,
             struct timeval *end_time,
             struct timeval *start_time
            )
{
	struct timeval temp_diff;
	if(difference == NULL)
	{
		difference = &temp_diff;
	}

	difference->tv_sec = end_time->tv_sec -start_time->tv_sec ;
	difference->tv_usec = end_time->tv_usec-start_time->tv_usec;

	while(difference->tv_usec<0)
	{
		difference->tv_usec += 1000000;
		difference->tv_sec -= 1;
	}

	return 1000000LL * difference->tv_sec + difference->tv_usec;
}
