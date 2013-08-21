/*
 * datastr.h
 *
 *  Created on: Mar 26, 2013
 *      Author: kwang
 */

#ifndef DATASTR_H_
#define DATASTR_H_

#include <stdlib.h>
#include <string.h>
//#include "kvstr.pb-c.h"
#include "src/ZHT/src/c_zhtclient.h"

extern int partition_size;   // number of compute nodes
extern int num_controller;   // number of controllers
extern char** mem_list_file;    // controller membership list
extern char* controller_id;
//extern KVStr* kv_str_key;    // the controller itself key
extern int num_job;
extern int num_job_fin;
extern int num_job_fail;
extern pthread_mutex_t num_job_fin_mutex;
extern pthread_mutex_t num_job_fail_mutex;
extern pthread_mutex_t opt_mutex;
//extern pthread_mutex_t compare_and_swap_mutex;

extern int _LOOKUP_SIZE;

typedef enum
{
    PENDING,
    RUNNING,
    SUCCESS,
    FAILED,
    CANCELED
} status_t;

typedef struct _queue_item
{
    char *job_description;
    struct _queue_item *next;
} queue_item;

typedef struct _queue
{
    queue_item* head;
    queue_item* tail;
    int queue_length;
} queue;

extern queue* init_queue();
extern void append_queue(queue*, char* job_description);
extern queue_item* del_first_queue(queue*);

//extern KVStr* _init_kv_str(int, uint32_t, int, int, int);

//extern char* _packing(KVStr*);
//extern KVStr* _unpacking(char*, int);
//extern void _packing_insert_zht(KVStr*, KVStr*);
//extern KVStr* _lookup_zht_unpacking(KVStr*);
extern int _zht_client_init(char*, char*, char*);
//extern char* _allocate_node(KVStr*, KVStr*, int, int);
extern char* _allocate_node(char* key, char* seen_value, char** seen_value_array,
		int num_node_allocate, int num_node_before);
//extern int _compare_and_swap(KVStr*, KVStr*, KVStr*);

extern char* c_calloc(int);
extern char* intstr(int);
extern int strtin(char*);
extern char** split_str(char*, char*);
extern int get_size(char**);
extern long long timeval_diff(struct timeval*, struct timeval*, struct timeval*);
#endif /* DATASTR_H_ */
