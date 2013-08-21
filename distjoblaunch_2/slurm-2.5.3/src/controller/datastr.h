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
#include "src/ZHT/src/c_zhtclient.h"
#include "src/ZHT/src/meta.pb-c.h"

extern int partition_size;   // number of compute nodes
extern int num_controller;   // number of controllers
extern char mem_list_file[20][100];    // controller membership list
extern char* controller_id;

extern int num_job;
extern int num_job_fin;
extern int num_job_fail;
extern pthread_mutex_t num_job_fin_mutex;
extern pthread_mutex_t num_job_fail_mutex;
extern pthread_mutex_t opt_mutex;

extern long long num_insert_msg;
extern long long num_lookup_msg;
extern long long num_comswap_msg;

extern pthread_mutex_t insert_msg_mutex;
extern pthread_mutex_t lookup_msg_mutex;
extern pthread_mutex_t comswap_msg_mutex;

extern pthread_mutex_t global_mutex;

extern int ntime;
extern pthread_mutex_t time_mutex;

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

extern int _zht_client_init(char*, char*, char*);
extern int _c_zht_insert2(char*, char*);
extern void _c_zht_lookup2(char*, char*);

extern char* _allocate_node(char*, char*, char**, int, int, char**);

extern void c_memset(char*, int);

extern char* c_calloc(int);
extern char** c_malloc_2(int, int);
extern char* int_to_str(int);
extern int str_to_int(char*);
extern char** split_str(char*, char*, int, int);
extern int get_size(char**);
extern long long timeval_diff(struct timeval*, struct timeval*, struct timeval*);
#endif /* DATASTR_H_ */
