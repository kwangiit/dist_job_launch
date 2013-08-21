#include "stdbool.h"
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
	new_item->job_description = c_calloc(100);
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

	return c_zht_init(memlist, config, use_tcp);
}

extern int _c_zht_insert2(char *key, char *value)
{
    char *buf;
    unsigned len;

    Package package = PACKAGE__INIT;
    package.virtualpath = key;
    if (strcmp(value, "") != 0) //tricky: bypass protocol-buf's bug
	package.realfullpath = value;
    package.has_isdir = true;
    package.isdir = false;
    package.has_operation = true;
    package.operation = 3;

    len = package__get_packed_size(&package);
    buf = (char*) calloc(len, sizeof(char));
    package__pack(&package, buf);
    int iret = c_zht_insert(buf);
    free(buf);
    return iret;
}

extern void _c_zht_lookup2(char *key, char *ret)
{
    Package package2 = PACKAGE__INIT;
    package2.virtualpath = key;
    package2.operation = 1;
    unsigned len = package__get_packed_size(&package2);
    char *buf = (char*) calloc(len, sizeof(char));
    package__pack(&package2, buf);
    size_t ln;
    char *result = (char*)calloc(1024 * 10, sizeof(char));
    if (result != NULL)
    {
	int lret = c_zht_lookup(buf, result, &ln);
	free(buf);
	if (lret == 0 && ln > 0)
	{
	    //printf("OK, seems fine!\n");
	    Package * lPackage;
	    char *lBuf = (char*) calloc(ln, sizeof(char));
	    strncpy(lBuf, result, ln);
	    lPackage = package__unpack(NULL, ln, lBuf);
	    free(lBuf); free(result);
	    if (lPackage == NULL)
	    {
		printf("error unpacking lookup result\n");
	    }
	    else
	    {
		//printf("The value is:%s\n", lPackage->realfullpath);
		strcpy(ret, lPackage->realfullpath);
	    }
	}
	else
	{
	    free(result);
	}
    }
}

extern char* _allocate_node(char* key, char* seen_value, char** seen_value_array,
		int num_node_allocate, int num_node_before, char** query_value)
{
	int i = 0;
	char* nodelist = c_calloc(100 * 100);
	char* new_value = c_calloc(100 * 100);
	int num_node_left = num_node_before - num_node_allocate;
	char *_num_node_left = int_to_str(num_node_left);
	//usleep(1000);
	pthread_mutex_lock(&time_mutex);
	ntime++;
	pthread_mutex_unlock(&time_mutex);
	strcat(new_value, _num_node_left);
	free(_num_node_left);
	strcat(new_value, ",");
	for (i = 0; i < num_node_allocate; i++)
	{
		strcat(nodelist, seen_value_array[i]);
		if (i != num_node_allocate - 1)
		{
			strcat(nodelist, ",");
		}
	}
	for (i = 0; i < num_node_left; i++)
	{
		strcat(new_value, seen_value_array[i + num_node_allocate]);
		if (i != num_node_left - 1)
		{
			strcat(new_value, ",");
		}
	}
	int res = c_zht_compare_and_swap(key, seen_value, new_value, query_value);
	free(new_value);
	if (res)
	{
		return nodelist;
	}
	else
	{
		free(nodelist);
		printf("OK, compare and swap failed!\n");
		return NULL;
	}
}

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

extern void c_memset(char *str, int size)
{
    if (!str)
    {
	str = c_calloc(size);
    }
    else
    {
	memset(str, '\0', size);
    }
}

extern char** c_malloc_2(int first_dim, int second_dim)
{
	char** str = (char**)malloc(first_dim * sizeof(char*));
	int i = 0;
	for (; i < first_dim; i++)
	{
		str[i] = c_calloc(second_dim);
	}
	return str;
}

extern char* int_to_str(int num)
{
	char *str = c_calloc(20);
	sprintf(str, "%d", num);
	return str;
}

extern int str_to_int(char* str)
{
	char **end = NULL;
	int num = (int)(strtol(str, end, 10));
	return num;
}

extern char** split_str(char* str, char* delim, int first_dim, int second_dim)
{
	char** res = c_malloc_2(first_dim, second_dim);
	int i = 0;
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
