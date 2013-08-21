/*
 The following is sequence you need to run this test case(c_zhtclient_lanl.c).
 You can also only run step 3 to see what will happened.

 1. to make, run
 make

 2. to insert 1000 nodes, run
 ./c_zhtclient_lanl -n neighbor -z zht.cfg -p TCP -i

 3. to run testcase, run
 ./c_zhtclient_lanl -n neighbor -z zht.cfg -p TCP

 4. to to check nodes finally available, run
 ./c_zhtclient_lanl -n neighbor -z zht.cfg -p TCP -l

 5. to clean, run
 make clean

 6. to allocate nodes by comp_swap on Kodiak, run
 parallel sh -c "./c_zhtclient_lanl -n neighbor -z zht.cfg -p TCP" -- 1 2
 parallel sh -c "./c_zhtclient_lanl -n neighbor -z zht.cfg -p TCP" -- 1 2 3 4
 parallel sh -c "./c_zhtclient_lanl -n neighbor -z zht.cfg -p TCP" -- 1 2 3 4 5 6
 parallel sh -c "./c_zhtclient_lanl -n neighbor -z zht.cfg -p TCP" -- 1 2 3 4 5 6 7 8 9


 => you may probably have parallel not installed, run sudo apt-get install moreutils

 => I changed the startup arguments only for this test programs, e.g. ./c_zhtclient_lanl -n neighbor -z zht.cfg -p TCP
 to see the usage, run ./c_zhtclient_lanl or ./c_zhtclient_lanl -h

 => parallel sh -c "./c_zhtclient_lanl -n neighbor -z zht.cfg -p TCP" -- 1 2

 -- 1 2, means to run c_zhtclient in two processes in parallel, this emulates two concurrent clients that invoke com_swap
 -- similarly, 1 2 3, means in three processes in parallel
 -- ...

 => parallel util is used here, just because I have to protect the shared variables(resulting in sequentially access somehow)
 by meams of multi-threaded invocation of com_swap, not really emulate concurrent clients that invokes com_swap


 ###########special attention###########
 1. this testcase is not dealt with multi-threads to simulate concurrent requests of nodes allocation, see
 <c_zhtclient_lanl_threaded.c> for that purpose.
 * */
#include   <stdbool.h>
#include   <stdlib.h>
#include   <stdio.h>

#include   <string.h>
#include "c_zhtclient.h"
#include "meta.pb-c.h"

#include <pthread.h>
#include <assert.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>

#define RC_NO_NODES_AVAIL -1000

#define NODES_ALLOC  100

pid_t CUR_PID;

char *key = "node_res";

const int LOOKUP_SIZE = 1024 * 2; //size of buffer to store lookup result, larger enough than TOTAL_SIZE

double next_double() {

	srand(time(0));

	return ((double) rand()) / RAND_MAX;
}

int lookup_nodes() {

	size_t ln;
	char *result = (char*) calloc(LOOKUP_SIZE, sizeof(char));

	if (result != NULL ) {

		int lret = c_zht_lookup2(key, result, &ln); //1 for look up, 2 for remove, 3 for insert, 4 for append

		/*		fprintf(stdout, "[%d] c_zht_lookup, return code(length): %d(%lu)\n",
		 seq_num, lret, ln);*/

		fprintf(stdout,
				"[%d] c_zht_lookup, return {key}:{value}=> {%s}:{%s}, rc(%d)\n",
				CUR_PID, key, result, lret);
	}

	int nodes = atoi(result);

	free(result);

	return nodes;
}

int compare_swap_internal(int nodes_avail, bool first_called,
		int *value_queried) {

	int nodes_left;

	if (first_called) {

		nodes_left = nodes_avail - NODES_ALLOC; //allocate 100 nodes
	} else {

		nodes_left = *value_queried - NODES_ALLOC; //allocate 100 nodes
	}

	/*todo: guard the case: nodes_left < 0, means no nodes available*/
	if (nodes_left < 0)
		return RC_NO_NODES_AVAIL; //for example, -1000 means no nodes available

	char p_nodes_avail[20];
	sprintf(p_nodes_avail, "%d", nodes_avail);

	char p_nodes_left[20];
	sprintf(p_nodes_left, "%d", nodes_left);

	char *p_nodes_queried = calloc(20, sizeof(char));

	int rc = c_zht_compare_and_swap(key, p_nodes_avail, p_nodes_left,
			&p_nodes_queried);

	*value_queried = atoi(p_nodes_queried);

	fprintf(stdout,
			"[%lu] c_zht_compare_and_swap, {seen_value}:{new_value}:{value_queried} => {%s}:{%s}:{%s}, rc(%d)\n",
			pthread_self(), p_nodes_avail, p_nodes_left, p_nodes_queried, rc);

	free(p_nodes_queried);

	return rc;
}

void compare_swap() {

	int node_avail = lookup_nodes();

	int value_queried = 0;

	int rc = compare_swap_internal(node_avail, true, &value_queried);

	if (rc == 1)
		return;

	if (rc == RC_NO_NODES_AVAIL)
		return; //todo: here,  you may need to return RC_NO_NODES_AVAIL to caller application

	while (rc == 0) {

		rc = compare_swap_internal(node_avail, false, &value_queried);
	}
}

void compare_swap_final() {

	int thread_mock = 6;
	int nodes_avail = lookup_nodes(thread_mock);

	assert(nodes_avail == 900);
}

void insert_resource() {

	char *nodes = "1000";

	int rc = c_zht_insert2(key, nodes);

	fprintf(stdout, "[%d] init [%s] nodes available, rc(%d)\n", CUR_PID, nodes,
			rc);
}

void test_compare_swap() {

	insert_resource();

	compare_swap();

	compare_swap_final();
}

void printUsage(char *argv_0);

int main(int argc, char **argv) {

	CUR_PID = getpid();

	extern char *optarg;

	double us = 0;
	int printHelp = 0;

	char *neighbor = NULL;
	char *zht_cfg = NULL;
	char *protocol = NULL;
	int is_init = 0;
	int is_lookup = 0;

	int c;
	while ((c = getopt(argc, argv, "n:z:p:ilh")) != -1) {
		switch (c) {
		case 'n':
			neighbor = optarg;
			break;
		case 'z':
			zht_cfg = optarg;
			break;
		case 'p':
			protocol = optarg;
			break;
		case 'i':
			is_init = 1;
			break;
		case 'l':
			is_lookup = 1;
			break;
		case 'h':
			printHelp = 1;
			break;
		default:
			fprintf(stdout, "Illegal argument \"%c\"\n", c);
			printUsage(argv[0]);
			exit(1);
		}
	}

	if (printHelp) {
		printUsage(argv[0]);
		exit(1);
	}

	if (neighbor != NULL && zht_cfg != NULL && protocol != NULL ) {

		bool useTCP = false;

		if (!strcmp("TCP", protocol)) {
			useTCP = true;
		} else {
			useTCP = false;
		}

		/*init...*/
		c_zht_init(neighbor, zht_cfg, useTCP); //neighbor zht.cfg TCP

		if (is_init) {

			insert_resource();
		} else if (is_lookup) {

			lookup_nodes();
		} else {
			test_compare_swap();
		}

		/*clear...*/
		c_zht_teardown();

	} else {

		printUsage(argv[0]);
		exit(1);
	}

	return 0;
}

void printUsage(char *argv_0) {

	fprintf(stdout, "Usage:\n%s %s\n", argv_0,
			"{-n neighbor -z zht.cfg -p TCP|UDP -i(init)} | -l(lookup) | {-h(help)}");
}
