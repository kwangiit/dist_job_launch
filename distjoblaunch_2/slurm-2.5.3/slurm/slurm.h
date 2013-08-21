/* slurm/slurm.h.  Generated from slurm.h.in by configure.  */
/*****************************************************************************\
 *  slurm.h - Definitions for all of the SLURM RPCs
 *****************************************************************************
 *  Copyright (C) 2002-2007 The Regents of the University of California.
 *  Copyright (C) 2008-2010 Lawrence Livermore National Security.
 *  Portions Copyright (C) 2010 SchedMD <http://www.schedmd.com>.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Morris Jette <jette1@llnl.gov>, et. al.
 *  CODE-OCEC-09-009. All rights reserved.
 *
 *  This file is part of SLURM, a resource management program.
 *  For details, see <http://www.schedmd.com/slurmdocs/>.
 *  Please also read the included file: DISCLAIMER.
 *
 *  SLURM is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission
 *  to link the code of portions of this program with the OpenSSL library under
 *  certain conditions as described in each individual source file, and
 *  distribute linked combinations including the two. You must obey the GNU
 *  General Public License in all respects for all of the code used other than
 *  OpenSSL. If you modify file(s) with this exception, you may extend this
 *  exception to your version of the file(s), but you are not obligated to do
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in
 *  the program, then also delete it here.
 *
 *  SLURM is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with SLURM; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#ifndef _SLURM_H
#define _SLURM_H

/* BEGIN_C_DECLS should be used at the beginning of your declarations,
   so that C++ compilers don't mangle their names.  Use END_C_DECLS at
   the end of C declarations. */
/* #undef BEGIN_C_DECLS */
/* #undef END_C_DECLS */
#ifdef __cplusplus
# define BEGIN_C_DECLS	extern "C" {
# define END_C_DECLS	}
#else
# define BEGIN_C_DECLS	/* empty */
# define END_C_DECLS	/* empty */
#endif

/* PARAMS is a macro used to wrap function prototypes, so that compilers
   that don't understand ANSI C prototypes still work, and ANSI C
   compilers can issue warnings about type mismatches.  */
/* #undef PARAMS */
#if defined (__STDC__) || defined (_AIX)			\
	|| (defined (__mips) && defined (_SYSTYPE_SVR4))	\
	|| defined(WIN32) || defined(__cplusplus)
# define PARAMS(protos)	protos
#else
# define PARAMS(protos)	()
#endif

/* Define to 1 if you have Blue Gene system support. */
/* #undef HAVE_BG */

/* Define to 1 if emulating or running on Blue Gene/L system */
/* #undef HAVE_BGL */

/* Define to 1 if emulating or running on Blue Gene/P system */
/* #undef HAVE_BGP */

/* Define to 1 if emulating or running on Blue Gene/L-P system */
/* #undef HAVE_BG_L_P */

/* Define to 1 if emulating or running on Blue Gene/Q system */
/* #undef HAVE_BGQ */

/* Number of dimensions the system has */
#define SYSTEM_DIMENSIONS 1
#define HIGHEST_DIMENSIONS 5

/* Define to 1 if you have the `elan3' library (-lelan3). */
/* #undef HAVE_ELAN */

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Default value for job's --wait-all-nodes option */
#ifndef DEFAULT_WAIT_ALL_NODES
#define DEFAULT_WAIT_ALL_NODES 0	/* don't wait */
#endif

BEGIN_C_DECLS

#include <slurm/slurm_errno.h>

#if HAVE_STDINT_H
#  include <stdint.h>		/* for uint16_t, uint32_t definitions */
#endif
#if HAVE_INTTYPES_H
#  include <inttypes.h>		/* for uint16_t, uint32_t definitions */
#endif
#include <stdbool.h>
#include <stdio.h>		/* for FILE definitions */
#include <sys/types.h>		/* for uid_t definition */
#include <time.h>		/* for time_t definitions */
#include <unistd.h>

#ifdef CRAPPY_COMPILER
/*
 * 'bool' can be implemented in a variety of ways.
 * C++ may define it one way.
 * <stdbool.h> may declare 'bool' to be a different type, such as
 * an enum which is not necessarily compatible with C++.
 *
 * If your compiler can't resolve 'bool', just define
 * CRAPPY_COMPILER at build time in order for SLURM to
 * define it's own version of bool.
 */

/* #undef TRUE */
#define TRUE   1

/* #undef FALSE */
#define FALSE  0

typedef unsigned char SLURM_BOOL;
/* #undef bool */
#define bool SLURM_BOOL
#endif

/* Define slurm_addr_t below to avoid including extraneous slurm headers */
#ifdef	HAVE_SYS_SOCKET_H
#  ifndef __slurm_addr_t_defined
#    include <netinet/in.h>
#    define  __slurm_addr_t_defined
typedef struct sockaddr_in slurm_addr_t;
#  endif
#endif

/* Just incase we don't have a c99 compatible system.  This should be
 * defined in inttypes.h.
 */
#ifndef PRIu64
#  define PRIu64 "llu"
#endif

#ifndef __slurm_cred_t_defined
#  define __slurm_cred_t_defined
typedef struct slurm_job_credential slurm_cred_t;
#endif

/* Define switch_jobinfo_t below to avoid including extraneous slurm headers */
#ifndef __switch_jobinfo_t_defined
#  define  __switch_jobinfo_t_defined
typedef struct switch_jobinfo switch_jobinfo_t;	/* opaque data type */
#endif

/* Define job_resources_t below
 * to avoid including extraneous slurm headers */
#ifndef __job_resources_t_defined
#  define  __job_resources_t_defined	/* Opaque data for select plugins */
typedef struct job_resources job_resources_t;
#endif

/* Define select_jobinfo_t, select_nodeinfo_t below
 * to avoid including extraneous slurm headers */
#ifndef __select_jobinfo_t_defined
#  define  __select_jobinfo_t_defined	/* Opaque data for select plugins */
typedef struct select_jobinfo select_jobinfo_t;  /* for BlueGene */
typedef struct select_nodeinfo select_nodeinfo_t;  /* for BlueGene */
#endif

/* Define jobacctinfo_t below to avoid including extraneous slurm headers */
#ifndef __jobacctinfo_t_defined
#  define  __jobacctinfo_t_defined
typedef struct jobacctinfo jobacctinfo_t;     /* opaque data type */
#endif

/* Define allocation_msg_thread_t below to avoid including extraneous
 * slurm headers */
#ifndef __allocation_msg_thread_t_defined
#  define  __allocation_msg_thread_t_defined
typedef struct allocation_msg_thread allocation_msg_thread_t;
#endif

#ifndef __sbcast_cred_t_defined
#  define  __sbcast_cred_t_defined
typedef struct sbcast_cred sbcast_cred_t;		/* opaque data type */
#endif

/*****************************************************************************\
 *      DEFINITIONS FOR VERSION MANAGEMENT
\*****************************************************************************/
/* Define SLURM version number.
 * High-order byte is major version.
 * Middle byte     is minor version.
 * Low-order byte  is micro version (NOTE: excludes "-pre#" component
 #                 of micro version used in pre-releases).
 * Use SLURM_VERSION_NUM macro to compare versions, for example
 * #if SLURM_VERSION_NUMBER > SLURM_VERSION_NUM(2,1,0)
 */
#define SLURM_VERSION_NUMBER 0x020503
#define SLURM_VERSION_NUM(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define SLURM_VERSION_MAJOR(a)   (((a) >> 16) & 0xff)
#define SLURM_VERSION_MINOR(a)   (((a) >>  8) & 0xff)
#define SLURM_VERSION_MICRO(a)    ((a)        & 0xff)

/*****************************************************************************\
 *	DEFINITIONS FOR INPUT VALUES
\*****************************************************************************/

/* INFINITE is used to identify unlimited configurations,  */
/* eg. the maximum count of nodes any job may use in some partition */
#define	INFINITE (0xffffffff)
#define NO_VAL	 (0xfffffffe)
#define MAX_TASKS_PER_NODE 128

/* Job step ID of batch scripts */
#define SLURM_BATCH_SCRIPT (0xfffffffe)

/* last entry must be JOB_END, keep in sync with job_state_string and
 *	job_state_string_compact. values may be ORed with JOB_STATE_FLAGS
 *	below.  */
enum job_states {
	JOB_PENDING,		/* queued waiting for initiation */
	JOB_RUNNING,		/* allocated resources and executing */
	JOB_SUSPENDED,		/* allocated resources, execution suspended */
	JOB_COMPLETE,		/* completed execution successfully */
	JOB_CANCELLED,		/* cancelled by user */
	JOB_FAILED,		/* completed execution unsuccessfully */
	JOB_TIMEOUT,		/* terminated on reaching time limit */
	JOB_NODE_FAIL,		/* terminated on node failure */
	JOB_PREEMPTED,		/* terminated due to preemption */
	JOB_END			/* not a real state, last entry in table */
};
#define	JOB_STATE_BASE	0x00ff	/* Used for job_states above */
#define	JOB_STATE_FLAGS	0xff00	/* Used for state flags below */
#define	JOB_COMPLETING	0x8000	/* Waiting for epilog completion */
#define	JOB_CONFIGURING	0x4000	/* Allocated nodes booting */
#define	JOB_RESIZING	0x2000	/* Size of job about to change, flag set
				 * before calling accounting functions
				 * immediately before job changes size */

#define READY_JOB_FATAL	   -2	/* fatal error */
#define READY_JOB_ERROR    -1	/* ordinary error */
#define READY_NODE_STATE 0x01	/* node is ready */
#define READY_JOB_STATE  0x02	/* job is ready to execute */

#define MAIL_JOB_BEGIN    0x0001	/* notify when job begins */
#define MAIL_JOB_END      0x0002	/* notify when job ends */
#define MAIL_JOB_FAIL     0x0004	/* notify if job fails */
#define MAIL_JOB_REQUEUE  0x0008	/* notify if job requeued */

#define NICE_OFFSET 10000	/* offset for job's nice value */

/* Reason for job to be pending rather than executing or reason for job
 * failure. If multiple reasons exists, only one is given for the sake of
 * system efficiency */
enum job_state_reason {
/* Reasons for job to be pending */
	WAIT_NO_REASON = 0,	/* not set or job not pending */
	WAIT_PRIORITY,		/* higher priority jobs exist */
	WAIT_DEPENDENCY,	/* dependent job has not completed */
	WAIT_RESOURCES,		/* required resources not available */
	WAIT_PART_NODE_LIMIT,	/* request exceeds partition node limit */
	WAIT_PART_TIME_LIMIT,	/* request exceeds partition time limit */
	WAIT_PART_DOWN,		/* requested partition is down */
	WAIT_PART_INACTIVE,	/* requested partition is inactive */
	WAIT_HELD,		/* job is held by administrator */
	WAIT_TIME,		/* job waiting for specific begin time */
	WAIT_LICENSES,		/* job is waiting for licenses */
	WAIT_ASSOC_JOB_LIMIT,	/* user/bank job limit reached */
	WAIT_ASSOC_RESOURCE_LIMIT,/* user/bank resource limit reached */
	WAIT_ASSOC_TIME_LIMIT,  /* user/bank time limit reached */
	WAIT_RESERVATION,	/* reservation not available */
	WAIT_NODE_NOT_AVAIL,	/* required node is DOWN or DRAINED */
	WAIT_HELD_USER,		/* job is held by user */
	WAIT_FRONT_END,		/* Front end nodes are DOWN */
	FAIL_DOWN_PARTITION,	/* partition for job is DOWN */
	FAIL_DOWN_NODE,		/* some node in the allocation failed */
	FAIL_BAD_CONSTRAINTS,	/* constraints can not be satisfied */
	FAIL_SYSTEM,		/* slurm system failure */
	FAIL_LAUNCH,		/* unable to launch job */
	FAIL_EXIT_CODE,		/* exit code was non-zero */
	FAIL_TIMEOUT,		/* reached end of time limit */
	FAIL_INACTIVE_LIMIT,	/* reached slurm InactiveLimit */
	FAIL_ACCOUNT,   	/* invalid account */
	FAIL_QOS,        	/* invalid QOS */
	WAIT_QOS_THRES,        	/* required QOS threshold has been breached */
	WAIT_QOS_JOB_LIMIT,	/* QOS job limit reached */
	WAIT_QOS_RESOURCE_LIMIT,/* QOS resource limit reached */
	WAIT_QOS_TIME_LIMIT	/* QOS time limit reached */
};

enum job_acct_types {
	JOB_START,
	JOB_STEP,
	JOB_SUSPEND,
	JOB_TERMINATED
};

/* Partition state flags */
#define PARTITION_SUBMIT	0x01	/* Allow job submission to partition */
#define PARTITION_SCHED 	0x02	/* Allow job startup from partition */

/* Actual partition states based upon state flags */
#define PARTITION_DOWN		(PARTITION_SUBMIT)
#define PARTITION_UP		(PARTITION_SUBMIT | PARTITION_SCHED)
#define PARTITION_DRAIN		(PARTITION_SCHED)
#define PARTITION_INACTIVE	0x00

enum connection_type {
	SELECT_MESH, 		/* nodes wired in mesh */
	SELECT_TORUS, 		/* nodes wired in torus */
	SELECT_NAV,		/* nodes wired in torus else mesh */
	SELECT_SMALL, 		/* nodes in a small partition */
	SELECT_HTC_S,           /* nodes in a htc running SMP mode */
	SELECT_HTC_D,           /* nodes in a htc running Dual mode */
	SELECT_HTC_V,           /* nodes in a htc running VN mode */
	SELECT_HTC_L            /* nodes in a htc running in Linux mode */
};

enum node_use_type {
	SELECT_COPROCESSOR_MODE,/* use extra processor for communications */
	SELECT_VIRTUAL_NODE_MODE,/* application uses both processors */
	SELECT_NAV_MODE		/* either mode is acceptable */
};

enum select_jobdata_type {
	SELECT_JOBDATA_GEOMETRY,/* data-> uint16_t geometry[SYSTEM_DIMENSIONS] */
	SELECT_JOBDATA_ROTATE,	/* data-> uint16_t rotate */
	SELECT_JOBDATA_CONN_TYPE,	/* data-> uint16_t connection_type */
	SELECT_JOBDATA_BLOCK_ID,	/* data-> char *bg_block_id */
	SELECT_JOBDATA_NODES,	/* data-> char *nodes */
	SELECT_JOBDATA_IONODES,	/* data-> char *ionodes */
	SELECT_JOBDATA_NODE_CNT,	/* data-> uint32_t cnode_cnt */
	SELECT_JOBDATA_ALTERED,    /* data-> uint16_t altered */
	SELECT_JOBDATA_BLRTS_IMAGE,/* data-> char *blrtsimage */
	SELECT_JOBDATA_LINUX_IMAGE,/* data-> char *linuximage */
	SELECT_JOBDATA_MLOADER_IMAGE,/* data-> char *mloaderimage */
	SELECT_JOBDATA_RAMDISK_IMAGE,/* data-> char *ramdiskimage */
	SELECT_JOBDATA_REBOOT,	/* data-> uint16_t reboot */
	SELECT_JOBDATA_RESV_ID,	/* data-> uint32_t reservation_id */
	SELECT_JOBDATA_PAGG_ID,	/* data-> uint64_t job container ID */
	SELECT_JOBDATA_PTR,	/* data-> select_jobinfo_t *jobinfo */
	SELECT_JOBDATA_BLOCK_PTR, /* data-> bg_record_t *bg_record */
	SELECT_JOBDATA_DIM_CNT, /* data-> uint16_t dim_cnt */
	SELECT_JOBDATA_BLOCK_NODE_CNT,	/* data-> uint32_t block_cnode_cnt */
	SELECT_JOBDATA_START_LOC, /* data-> uint16_t
				   * start_loc[SYSTEM_DIMENSIONS] */
	SELECT_JOBDATA_USER_NAME, /* data-> char * user_name */
	SELECT_JOBDATA_CONFIRMED, /* data-> uint8_t ALPS reservation confirmed */
};

enum select_nodedata_type {
	SELECT_NODEDATA_BITMAP_SIZE, /* data-> uint16_t */
	SELECT_NODEDATA_SUBGRP_SIZE, /* data-> uint16_t */
	SELECT_NODEDATA_SUBCNT,      /* data-> uint16_t */
	SELECT_NODEDATA_BITMAP,      /* data-> bitstr_t * needs to be
				      * freed with FREE_NULL_BITMAP */
	SELECT_NODEDATA_STR,         /* data-> char *  needs to be
				      * freed with xfree */
	SELECT_NODEDATA_PTR,         /* data-> select_nodeinfo_t *nodeinfo */
	SELECT_NODEDATA_EXTRA_INFO,  /* data-> char *  needs to be
				      * freed with xfree */
	SELECT_NODEDATA_RACK_MP,     /* data-> char * needs to be
				      * freed with xfree */
};

enum select_print_mode {
	SELECT_PRINT_HEAD,	/* Print just the header */
	SELECT_PRINT_DATA,	/* Print just the data */
	SELECT_PRINT_MIXED,	/* Print "field=value" */
	SELECT_PRINT_MIXED_SHORT,/* Print less "field=value" */
	SELECT_PRINT_BG_ID,	/* Print just the BG_ID */
	SELECT_PRINT_NODES,	/* Print the nodelist */
	SELECT_PRINT_CONNECTION,/* Print just the CONNECTION type */
	SELECT_PRINT_ROTATE,    /* Print just the ROTATE */
	SELECT_PRINT_GEOMETRY,	/* Print just the GEO */
	SELECT_PRINT_START,	/* Print just the START location */
	SELECT_PRINT_BLRTS_IMAGE,/* Print just the BLRTS IMAGE */
	SELECT_PRINT_LINUX_IMAGE,/* Print just the LINUX IMAGE */
	SELECT_PRINT_MLOADER_IMAGE,/* Print just the MLOADER IMAGE */
	SELECT_PRINT_RAMDISK_IMAGE,/* Print just the RAMDISK IMAGE */
	SELECT_PRINT_REBOOT,	/* Print just the REBOOT */
	SELECT_PRINT_RESV_ID,	/* Print just Cray/BASIL reservation ID */
	SELECT_PRINT_START_LOC,	/* Print just the start location */
};

enum select_node_cnt {
	SELECT_GET_NODE_SCALING,      /* Give scaling factor for node count */
	SELECT_GET_NODE_CPU_CNT,      /* Give how many cpus are on a node */
	SELECT_GET_MP_CPU_CNT,        /* Give how many cpus are on a
				       * base partition */
	SELECT_APPLY_NODE_MIN_OFFSET, /* Apply min offset to variable */
	SELECT_APPLY_NODE_MAX_OFFSET, /* Apply max offset to variable */
	SELECT_SET_NODE_CNT,	      /* Set altered node cnt */
	SELECT_SET_MP_CNT             /* Given a node cnt return the
				       * base partition count */
};

/* jobacct data types */
enum jobacct_data_type {
	JOBACCT_DATA_TOTAL,	/* data-> jobacctinfo_t * */
	JOBACCT_DATA_PIPE,      /* data-> file descriptor */
	JOBACCT_DATA_RUSAGE,	/* data-> rusage set user_cpu_sec,
				 * user_cpu_usec, sys_cpu_sec, sys_cpu_usec */
	JOBACCT_DATA_MAX_VSIZE,	/* data-> uint32_t vsize */
	JOBACCT_DATA_MAX_VSIZE_ID,	/* data-> jobacct_id_t vsize */
	JOBACCT_DATA_TOT_VSIZE,	/* data-> uint32_t vsize */
	JOBACCT_DATA_MAX_RSS,	/* data-> uint32_t psize */
	JOBACCT_DATA_MAX_RSS_ID,	/* data-> jobacct_id_t psize */
	JOBACCT_DATA_TOT_RSS,	/* data-> uint32_t psize */
	JOBACCT_DATA_MAX_PAGES,	/* data-> uint32_t psize */
	JOBACCT_DATA_MAX_PAGES_ID,	/* data-> jobacct_id_t psize */
	JOBACCT_DATA_TOT_PAGES,	/* data-> uint32_t psize */
	JOBACCT_DATA_MIN_CPU,	/* data-> uint32_t psize */
	JOBACCT_DATA_MIN_CPU_ID,	/* data-> jobacct_id_t psize */
	JOBACCT_DATA_TOT_CPU,	/* data-> uint32_t psize */
	JOBACCT_DATA_ACT_CPUFREQ, /* data-> uint32_t psize hb*/
	JOBACCT_DATA_CONSUMED_ENERGY /* data-> uint32_t psize hb*/
};

enum acct_energy_type {
	ENERGY_DATA_JOULES_TASK,
	ENERGY_DATA_STRUCT,
	ENERGY_DATA_RECONFIG,
};

/* Possible task distributions across the nodes */
typedef enum task_dist_states {
	/* NOTE: start SLURM_DIST_CYCLIC at 1 for HP MPI */
	SLURM_DIST_CYCLIC = 1,	/* distribute tasks 1 per node, round robin */
	SLURM_DIST_BLOCK,	/* distribute tasks filling node by node */
	SLURM_DIST_ARBITRARY,	/* arbitrary task distribution  */
	SLURM_DIST_PLANE,	/* distribute tasks by filling up
				   planes of lllp first and then by
				   going across the nodes See
				   documentation for more
				   information */
	SLURM_DIST_CYCLIC_CYCLIC,/* distribute tasks 1 per node,
				    round robin, same for lowest
				    level of logical processor (lllp) */
	SLURM_DIST_CYCLIC_BLOCK, /* cyclic for node and block for lllp  */
	SLURM_DIST_BLOCK_CYCLIC, /* block for node and cyclic for lllp  */
	SLURM_DIST_BLOCK_BLOCK,	/* block for node and block for lllp  */
	SLURM_NO_LLLP_DIST,	/* No distribution specified for lllp */
	SLURM_DIST_UNKNOWN	/* unknown dist */
} task_dist_states_t;

/* Open stdout/err file mode, 0 for system default (JobFileAppend) */
#define OPEN_MODE_APPEND	1
#define OPEN_MODE_TRUNCATE	2

typedef enum cpu_bind_type {	/* cpu binding type from --cpu_bind=... */
	/* verbose can be set with any other flag */
	CPU_BIND_VERBOSE   = 0x01, /* =v, */
	/* the following auto-binding flags are mutually exclusive */
	CPU_BIND_TO_THREADS= 0x02, /* =threads */
	CPU_BIND_TO_CORES  = 0x04, /* =cores */
	CPU_BIND_TO_SOCKETS= 0x08, /* =sockets */
	CPU_BIND_TO_LDOMS  = 0x10, /* locality domains */
	CPU_BIND_TO_BOARDS = 0x1000, /* =boards */
	/* the following manual binding flags are mutually exclusive */
	/* CPU_BIND_NONE needs to be the lowest value among manual bindings */
	CPU_BIND_NONE	   = 0x20, /* =no */
	CPU_BIND_RANK  	   = 0x40, /* =rank */
	CPU_BIND_MAP	   = 0x80, /* =map_cpu:<list of CPU IDs> */
	CPU_BIND_MASK	   = 0x100,/* =mask_cpu:<list of CPU masks> */
	CPU_BIND_LDRANK    = 0x200,/* =locality domain rank */
	CPU_BIND_LDMAP	   = 0x400,/* =map_ldom:<list of locality domains> */
	CPU_BIND_LDMASK	   = 0x800,/* =mask_ldom:<list of ldom masks> */

	/* the following is used only as a flag for expressing
	 * the contents of TaskPluginParams */
	CPU_BIND_CPUSETS   = 0x8000
} cpu_bind_type_t;

/* Flag to indicate that cpu_freq is a range: low,medium,high
 * instead of an integer value in kilohertz */
#define CPU_FREQ_RANGE_FLAG 0x80000000
#define CPU_FREQ_LOW        0x80000001
#define CPU_FREQ_MEDIUM     0x80000002
#define CPU_FREQ_HIGH       0x80000003

typedef enum mem_bind_type {    /* memory binding type from --mem_bind=... */
	/* verbose can be set with any other flag */
	MEM_BIND_VERBOSE= 0x01,	/* =v, */
	/* the following manual binding flags are mutually exclusive */
	/* MEM_BIND_NONE needs to be the first in this sub-list */
	MEM_BIND_NONE   = 0x02,	/* =no */
	MEM_BIND_RANK   = 0x04,	/* =rank */
	MEM_BIND_MAP    = 0x08,	/* =map_mem:<list of CPU IDs> */
	MEM_BIND_MASK   = 0x10,	/* =mask_mem:<list of CPU masks> */
	MEM_BIND_LOCAL  = 0x20	/* =local */
} mem_bind_type_t;

/* The last entry in node_states must be STATE_END, keep in sync with
 * node_state_string. values may be ORed with NODE_STATE_FLAGS below.
 * Node states typically alternate between NODE_STATE_IDLE and
 * NODE_STATE_ALLOCATED. The NODE_STATE_COMPLETING flag will be set
 * when jobs are in the process of terminating. */
enum node_states {
	NODE_STATE_UNKNOWN,	/* node's initial state, unknown */
	NODE_STATE_DOWN,	/* node in non-usable state */
	NODE_STATE_IDLE,	/* node idle and available for use */
	NODE_STATE_ALLOCATED,	/* node has been allocated to a job */
	NODE_STATE_ERROR,	/* node is in an error state */
	NODE_STATE_MIXED,	/* node has a mixed state */
	NODE_STATE_FUTURE,	/* node slot reserved for future use */
	NODE_STATE_END		/* last entry in table */
};
#define NODE_STATE_BASE       0x000f
#define NODE_STATE_FLAGS      0xfff0
#define NODE_STATE_CLOUD      0x0080	/* node comes from cloud */
#define NODE_RESUME           0x0100	/* Restore a DRAINED, DRAINING, DOWN
					 * or FAILING node to service (e.g.
					 * IDLE or ALLOCATED). Used in
					 * slurm_update_node() request */
#define NODE_STATE_DRAIN      0x0200	/* node do not new allocated work */
#define NODE_STATE_COMPLETING 0x0400	/* node is completing allocated job */
#define NODE_STATE_NO_RESPOND 0x0800	/* node is not responding */
#define NODE_STATE_POWER_SAVE 0x1000	/* node in power save mode */
#define NODE_STATE_FAIL       0x2000	/* node is failing, do not allocate
					 * new work */
#define NODE_STATE_POWER_UP   0x4000	/* restore power or otherwise
					 * configure a node */
#define NODE_STATE_MAINT      0x8000	/* node in maintenance reservation */

/* used to define the size of the credential.signature size
 * used to define the key size of the io_stream_header_t
 */
#define SLURM_SSL_SIGNATURE_LENGTH 128

/* Used as show_flags for slurm_get_ and slurm_load_ function calls.
 * Values can be can be ORed */
#define SHOW_ALL	0x0001	/* Show info for "hidden" partitions */
#define SHOW_DETAIL	0x0002	/* Show detailed resource information */
#define SHOW_DETAIL2	0x0004	/* Show batch script listing */

/* Define keys for ctx_key argument of slurm_step_ctx_get() */
enum ctx_keys {
	SLURM_STEP_CTX_STEPID,	/* get the created job step id */
	SLURM_STEP_CTX_TASKS,	/* get array of task count on each node */
	SLURM_STEP_CTX_TID,	/* get array of task IDs for specified node */
	SLURM_STEP_CTX_RESP,	/* get job step create response message */
	SLURM_STEP_CTX_CRED,
	SLURM_STEP_CTX_SWITCH_JOB,
	SLURM_STEP_CTX_NUM_HOSTS,
	SLURM_STEP_CTX_HOST,
	SLURM_STEP_CTX_JOBID,
	SLURM_STEP_CTX_USER_MANAGED_SOCKETS
};

/* CR_CPU, CR_SOCKET and CR_CORE are mututally exclusive
 * CR_MEMORY may be added to any of the above values or used by itself
 * CR_ONE_TASK_PER_CORE may also be added to any of the above values */
#define CR_CPU		0x0001	/* Resources are shared down to the level of
				 * logical processors which can be socket,
				 * core, or thread depending on the system. */
#define CR_SOCKET	0x0002	/* Resources are shared down to the socket
				 * level. Jobs will not be co-allocated
				 * within a socket. */
#define CR_CORE		0x0004	/* Resources are shared down to the core level.
				 * Jobs will not be co-allocated within a
				 * core. */
#define CR_BOARD	0x0008	/* Resources are shared down to the board
				 * level. Jobs will not be co-allocated
				 * within a board. */
#define CR_MEMORY	0x0010	/* Memory as consumable resources. Memory is
				 * not over-commited when selected as a CR. */

#define CR_ONE_TASK_PER_CORE 0x0100
/* By default, schedule only one task per core.
 * Without this option, tasks would be
 * allocated threads. */
#define CR_CORE_DEFAULT_DIST_BLOCK 0x1000
/* By default, distribute cores using a block
 * approach inside the nodes */

#define MEM_PER_CPU  0x80000000
#define SHARED_FORCE 0x8000

#define PRIVATE_DATA_JOBS	  0x0001 /* job/step data is private */
#define PRIVATE_DATA_NODES	  0x0002 /* node data is private */
#define PRIVATE_DATA_PARTITIONS	  0x0004 /* partition data is private */
#define PRIVATE_DATA_USAGE	  0x0008 /* accounting usage data is private */
#define PRIVATE_DATA_USERS	  0x0010 /* accounting user data is private */
#define PRIVATE_DATA_ACCOUNTS	  0x0020 /* accounting account data is private*/
#define PRIVATE_DATA_RESERVATIONS 0x0040 /* reservation data is private */

#define PRIORITY_RESET_NONE	0x0000	/* never clear */
#define PRIORITY_RESET_NOW	0x0001	/* clear now (when slurmctld restarts) */
#define PRIORITY_RESET_DAILY	0x0002	/* clear daily at midnight */
#define PRIORITY_RESET_WEEKLY	0x0003	/* clear weekly at Sunday 00:00 */
#define PRIORITY_RESET_MONTHLY	0x0004	/* clear monthly on first at 00:00 */
#define PRIORITY_RESET_QUARTERLY 0x0005	/* clear quarterly on first at 00:00 */
#define PRIORITY_RESET_YEARLY	0x0006	/* clear yearly on first at 00:00 */

#define PROP_PRIO_OFF		0x0000	/* Do not propagage user nice value */
#define PROP_PRIO_ON		0x0001	/* Propagate user nice value */
#define PROP_PRIO_NICER		0x0002	/* Insure that user tasks have a nice
					 * value that is higher than slurmd */

#define PRIORITY_FLAGS_ACCRUE_ALWAYS	0x0001	/* Flag to always accrue age
					 * priority to pending jobs ignoring
					 * dependencies or holds */


/*****************************************************************************\
 *      SLURM HOSTLIST FUNCTIONS
\*****************************************************************************/

/* The hostlist opaque data type
 *
 * A hostlist is a list of hostnames optimized for a prefixXXXX style
 * naming convention, where XXXX  is a decimal, numeric suffix.
 */
#ifndef   __hostlist_t_defined
#  define __hostlist_t_defined
typedef struct hostlist * hostlist_t;
#endif

/*
 * slurm_hostlist_create():
 *
 * Create a new hostlist from a string representation.
 *
 * The string representation (str) may contain one or more hostnames or
 * bracketed hostlists separated by either `,' or whitespace. A bracketed
 * hostlist is denoted by a common prefix followed by a list of numeric
 * ranges contained within brackets: e.g. "tux[0-5,12,20-25]"
 *
 * To support systems with 3-D topography, a rectangular prism may
 * be described using two three digit numbers separated by "x": e.g.
 * "bgl[123x456]". This selects all nodes between 1 and 4 inclusive
 * in the first dimension, between 2 and 5 in the second, and between
 * 3 and 6 in the third dimension for a total of 4*4*4=64 nodes
 *
 * Note: if this module is compiled with WANT_RECKLESS_HOSTRANGE_EXPANSION
 * defined, a much more loose interpretation of host ranges is used.
 * Reckless hostrange expansion allows all of the following (in addition to
 * bracketed hostlists):
 *
 *  o tux0-5,tux12,tux20-25
 *  o tux0-tux5,tux12,tux20-tux25
 *  o tux0-5,12,20-25
 *
 * If str is NULL, and empty hostlist is created and returned.
 *
 * If the create fails, hostlist_create() returns NULL.
 *
 * The returned hostlist must be freed with hostlist_destroy()
 *
 */
extern hostlist_t slurm_hostlist_create PARAMS((const char *hostlist));

/* slurm_hostlist_count():
 *
 * Return the number of hosts in hostlist hl.
 */
extern int slurm_hostlist_count PARAMS((hostlist_t hl));

/*
 * slurm_hostlist_destroy():
 *
 * Destroy a hostlist object. Frees all memory allocated to the hostlist.
 */
extern void slurm_hostlist_destroy PARAMS((hostlist_t hl));

/* slurm_hostlist_find():
 *
 * Searches hostlist hl for the first host matching hostname
 * and returns position in list if found.
 *
 * Returns -1 if host is not found.
 *
 */
extern int slurm_hostlist_find PARAMS((hostlist_t hl, const char *hostname));

/* slurm_hostlist_push():
 *
 * push a string representation of hostnames onto a hostlist.
 *
 * The hosts argument may take the same form as in slurm_hostlist_create()
 *
 * Returns the number of hostnames inserted into the list,
 * or 0 on failure.
 */
extern int slurm_hostlist_push PARAMS((hostlist_t hl, const char *hosts));

/* slurm_hostlist_push_host():
 *
 * Push a single host onto the hostlist hl.
 * This function is more efficient than slurm_hostlist_push() for a single
 * hostname, since the argument does not need to be checked for ranges.
 *
 * return value is 1 for success, 0 for failure.
 */
extern int slurm_hostlist_push_host PARAMS((hostlist_t hl, const char *host));

/* slurm_hostlist_ranged_string():
 *
 * Write the string representation of the hostlist hl into buf,
 * writing at most n chars. Returns the number of bytes written,
 * or -1 if truncation occurred.
 *
 * The result will be NULL terminated.
 *
 * slurm_hostlist_ranged_string() will write a bracketed hostlist representation
 * where possible.
 */
extern ssize_t slurm_hostlist_ranged_string PARAMS((hostlist_t hl,
						    size_t n, char *buf));

/* slurm_hostlist_ranged_string_malloc():
 *
 * Return the string representation of the hostlist hl.
 *
 * The result must be released using free();
 */
extern char *slurm_hostlist_ranged_string_malloc PARAMS((hostlist_t hl));

/* hostlist_ranged_string_xmalloc():
 *
 * Wrapper of hostlist_ranged_string(), with result buffer dynamically
 * allocated using xmalloc().
 * The result will be NULL on failure (out of memory).
 *
 * Caller should free the result string using xfree().
 */
extern char *slurm_hostlist_ranged_string_xmalloc PARAMS((hostlist_t hl));

/*
 * slurm_hostlist_shift():
 *
 * Returns the string representation of the first host in the hostlist
 * or NULL if the hostlist is empty or there was an error allocating memory.
 * The host is removed from the hostlist.
 *
 * Note: Caller is responsible for freeing the returned memory.
 */
extern char * slurm_hostlist_shift PARAMS((hostlist_t hl));

/* slurm_hostlist_uniq():
 *
 * Sort the hostlist hl and remove duplicate entries.
 *
 */
extern void slurm_hostlist_uniq PARAMS((hostlist_t hl));

/*****************************************************************************\
 *      SLURM LIST FUNCTIONS
\*****************************************************************************/

#ifndef   __list_datatypes_defined
#  define __list_datatypes_defined
typedef struct list * List;
/*
 *  List opaque data type.
 */

typedef struct listIterator * ListIterator;
/*
 *  List Iterator opaque data type.
 */

typedef void (*ListDelF) (void *x);
/*
 *  Function prototype to deallocate data stored in a list.
 *    This function is responsible for freeing all memory associated
 *    with an item, including all subordinate items (if applicable).
 */

typedef int (*ListCmpF) (void *x, void *y);
/*
 *  Function prototype for comparing two items in a list.
 *  Returns less-than-zero if (x<y), zero if (x==y), and
 *    greather-than-zero if (x>y).
 */

typedef int (*ListFindF) (void *x, void *key);
/*
 *  Function prototype for matching items in a list.
 *  Returns non-zero if (x==key); o/w returns zero.
 */

typedef int (*ListForF) (void *x, void *arg);
/*
 *  Function prototype for operating on each item in a list.
 *  Returns less-than-zero on error.
 */
#endif

/* slurm_list_append():
 *
 *  Inserts data [x] at the end of list [l].
 *  Returns the data's ptr, or lsd_nomem_error() if insertion failed.
 */
extern void * slurm_list_append PARAMS((List l, void *x));

/* slurm_list_count():
 *
 *  Returns the number of items in list [l].
 */
extern int slurm_list_count PARAMS((List l));

/* slurm_list_create():
 *
 *  Creates and returns a new empty list, or lsd_nomem_error() on failure.
 *  The deletion function [f] is used to deallocate memory used by items
 *    in the list; if this is NULL, memory associated with these items
 *    will not be freed when the list is destroyed.
 *  Note: Abandoning a list without calling slurm_list_destroy() will result
 *    in a memory leak.
 */
extern List slurm_list_create PARAMS((ListDelF f));

/* slurm_list_destroy():
 *
 *  Destroys list [l], freeing memory used for list iterators and the
 *    list itself; if a deletion function was specified when the list
 *    was created, it will be called for each item in the list.
 */
extern void slurm_list_destroy PARAMS((List l));

/* slurm_list_find():
 *
 *  Traverses the list from the point of the list iterator [i]
 *    using [f] to match each item with [key].
 *  Returns a ptr to the next item for which the function [f]
 *    returns non-zero, or NULL once the end of the list is reached.
 *  Example: i=slurm_list_iterator_reset(i);
 *	     while ((x=slurm_list_find(i,f,k))) {...}
 */
extern void * slurm_list_find PARAMS((ListIterator i, ListFindF f, void *key));

/* slurm_list_is_empty():
 *
 *  Returns non-zero if list [l] is empty; o/w returns zero.
 */
extern int slurm_list_is_empty PARAMS((List l));

/*
 *  Creates and returns a list iterator for non-destructively traversing
 *    list [l], or lsd_nomem_error() on failure.
 */
extern ListIterator slurm_list_iterator_create PARAMS((List l));

/* slurm_list_iterator_reset():
 *
 *  Resets the list iterator [i] to start traversal at the beginning
 *    of the list.
 */
extern void slurm_list_iterator_reset PARAMS((ListIterator i));

/*
 *  Destroys the list iterator [i]; list iterators not explicitly destroyed
 *    in this manner will be destroyed when the list is deallocated via
 *    list_destroy().
 */
extern void slurm_list_iterator_destroy PARAMS((ListIterator i));

/* slurm_list_next():
 *
 *  Returns a ptr to the next item's data,
 *    or NULL once the end of the list is reached.
 *  Example: i=slurm_list_iterator_create(i);
 *	     while ((x=slurm_list_next(i))) {...}
 */
extern void * slurm_list_next PARAMS((ListIterator i));

/* slurm_list_sort():
 *
 *  Sorts list [l] into ascending order according to the function [f].
 *  Note: Sorting a list resets all iterators associated with the list.
 *  Note: The sort algorithm is stable.
 */
extern void slurm_list_sort PARAMS((List l, ListCmpF f));

/*****************************************************************************\
 *      SLURM BITSTR FUNCTIONS
\*****************************************************************************/

#ifndef   __bitstr_datatypes_defined
#  define __bitstr_datatypes_defined

#ifdef USE_64BIT_BITSTR
typedef int64_t bitstr_t;
#define BITSTR_SHIFT 		BITSTR_SHIFT_WORD64
#else
typedef int32_t bitstr_t;
#define BITSTR_SHIFT 		BITSTR_SHIFT_WORD32
#endif

typedef bitstr_t bitoff_t;

#endif

#define ALLOC_SID_ADMIN_HOLD	0x00000001	/* admin job hold */
#define ALLOC_SID_USER_HOLD	0x00000002	/* user job hold */

/*****************************************************************************\
 *	PROTOCOL DATA STRUCTURE DEFINITIONS
\*****************************************************************************/
typedef struct dynamic_plugin_data {
	void *data;
	uint32_t plugin_id;
} dynamic_plugin_data_t;

typedef struct acct_gather_energy {
	uint32_t previous_consumed_energy;
	uint32_t base_consumed_energy;
	uint32_t base_watts;	  /* lowest power consump of node, in watts */
	uint32_t consumed_energy; /* total energy consumed by node, in joules */
	uint32_t current_watts;	  /* current power consump of node, in watts */
} acct_gather_energy_t;

typedef struct job_descriptor {	/* For submit, allocate, and update requests */
	char *account;		/* charge to specified account */
	uint16_t acctg_freq;	/* accounting polling interval (seconds) */
	char *alloc_node;	/* node making resource allocation request
				 * NOTE: Normally set by slurm_submit* or
				 * slurm_allocate* function */
	uint16_t alloc_resp_port; /* port to send allocation confirmation to */
	uint32_t alloc_sid;	/* local sid making resource allocation request
				 * NOTE: Normally set by slurm_submit* or
				 * slurm_allocate* function
				 * NOTE: Also used for update flags, see
				 * ALLOC_SID_* flags */
	uint32_t argc;		/* number of arguments to the script */
	char **argv;		/* arguments to the script */
	time_t begin_time;	/* delay initiation until this time */
	uint16_t ckpt_interval;	/* periodically checkpoint this job */
	char *ckpt_dir;	 	/* directory to store checkpoint images */
	char *comment;		/* arbitrary comment (used by Moab scheduler) */
	uint16_t contiguous;	/* 1 if job requires contiguous nodes,
				 * 0 otherwise,default=0 */
	char *cpu_bind;		/* binding map for map/mask_cpu */
	uint16_t cpu_bind_type;	/* see cpu_bind_type_t */
	char *dependency;	/* synchronize job execution with other jobs */
	time_t end_time;	/* time by which job must complete, used for
				 * job update only now, possible deadline
				 * scheduling in the future */
	char **environment;	/* environment variables to set for job,
				 *  name=value pairs, one per line */
	uint32_t env_size;	/* element count in environment */
	char *exc_nodes;	/* comma separated list of nodes excluded
				 * from job's allocation, default NONE */
	char *features;		/* comma separated list of required features,
				 * default NONE */
	char *gres;		/* comma separated list of required generic
				 * resources, default NONE */
	uint32_t group_id;	/* group to assume, if run as root. */
	uint16_t immediate;	/* 1 if allocate to run or fail immediately,
				 * 0 if to be queued awaiting resources */
	uint32_t job_id;	/* job ID, default set by SLURM */
	uint16_t kill_on_node_fail; /* 1 if node failure to kill job,
				     * 0 otherwise,default=1 */
	char *licenses;		/* licenses required by the job */
	uint16_t mail_type;	/* see MAIL_JOB_ definitions above */
	char *mail_user;	/* user to receive notification */
	char *mem_bind;		/* binding map for map/mask_cpu */
	uint16_t mem_bind_type;	/* see mem_bind_type_t */
	char *name;		/* name of the job, default "" */
	char *network;		/* network use spec */
	uint16_t nice;		/* requested priority change,
				 * NICE_OFFSET == no change */
	uint32_t num_tasks;	/* number of tasks to be started,
				 * for batch only */
	uint8_t open_mode;	/* out/err open mode truncate or append,
				 * see OPEN_MODE_* */
	uint16_t other_port;	/* port to send various notification msg to */
	uint8_t overcommit;	/* over subscribe resources, for batch only */
	char *partition;	/* name of requested partition,
				 * default in SLURM config */
	uint16_t plane_size;	/* plane size when task_dist =
				   SLURM_DIST_PLANE */
	uint32_t priority;	/* relative priority of the job,
				 * explicitly set only for user root,
				 * 0 == held (don't initiate) */
	char *qos;		/* Quality of Service */
	char *resp_host;	/* NOTE: Set by slurmctld */
	char *req_nodes;	/* comma separated list of required nodes
				 * default NONE */
	uint16_t requeue;	/* enable or disable job requeue option */
	char *reservation;	/* name of reservation to use */
	char *script;		/* the actual job script, default NONE */
	uint16_t shared;	/* 1 if job can share nodes with other jobs,
				 * 0 if job needs exclusive access to the node,
				 * or NO_VAL to accept the system default.
				 * SHARED_FORCE to eliminate user control. */
	char **spank_job_env;	/* environment variables for job prolog/epilog
				 * scripts as set by SPANK plugins */
	uint32_t spank_job_env_size; /* element count in spank_env */
	uint16_t task_dist;	/* see enum task_dist_state */
	uint32_t time_limit;	/* maximum run time in minutes, default is
				 * partition limit */
	uint32_t time_min;	/* minimum run time in minutes, default is
				 * time_limit */
	uint32_t user_id;	/* set only if different from current UID,
				 * can only be explicitly set by user root */
	uint16_t wait_all_nodes;/* 0 to start job immediately after allocation
				 * 1 to start job after all nodes booted
				 * or NO_VAL to use system default */
	uint16_t warn_signal;	/* signal to send when approaching end time */
	uint16_t warn_time;	/* time before end to send signal (seconds) */
	char *work_dir;		/* pathname of working directory */

	/* job constraints: */
	uint16_t cpus_per_task;	/* number of processors required for
				 * each task */
	uint32_t min_cpus;	/* minimum number of processors required,
				 * default=0 */
	uint32_t max_cpus;	/* maximum number of processors required,
				 * default=0 */
	uint32_t min_nodes;	/* minimum number of nodes required by job,
				 * default=0 */
	uint32_t max_nodes;	/* maximum number of nodes usable by job,
				 * default=0 */
	uint16_t boards_per_node; /* boards per node required by job  */
	uint16_t sockets_per_board;/* sockets per board required by job */
	uint16_t sockets_per_node;/* sockets per node required by job */
	uint16_t cores_per_socket;/* cores per socket required by job */
	uint16_t threads_per_core;/* threads per core required by job */
	uint16_t ntasks_per_node;/* number of tasks to invoke on each node */
	uint16_t ntasks_per_socket;/* number of tasks to invoke on
				    * each socket */
	uint16_t ntasks_per_core;/* number of tasks to invoke on each core */
	uint16_t ntasks_per_board;/* number of tasks to invoke on each board */
	uint16_t pn_min_cpus;    /* minimum # CPUs per node, default=0 */
	uint32_t pn_min_memory;  /* minimum real memory per node OR
				  * real memory per CPU | MEM_PER_CPU,
				  * default=0 (no limit) */
	uint32_t pn_min_tmp_disk;/* minimum tmp disk per node,
				  * default=0 */

/*
 * The following parameters are only meaningful on a Blue Gene
 * system at present. Some will be of value on other system. Don't remove these
 * they are needed for LCRM and others that can't talk to the opaque data type
 * select_jobinfo.
 */
	uint16_t geometry[HIGHEST_DIMENSIONS];	/* node count in various
						 * dimensions, e.g. X, Y, and Z */
	uint16_t conn_type[HIGHEST_DIMENSIONS];	/* see enum connection_type */
	uint16_t reboot;	/* force node reboot before startup */
	uint16_t rotate;	/* permit geometry rotation if set */
	char *blrtsimage;       /* BlrtsImage for block */
	char *linuximage;       /* LinuxImage for block */
	char *mloaderimage;     /* MloaderImage for block */
	char *ramdiskimage;     /* RamDiskImage for block */

/* End of Blue Gene specific values */
	uint32_t req_switch;    /* Minimum number of switches */
	dynamic_plugin_data_t *select_jobinfo; /* opaque data type,
					   * SLURM internal use only */
	char *std_err;		/* pathname of stderr */
	char *std_in;		/* pathname of stdin */
	char *std_out;		/* pathname of stdout */
	uint32_t wait4switch;   /* Maximum time to wait for minimum switches */
	char *wckey;            /* wckey for job */
} job_desc_msg_t;

typedef struct job_info {
	char *account;		/* charge to specified account */
	char    *alloc_node;	/* local node making resource alloc */
	uint32_t alloc_sid;	/* local sid making resource alloc */
	uint32_t assoc_id;	/* association id for job */
	uint16_t batch_flag;	/* 1 if batch: queued job with script */
	char *batch_host;	/* name of host running batch script */
	char *batch_script;	/* contents of batch script */
	char *command;		/* command to be executed, built from submitted
				 * job's argv and NULL for salloc command */
	char *comment;		/* arbitrary comment (used by Moab scheduler) */
	uint16_t contiguous;	/* 1 if job requires contiguous nodes */
	uint16_t cpus_per_task;	/* number of processors required for
				 * each task */
	char *dependency;	/* synchronize job execution with other jobs */
	uint32_t derived_ec;	/* highest exit code of all job steps */
	time_t eligible_time;	/* time job is eligible for running */
	time_t end_time;	/* time of termination, actual or expected */
	char *exc_nodes;	/* comma separated list of excluded nodes */
	int *exc_node_inx;	/* excluded list index pairs into node_table:
				 * start_range_1, end_range_1,
				 * start_range_2, .., -1  */
	uint32_t exit_code;	/* exit code for job (status from wait call) */
	char *features;		/* comma separated list of required features */
	char *gres;		/* comma separated list of generic resources */
	uint32_t group_id;	/* group job sumitted as */
	uint32_t job_id;	/* job ID */
	uint16_t job_state;	/* state of the job, see enum job_states */
	char *licenses;		/* licenses required by the job */
	uint32_t max_cpus;	/* maximum number of cpus usable by job */
	uint32_t max_nodes;	/* maximum number of nodes usable by job */
	uint16_t boards_per_node;  /* boards per node required by job   */
	uint16_t sockets_per_board;/* sockets per board required by job */
	uint16_t sockets_per_node; /* sockets per node required by job  */
	uint16_t cores_per_socket; /* cores per socket required by job  */
	uint16_t threads_per_core; /* threads per core required by job  */
	char *name;		/* name of the job */
	char *network;		/* network specification */
	char *nodes;		/* list of nodes allocated to job */
	uint16_t nice;	  	/* requested priority change */
	int *node_inx;		/* list index pairs into node_table for *nodes:
				 * start_range_1, end_range_1,
				 * start_range_2, .., -1  */
	uint16_t ntasks_per_core;/* number of tasks to invoke on each core */
	uint16_t ntasks_per_node;/* number of tasks to invoke on each node */
	uint16_t ntasks_per_socket;/* number of tasks to invoke on each socket*/
	uint16_t ntasks_per_board; /* number of tasks to invoke on each board */

	uint32_t num_nodes;	/* minimum number of nodes required by job */
	uint32_t num_cpus;	/* minimum number of cpus required by job */
	char *partition;	/* name of assigned partition */
	uint32_t pn_min_memory; /* minimum real memory per node, default=0 */
	uint16_t pn_min_cpus;   /* minimum # CPUs per node, default=0 */
	uint32_t pn_min_tmp_disk; /* minimum tmp disk per node, default=0 */
	time_t pre_sus_time;	/* time job ran prior to last suspend */
	uint32_t priority;	/* relative priority of the job,
				 * 0=held, 1=required nodes DOWN/DRAINED */
	char *qos;		/* Quality of Service */
	char *req_nodes;	/* comma separated list of required nodes */
	int *req_node_inx;	/* required list index pairs into node_table:
				 * start_range_1, end_range_1,
				 * start_range_2, .., -1  */
	uint32_t req_switch;    /* Minimum number of switches */
	uint16_t requeue;       /* enable or disable job requeue option */
	time_t resize_time;	/* time of latest size change */
	uint16_t restart_cnt;	/* count of job restarts */
	char *resv_name;	/* reservation name */
	dynamic_plugin_data_t *select_jobinfo; /* opaque data type,
						* process using
						* slurm_get_select_jobinfo()
						*/
	job_resources_t *job_resrcs; /* opaque data type, job resources */
	uint16_t shared;	/* 1 if job can share nodes with other jobs */
	uint16_t show_flags;	/* conveys level of details requested */
	time_t start_time;	/* time execution begins, actual or expected */
	char *state_desc;	/* optional details for state_reason */
	uint16_t state_reason;	/* reason job still pending or failed, see
				 * slurm.h:enum job_state_reason */
	time_t submit_time;	/* time of job submission */
	time_t suspend_time;	/* time job last suspended or resumed */
	uint32_t time_limit;	/* maximum run time in minutes or INFINITE */
	uint32_t time_min;	/* minimum run time in minutes or INFINITE */
	uint32_t user_id;	/* user the job runs as */
	time_t 	preempt_time;	/* preemption signal time */
	uint32_t wait4switch;   /* Maximum time to wait for minimum switches */
	char *wckey;            /* wckey for job */
	char *work_dir;		/* pathname of working directory */
} slurm_job_info_t;

#ifndef __PERMAPI__H__
/* If on an IBM PERCS machine the libpermapi.so file we generate in
 * the switch/nrt plugin will need to link to this file and the
 * permapi.h file which both have a job_info_t structure defined in
 * it.  So to avoid conflict only define job_info_t if we haven't
 * included that file first.  Then when in the libpermapi environment
 * we can use slurm_job_info_t to access the job_info_t data.  If not
 * in that environment we should never know anything changed.
 */
typedef slurm_job_info_t job_info_t;
#endif

typedef struct job_info_msg {
	time_t last_update;	/* time of latest info */
	uint32_t record_count;	/* number of records */
	slurm_job_info_t *job_array;	/* the job records */
} job_info_msg_t;

typedef struct step_update_request_msg {
	uint32_t job_id;
	uint32_t step_id;
	uint32_t time_limit;	/* In minutes */
} step_update_request_msg_t;

typedef struct slurm_step_layout {
	char *front_end;	/* If a front-end architecture, the name of
				 * of the node running all tasks,
				 * NULL otherwise */
	uint32_t node_cnt;	/* node count */
	char *node_list;        /* list of nodes in step */
	uint16_t plane_size;	/* plane size when task_dist =
				 * SLURM_DIST_PLANE */
	/* Array of length "node_cnt". Each element of the array
	 * is the number of tasks assigned to the corresponding node */
	uint16_t *tasks;
	uint32_t task_cnt;	/* total number of tasks in the step */
	uint16_t task_dist;	/* see enum task_dist_state */
	/* Array (of length "node_cnt") of task ID arrays.  The length
	 * of each subarray is designated by the corresponding value in
	 * the tasks array. */
	uint32_t **tids;	/* host id => task id mapping */
} slurm_step_layout_t;

typedef struct slurm_step_io_fds {
	struct {
		int fd;
		uint32_t taskid;
		uint32_t nodeid;
	} in, out, err;
} slurm_step_io_fds_t;

#define SLURM_STEP_IO_FDS_INITIALIZER {{0, (uint32_t)-1, (uint32_t)-1},	\
		{1, (uint32_t)-1, (uint32_t)-1},			\
		{2, (uint32_t)-1, (uint32_t)-1}}

typedef struct launch_tasks_response_msg {
	uint32_t return_code;
	char    *node_name;
	uint32_t srun_node_id;
	uint32_t count_of_pids;
	uint32_t *local_pids;
	uint32_t *task_ids; /* array of length count_of_pids */
} launch_tasks_response_msg_t;

typedef struct task_ext_msg {
	uint32_t num_tasks;
	uint32_t *task_id_list;
	uint32_t return_code;
	uint32_t job_id;
	uint32_t step_id;
} task_exit_msg_t;

typedef struct srun_ping_msg {
	uint32_t job_id;	/* slurm job_id */
	uint32_t step_id;	/* step_id or NO_VAL */
} srun_ping_msg_t;

typedef struct srun_job_complete_msg {
	uint32_t job_id;        /* slurm job_id */
	uint32_t step_id;       /* step_id or NO_VAL */
} srun_job_complete_msg_t;

typedef struct srun_timeout_msg {
	uint32_t job_id;	/* slurm job_id */
	uint32_t step_id;	/* step_id or NO_VAL */
	time_t   timeout;	/* when job scheduled to be killed */
} srun_timeout_msg_t;

typedef struct srun_user_msg {
	uint32_t job_id;	/* slurm job_id */
	char *msg;		/* message to user's srun */
} srun_user_msg_t;

typedef struct srun_node_fail_msg {
	uint32_t job_id;	/* slurm job_id */
	char *nodelist;		/* name of failed node(s) */
	uint32_t step_id;	/* step_id or NO_VAL */
} srun_node_fail_msg_t;

typedef struct srun_step_missing_msg {
	uint32_t job_id;	/* slurm job_id */
	char *nodelist;		/* name of node(s) lacking this step */
	uint32_t step_id;	/* step_id or NO_VAL */
} srun_step_missing_msg_t;

enum suspend_opts {
	SUSPEND_JOB,		/* Suspend a job now */
	RESUME_JOB		/* Resume a job now */
};

typedef struct suspend_msg {
	uint16_t op;            /* suspend operation, see enum suspend_opts */
	uint32_t job_id;        /* slurm job_id */
} suspend_msg_t;

typedef struct {
	uint16_t ckpt_interval;	/* checkpoint interval in minutes */
	uint32_t cpu_count;	/* number of required processors */
	uint32_t cpu_freq;	/* requested cpu frequency */
	uint16_t exclusive;	/* 1 if CPUs not shared with other steps */
	char *features;		/* required node features, default NONE */
	uint16_t immediate;	/* 1 if allocate to run or fail immediately,
				 * 0 if to be queued awaiting resources */
	uint32_t job_id;	/* job ID */
	uint32_t mem_per_cpu;	/* memory required per CPU (MB),
				 * use job limit if 0 */
	char *ckpt_dir;		/* directory to store checkpoint image files */
	char *gres;		/* generic resources needed */
	char *name;		/* name of the job step */
	char *network;		/* network use spec */
	uint8_t no_kill;	/* 1 if no kill on node failure */
	uint32_t min_nodes;	/* minimum number of nodes required by job,
				 * default=0 */
	uint32_t max_nodes;	/* maximum number of nodes usable by job,
				 * default=0 */
	char *node_list;	/* list of required nodes */
	bool overcommit;	/* "true" to allow the allocation of more tasks
				 * to a node than available processors,
				 * "false" to accept at most one task per
				 * processor. "false" by default. */
	uint16_t plane_size;	/* plane size when task_dist =
				 * SLURM_DIST_PLANE */
	uint16_t relative;	/* first node to use of job's allocation */
	uint16_t resv_port_cnt;	/* reserve ports if set */
	uint32_t task_count;	/* number of tasks required */
	uint16_t task_dist;	/* see enum task_dist_state, default
				 * is SLURM_DIST_CYCLIC */
	uint32_t time_limit;	/* step time limit */
	uid_t uid;		/* user ID */
	uint16_t verbose_level; /* for extra logging decisions in step
				 * launch api */
} slurm_step_ctx_params_t;

typedef struct {
	char *alias_list;	/* node name/address/hostnamne aliases */
	uint32_t argc;
	char **argv;
	uint32_t envc;
	char **env;
	char *cwd;
	bool user_managed_io;
	uint32_t msg_timeout; /* timeout set for sending message */

	/* START - only used if user_managed_io is false */
	bool buffered_stdio;
	bool labelio;
	char *remote_output_filename;
	char *remote_error_filename;
	char *remote_input_filename;
	slurm_step_io_fds_t local_fds;
	/*  END  - only used if user_managed_io is false */

	uint32_t gid;
	bool multi_prog;
	uint32_t slurmd_debug;  /* remote slurmd debug level */
	bool parallel_debug;
	char *task_prolog;
	char *task_epilog;
	uint16_t cpu_bind_type;	/* use cpu_bind_type_t */
	char *cpu_bind;
	uint32_t cpu_freq;      /* requested cpu frequency */
	uint16_t mem_bind_type;	/* use mem_bind_type_t */
	char *mem_bind;

	uint16_t max_sockets;
	uint16_t max_cores;
	uint16_t max_threads;
	uint16_t cpus_per_task;
	uint16_t task_dist;
	bool preserve_env;

	char *mpi_plugin_name;
	uint8_t open_mode;
	uint16_t acctg_freq;
	bool pty;
	char *ckpt_dir;
	char *restart_dir;
	char **spank_job_env;	/* environment variables for job prolog/epilog
				 * scripts as set by SPANK plugins */
	uint32_t spank_job_env_size;	/* element count in spank_env */
} slurm_step_launch_params_t;

typedef struct {
	void (*step_complete)(srun_job_complete_msg_t *);
	void (*step_signal)(int);
	void (*step_timeout)(srun_timeout_msg_t *);
	void (*task_start)(launch_tasks_response_msg_t *);
	void (*task_finish)(task_exit_msg_t *);
} slurm_step_launch_callbacks_t;

typedef struct {
	void (*ping)(srun_ping_msg_t *);
	void (*job_complete)(srun_job_complete_msg_t *);
	void (*timeout)(srun_timeout_msg_t *);
	void (*user_msg)(srun_user_msg_t *);
	void (*node_fail)(srun_node_fail_msg_t *);
	void (*job_suspend)(suspend_msg_t *);
} slurm_allocation_callbacks_t;

typedef struct {
	void (*acct_full)();
	void (*dbd_fail)();
	void (*dbd_resumed)();
	void (*db_fail)();
	void (*db_resumed)();
} slurm_trigger_callbacks_t;

typedef struct {
	char *ckpt_dir;		/* path to store checkpoint image files */
	uint16_t ckpt_interval;	/* checkpoint interval in minutes */
	char *gres;		/* generic resources required */
	uint32_t job_id;	/* job ID */
	char *name;		/* name of job step */
	char *network;		/* network specs for job step */
	char *nodes;		/* list of nodes allocated to job_step */
	int *node_inx;		/* list index pairs into node_table for *nodes:
				 * start_range_1, end_range_1,
				 * start_range_2, .., -1  */
	uint32_t num_cpus;	/* how many cpus are being used by step */
	uint32_t cpu_freq;	/* requested cpu frequency */
	uint32_t num_tasks;	/* number of tasks */
	char *partition;	/* name of assigned partition */
	char *resv_ports;	/* ports allocated for MPI */
	time_t run_time;	/* net run time (factor out time suspended) */
	dynamic_plugin_data_t *select_jobinfo; /* opaque data type,
						* process using
						* slurm_get_select_jobinfo()
						*/
	time_t start_time;	/* step start time */
	uint32_t step_id;	/* step ID */
	uint32_t time_limit;	/* step time limit */
	uint32_t user_id;	/* user the job runs as */
} job_step_info_t;

typedef struct job_step_info_response_msg {
	time_t last_update;		/* time of latest info */
	uint32_t job_step_count;	/* number of records */
	job_step_info_t *job_steps;	/* the job step records */
} job_step_info_response_msg_t;

typedef struct {
   	char *node_name;
        uint32_t *pid;
	uint32_t pid_cnt;
} job_step_pids_t;

typedef struct {
	uint32_t job_id;
	List pid_list; /* List of job_step_pids_t *'s */
	uint32_t step_id;
} job_step_pids_response_msg_t;

typedef struct {
	jobacctinfo_t *jobacct;
	uint32_t num_tasks;
   	uint32_t return_code;
	job_step_pids_t *step_pids;
} job_step_stat_t;

typedef struct {
	uint32_t job_id;	/* job ID */
	List stats_list;	/* List of job_step_stat_t *'s */
	uint32_t step_id;
} job_step_stat_response_msg_t;

typedef struct node_info {
	char *arch;		/* computer architecture */
	uint16_t boards;        /* total number of boards per node  */
	time_t boot_time;	/* time of node boot */
	uint16_t cores;         /* number of cores per socket       */
	uint16_t cpus;		/* configured count of cpus running on
				 * the node */
	char *features;		/* list of a node's features */
	char *gres;		/* list of a node's generic resources */
	uint32_t cpu_load;	/* CPU load * 100 */
	char *name;		/* node name to slurm */
	char *node_addr;	/* communication name (optional) */
	char *node_hostname;	/* node's hostname (optional) */
	uint16_t node_state;	/* see enum node_states */
	char *os;		/* operating system currently running */
	uint32_t real_memory;	/* configured MB of real memory on the node */
	char *reason;		/* reason for node being DOWN or DRAINING */
	time_t reason_time;	/* Time stamp when reason was set, ignore if
				 * no reason is set. */
	uint32_t reason_uid;   	/* User that set the reason, ignore if
				 * no reason is set. */
	time_t slurmd_start_time;/* time of slurmd startup */
	uint16_t sockets;       /* total number of sockets per node */
	uint16_t threads;       /* number of threads per core */
	uint32_t tmp_disk;	/* configured MB of total disk in TMP_FS */
	uint32_t weight;	/* arbitrary priority of node for scheduling */
	acct_gather_energy_t *energy;
	dynamic_plugin_data_t *select_nodeinfo;  /* opaque data structure,
						  * use
						  * slurm_get_select_nodeinfo()
						  * to access contents */
} node_info_t;

typedef struct node_info_msg {
	time_t last_update;		/* time of latest info */
	uint32_t node_scaling;          /* the amount of nodes per
					   each record used for
					   systems like bluegene
					   where SLURM handles a
					   bluegene midplane as a
					   single SLURM node. */
	uint32_t record_count;		/* number of records */
	node_info_t *node_array;	/* the node records */
} node_info_msg_t;

typedef struct front_end_info {
	time_t boot_time;		/* Time of node boot,
					 * computed from up_time */
	char *name;			/* node name */
	uint16_t node_state;		/* see enum node_states */
	char *reason;			/* reason for node being DOWN or
					 * DRAINING */
	time_t reason_time;		/* Time stamp when reason was set,
					 * ignore if no reason is set. */
	uint32_t reason_uid;   		/* User that set the reason,
					 * ignore if no reason is set. */
	time_t slurmd_start_time;	/* Time of slurmd startup */
} front_end_info_t;

typedef struct front_end_info_msg {
	time_t last_update;		/* time of latest info */
	uint32_t record_count;		/* number of records */
	front_end_info_t *front_end_array;	/* the front_end records */
} front_end_info_msg_t;

typedef struct topo_info {
	uint16_t level;			/* level in hierarchy, leaf=0 */
	uint32_t link_speed;		/* link speed, arbitrary units */
	char *name;			/* switch name */
	char *nodes;			/* name if direct descendent nodes */
	char *switches;			/* name if direct descendent switches */
} topo_info_t;

typedef struct topo_info_response_msg {
	uint32_t record_count;		/* number of records */
	topo_info_t *topo_array;	/* the switch topology records */
} topo_info_response_msg_t;

typedef struct job_alloc_info_msg {
	uint32_t job_id;	/* job ID */
} job_alloc_info_msg_t;

typedef struct acct_gather_node_resp_msg {
	char *node_name;	  /* node name */
	acct_gather_energy_t *energy;
} acct_gather_node_resp_msg_t;

/* Current partition state information and used to set partition options
 * using slurm_update_partition(). */
#define PART_FLAG_DEFAULT	0x0001	/* Set if default partition */
#define PART_FLAG_HIDDEN	0x0002	/* Set if partition is hidden */
#define PART_FLAG_NO_ROOT	0x0004	/* Set if user root jobs disabled */
#define PART_FLAG_ROOT_ONLY	0x0008	/* Set if only root can submit jobs */
#define PART_FLAG_REQ_RESV	0x0010	/* Set if reservation is required */
/* Used with slurm_update_partition() to clear flags associated with existing
 * partitions. For example, if a partition is currently hidden and you want
 * to make it visible then set flags to PART_FLAG_HIDDEN_CLR and call
 * slurm_update_partition(). */
#define PART_FLAG_DEFAULT_CLR	0x0100	/* Clear DEFAULT partition flag */
#define PART_FLAG_HIDDEN_CLR	0x0200	/* Clear HIDDEN partition flag */
#define PART_FLAG_NO_ROOT_CLR	0x0400	/* Clear NO_ROOT partition flag */
#define PART_FLAG_ROOT_ONLY_CLR	0x0800	/* Clear ROOT_ONLY partition flag */
#define PART_FLAG_REQ_RESV_CLR	0x1000	/* Clear RES_REQ partition flag */

typedef struct partition_info {
	char *allow_alloc_nodes;/* list names of allowed allocating nodes */
	char *allow_groups;	/* comma delimited list of groups,
				 * null indicates all */
	char *alternate; 	/* name of alternate partition */
	uint32_t def_mem_per_cpu; /* default MB memory per allocated CPU */
	uint32_t default_time;	/* minutes, NO_VAL or INFINITE */
	uint16_t flags;		/* see PART_FLAG_* above */
	uint32_t grace_time; 	/* preemption grace time in seconds */
	uint32_t max_mem_per_cpu; /* maximum MB memory per allocated CPU */
	uint32_t max_nodes;	/* per job or INFINITE */
	uint16_t max_share;	/* number of jobs to gang schedule */
	uint32_t max_time;	/* minutes or INFINITE */
	uint32_t min_nodes;	/* per job */
	char *name;		/* name of the partition */
	int *node_inx;		/* list index pairs into node_table:
				 * start_range_1, end_range_1,
				 * start_range_2, .., -1  */
	char *nodes;		/* list names of nodes in partition */
	uint16_t preempt_mode;	/* See PREEMPT_MODE_* in slurm/slurm.h */
	uint16_t priority;	/* scheduling priority for jobs */
	uint16_t state_up;	/* see PARTITION_ states above */
	uint32_t total_cpus;	/* total number of cpus in the partition */
	uint32_t total_nodes;	/* total number of nodes in the partition */
} partition_info_t;

typedef struct delete_partition_msg {
	char *name;		/* name of partition to be delete */
} delete_part_msg_t;

typedef struct resource_allocation_response_msg {
	uint32_t job_id;	/* assigned job id */
	char *node_list;	/* assigned list of nodes */
	char *alias_list;	/* node name/address/hostnamne aliases */
	uint32_t num_cpu_groups;/* elements in below cpu arrays */
	uint16_t *cpus_per_node;/* cpus per node */
	uint32_t *cpu_count_reps;/* how many nodes have same cpu count */
	uint32_t node_cnt;	/* count of nodes */
	uint32_t error_code;	/* error code for warning message */
	dynamic_plugin_data_t *select_jobinfo;	/* opaque data structure,
						 * use
						 * slurm_get_select_jobinfo()
						 * to access contents */
	uint32_t pn_min_memory;  /* minimum real memory per node OR
				  * real memory per CPU | MEM_PER_CPU,
				  * default=0 (no limit) */
} resource_allocation_response_msg_t;

typedef struct job_alloc_info_response_msg {
	uint32_t job_id;	/* assigned job id */
	char *node_list;	/* assigned list of nodes */
	uint32_t num_cpu_groups;/* elements in below cpu arrays */
	uint16_t *cpus_per_node;/* cpus per node */
	uint32_t *cpu_count_reps;/* how many nodes have same cpu count */
	uint32_t node_cnt;	/* count of nodes */
	slurm_addr_t *node_addr;	/* network addresses */
	uint32_t error_code;	/* error code for warning message */
	dynamic_plugin_data_t *select_jobinfo;	/* opaque data structure,
						 * use
						 * slurm_get_select_jobinfo()
						 * to access contents */
} job_alloc_info_response_msg_t;

typedef struct partition_info_msg {
	time_t last_update;	/* time of latest info */
	uint32_t record_count;	/* number of records */
	partition_info_t *partition_array; /* the partition records */
} partition_info_msg_t;


/* BLUEGENE specific information */

typedef struct {
	char *cnodes;           /* used for sub-block jobs */
	int *cnode_inx;         /* list index pairs for cnodes in the
				 * node listed for *cnodes:
				 * start_range_1, end_range_1,
				 * start_range_2, .., -1  used for
				 * sub-block jobs */
	uint32_t job_id;        /* job id */
	void *job_ptr;          /* internal use only, not packed. */
	uint32_t user_id;       /* user running the job's ID */
	char *user_name;        /* user running the job's name */
} block_job_info_t;

typedef struct {
	char *bg_block_id;
	char *blrtsimage;       /* BlrtsImage for this block */
	uint16_t conn_type[HIGHEST_DIMENSIONS];
	uint32_t cnode_cnt;
	uint32_t cnode_err_cnt;
	int *ionode_inx;        /* list index pairs for ionodes in the
				 * node listed for *ionode_str:
				 * start_range_1, end_range_1,
				 * start_range_2, .., -1  */
	char *ionode_str;
	List job_list;          /* List of running jobs on each block */
	char *linuximage;       /* LinuxImage for this block */
	char *mloaderimage;     /* mloaderImage for this block */
	int *mp_inx;            /* list index pairs into node_table for *mp_str:
				 * start_range_1, end_range_1,
				 * start_range_2, .., -1  */
	char *mp_str;
	uint16_t node_use;
	char *ramdiskimage;     /* RamDiskImage for this block */
	char *reason;
	uint16_t state;
} block_info_t;

typedef struct {
	block_info_t *block_array;
	time_t    last_update;
	uint32_t  record_count;
} block_info_msg_t;

typedef block_info_t update_block_msg_t;

/*
 * slurm_print_block_info_msg - output information about all Bluegene
 *	blocks based upon message as loaded using slurm_load_block
 * IN out - file to write to
 * IN info_ptr - block information message pointer
 * IN one_liner - print as a single line if true
 */
extern void slurm_print_block_info_msg PARAMS(
	(FILE *out, block_info_msg_t *info_ptr, int one_liner));

/*
 * slurm_print_block_info - output information about a specific Bluegene
 *	block based upon message as loaded using slurm_load_block
 * IN out - file to write to
 * IN bg_info_ptr - an individual block information record pointer
 * IN one_liner - print as a single line if true
 */
extern void slurm_print_block_info PARAMS(
	(FILE *out, block_info_t *bg_info_ptr, int one_liner));

/*
 * slurm_sprint_block_info - output information about a specific Bluegene
 *	block based upon message as loaded using slurm_load_block
 * IN bg_info_ptr - an individual partition information record pointer
 * IN one_liner - print as a single line if true
 * RET out - char * containing formatted output (must be freed after call)
 *           NULL is returned on failure.
 */
extern char *slurm_sprint_block_info PARAMS(
	(block_info_t * bg_info_ptr, int one_liner));

/*
 * slurm_load_block_info - issue RPC to get slurm all node select plugin
 *	information if changed since update_time
 * IN update_time - time of current configuration data
 * IN block_info_msg_pptr - place to store a node select configuration
 *	pointer
 * IN show_flags - controls output form or filtering, see SHOW_FLAGS in slurm.h
 * RET 0 or a slurm error code
 * NOTE: free the response using slurm_free_block_info_msg
 */
extern int slurm_load_block_info PARAMS(
	(time_t update_time, block_info_msg_t **block_info_msg_pptr,
	 uint16_t show_flags));

/*
 * slurm_free_block_info_msg - free buffer returned by
 *	slurm_load_block
 * IN block_info_msg_ptr - pointer to block_info_msg_t
 * RET 0 or a slurm error code
 */
extern void slurm_free_block_info_msg PARAMS(
	(block_info_msg_t *block_info_msg));

/* update state or remove block */
extern int slurm_update_block PARAMS((update_block_msg_t *block_msg));

void slurm_init_update_block_msg PARAMS((update_block_msg_t *update_block_msg));

/*********************************/

/*
 * Resource reservation data structures.
 * Create, show, modify and delete functions are required
 */
#define RESERVE_FLAG_MAINT	0x0001	/* Set MAINT flag */
#define RESERVE_FLAG_NO_MAINT	0x0002	/* Clear MAINT flag */
#define RESERVE_FLAG_DAILY	0x0004	/* Set DAILY flag */
#define RESERVE_FLAG_NO_DAILY	0x0008	/* Clear DAILY flag */
#define RESERVE_FLAG_WEEKLY	0x0010	/* Set WEEKLY flag */
#define RESERVE_FLAG_NO_WEEKLY	0x0020	/* Clear WEEKLY flag */
#define RESERVE_FLAG_IGN_JOBS	0x0040	/* Ignore running jobs */
#define RESERVE_FLAG_NO_IGN_JOB	0x0080	/* Clear ignore running jobs */
#define RESERVE_FLAG_LIC_ONLY	0x0100	/* Reserve licenses only, any nodes */
#define RESERVE_FLAG_NO_LIC_ONLY 0x0200	/* Clear reserve licenses only flag */
#define RESERVE_FLAG_STATIC     0x0400 /* Static node allocation */
#define RESERVE_FLAG_NO_STATIC  0x0800 /* Clear static node allocation */
#define RESERVE_FLAG_PART_NODES 0x1000	/* Use Partition Nodes only */
#define RESERVE_FLAG_NO_PART_NODES 0x2000 /* Clear Partition Nodes only */
#define RESERVE_FLAG_OVERLAP	0x4000	/* Permit to overlap others */
#define RESERVE_FLAG_SPEC_NODES	0x8000	/* Contains specific nodes */

typedef struct reserve_info {
	char *accounts;		/* names of accounts permitted to use */
	time_t end_time;	/* end time of reservation */
	char *features;		/* required node features */
	uint16_t flags;		/* see RESERVE_FLAG_* above */
	char *licenses;		/* names of licenses to be reserved */
	char *name;		/* name of reservation */
	uint32_t node_cnt;	/* count of nodes required */
	uint32_t core_cnt;	/* count of cores required */
	int *node_inx;		/* list index pairs into node_table for *nodes:
				 * start_range_1, end_range_1,
				 * start_range_2, .., -1  */
	char *node_list;	/* list of reserved nodes or ALL */
	char *partition;	/* name of partition to be used */
	time_t start_time;	/* start time of reservation */
	char *users;		/* names of users permitted to use */
} reserve_info_t;

typedef struct reserve_info_msg {
	time_t last_update;	/* time of latest info */
	uint32_t record_count;	/* number of records */
	reserve_info_t *reservation_array; /* the reservation records */
} reserve_info_msg_t;

typedef struct resv_desc_msg {
	char *accounts;		/* names of accounts permitted to use */
	uint32_t duration;	/* duration of reservation in seconds */
	time_t end_time;	/* end time of reservation */
	char *features;		/* required node features */
	uint16_t flags;		/* see RESERVE_FLAG_* above */
	char *licenses;		/* names of licenses to be reserved */
	char *name;		/* name of reservation (optional on create) */
	uint32_t core_cnt;	/* Count of cores required */
	uint32_t *node_cnt;	/* Count of nodes required. Specify set of job
				 * sizes with trailing zero to optimize layout
				 * for those jobs just specify their total size
				 * to ignore optimized topology. For example,
				 * {512,512,1024,0} OR {2048,0}. */
	char *node_list;	/* list of reserved nodes or ALL */
	char *partition;	/* name of partition to be used */
	time_t start_time;	/* start time of reservation */
	char *users;		/* names of users permitted to use */
} resv_desc_msg_t;

typedef struct reserve_response_msg {
	char *name;		/* name of reservation */
} reserve_response_msg_t;

typedef struct reservation_name_msg {
	char *name;		/* name of reservation just created or
				 * to be delete */
} reservation_name_msg_t;


#define DEBUG_FLAG_SELECT_TYPE	0x00000001	/* SelectType plugin */
#define DEBUG_FLAG_STEPS	0x00000002	/* slurmctld steps */
#define DEBUG_FLAG_TRIGGERS	0x00000004	/* slurmctld triggers */
#define DEBUG_FLAG_CPU_BIND	0x00000008	/* CPU binding */
#define DEBUG_FLAG_WIKI		0x00000010	/* wiki communications */
#define DEBUG_FLAG_NO_CONF_HASH	0x00000020	/* no warning about slurm.conf
						 * files checksum mismatch */
#define DEBUG_FLAG_GRES		0x00000040	/* Generic Resource info */
#define DEBUG_FLAG_BG_PICK      0x00000080	/* debug for bluegene
						 * picking blocks */
#define DEBUG_FLAG_BG_WIRES	0x00000100	/* debug for bluegene wires */
#define DEBUG_FLAG_BG_ALGO	0x00000200	/* debug for bluegene algo */
#define DEBUG_FLAG_BG_ALGO_DEEP	0x00000400	/* debug for bluegene
						 * algo deep */
#define DEBUG_FLAG_PRIO 	0x00000800	/* debug for priority plugin */
#define DEBUG_FLAG_BACKFILL	0x00001000	/* debug for sched/backfill */
#define DEBUG_FLAG_GANG		0x00002000	/* debug gang scheduler */
#define DEBUG_FLAG_RESERVATION	0x00004000	/* advanced reservations */
#define DEBUG_FLAG_FRONT_END	0x00008000	/* front-end nodes */
#define DEBUG_FLAG_NO_REALTIME	0x00010000	/* get debug when the
						 * realtime server
						 * dies on a bluegene system */
#define DEBUG_FLAG_SWITCH	0x00020000	/* SwitchType plugin */
#define DEBUG_FLAG_ENERGY	0x00040000	/* AcctGatherEnergy plugin */

#define GROUP_FORCE		0x8000	/* if set, update group membership
					 * info even if no updates to
					 * /etc/group file */
#define GROUP_CACHE		0x4000	/* if set, cache /etc/groups to avoid
					 * initgroups(2) */
#define GROUP_TIME_MASK		0x0fff	/* group membership update interval
					 * in seconds */

#define PREEMPT_MODE_OFF	0x0000	/* disable job preemption */
#define PREEMPT_MODE_SUSPEND	0x0001	/* suspend jobs to preempt */
#define PREEMPT_MODE_REQUEUE	0x0002	/* requeue or kill jobs to preempt */
#define PREEMPT_MODE_CHECKPOINT	0x0004	/* checkpoint job to preempt,
					 * no automatic restart */
#define PREEMPT_MODE_CANCEL	0x0008	/* always cancel the job */
#define PREEMPT_MODE_GANG	0x8000	/* enable gang scheduling */

#define RECONFIG_KEEP_PART_INFO 0x0001 /* keep dynamic partition info on scontrol reconfig */
#define RECONFIG_KEEP_PART_STAT 0x0002 /* keep dynamic partition state on scontrol reconfig */

typedef struct slurm_ctl_conf {
	time_t last_update;	/* last update time of the build parameters */
	uint16_t accounting_storage_enforce; /* job requires valid association:
					      * user/account/partition/cluster */
	char *accounting_storage_backup_host;	/* accounting storage
						 * backup host */
	char *accounting_storage_host;	/* accounting storage host */
	char *accounting_storage_loc;	/* accounting storage (db table)
					 * location */
	char *accounting_storage_pass;	/* accounting storage
					 * password */
	uint32_t accounting_storage_port;/* node accountinging storage port */
	char *accounting_storage_type; /* accounting storage type */
	char *accounting_storage_user; /* accounting storage user */
	uint16_t acctng_store_job_comment; /* send job comment to accounting */
	char *acct_gather_energy_type; /* energy accounting type */
	uint16_t acct_gather_node_freq; /* secs between node acct request */
	char *authtype;		/* authentication type */
	char *backup_addr;	/* comm path of slurmctld secondary server */
	char *backup_controller;/* name of slurmctld secondary server */
	uint16_t batch_start_timeout;	/* max secs for batch job to start */
	time_t boot_time;	/* time slurmctld last booted */
	char *checkpoint_type;	/* checkpoint plugin type */
	char *cluster_name;     /* general name of the entire cluster */
	uint16_t complete_wait;	/* seconds to wait for job completion before
				 * scheduling another job */
	char *control_addr;	/* comm path of slurmctld primary server */
	char *control_machine;	/* name of slurmctld primary server */
	char *crypto_type;	/* cryptographic signature plugin */
	uint32_t debug_flags;	/* see DEBUG_FLAG_* above for values */
	uint32_t def_mem_per_cpu; /* default MB memory per allocated CPU */
	uint16_t disable_root_jobs; /* if set then user root can't run jobs */
	uint16_t enforce_part_limits;	/* if set, reject job exceeding
					 * partition size and/or time limits */
	char *epilog;		/* pathname of job epilog */
	uint32_t epilog_msg_time;  /* usecs for slurmctld to process an
				    * epilog complete message */
	char *epilog_slurmctld;	/* pathname of job epilog run by slurmctld */
	uint16_t fast_schedule;	/* 1 to *not* check configurations by node
				 * (only check configuration file, faster) */
	uint32_t first_job_id;	/* first slurm generated job_id to assign */
	uint16_t get_env_timeout; /* timeout for srun --get-user-env option */
	char * gres_plugins;	/* list of generic resource plugins */
	uint16_t group_info;	/* see GROUP_* fields above */
	uint32_t hash_val;      /* Hash value of the slurm.conf file */
	uint16_t health_check_interval;	/* secs between health checks */
	char * health_check_program;	/* pathname of health check program */
	uint16_t inactive_limit;/* seconds of inactivity before a
				 * inactive resource allocation is released */
	uint16_t job_acct_gather_freq; /* poll frequency for job accounting
					* gather plugins */
	char *job_acct_gather_type; /* job accounting gather type */
	char *job_ckpt_dir;	/* directory saving job record checkpoint */
	char *job_comp_host;	/* job completion logging host */
	char *job_comp_loc;	/* job completion logging location */
	char *job_comp_pass;	/* job completion storage password */
	uint32_t job_comp_port;	/* job completion storage port */
	char *job_comp_type;	/* job completion storage type */
	char *job_comp_user;	/* job completion storage user */
	char *job_credential_private_key;	/* path to private key */
	char *job_credential_public_certificate;/* path to public certificate*/
	uint16_t job_file_append; /* if set, append to stdout/err file */
	uint16_t job_requeue;	/* If set, jobs get requeued on node failre */
	char *job_submit_plugins;  /* List of job_submit plugins to use */
	uint16_t kill_on_bad_exit; /* If set, the job will be
				    * terminated immediately when one of
				    * the processes is aborted or crashed */
	uint16_t kill_wait;	/* seconds between SIGXCPU to SIGKILL
				 * on job termination */
	char *launch_type;	/* type of step launcher to use */
	char *licenses;		/* licenses available on this cluster */
	char *licenses_used;	/* licenses used on this cluster */
	char *mail_prog;	/* pathname of mail program */
	uint32_t max_job_cnt;	/* maximum number of active jobs */
	uint32_t max_job_id;	/* maximum job id before using first_job_id */
	uint32_t max_mem_per_cpu; /* maximum MB memory per allocated CPU */
	uint32_t max_step_cnt;	/* maximum number of steps per job */
	uint16_t max_tasks_per_node; /* maximum tasks per node */
	uint16_t min_job_age;	/* COMPLETED jobs over this age (secs)
				 * purged from in memory records */
	char *mpi_default;	/* Default version of MPI in use */
	char *mpi_params;	/* MPI parameters */
	uint16_t msg_timeout;	/* message timeout */
	uint32_t next_job_id;	/* next slurm generated job_id to assign */
	char *node_prefix;      /* prefix of nodes in partition, only set in
				   bluegene clusters NULL otherwise */
	uint16_t over_time_limit; /* job's time limit can be exceeded by this
				   * number of minutes before cancellation */
	char *plugindir;	/* pathname to plugins */
	char *plugstack;        /* pathname to plugin stack config
				 * file */
	uint16_t preempt_mode;	/* See PREEMPT_MODE_* in slurm/slurm.h */
	char *preempt_type;	/* job preemption selection plugin */
	uint32_t priority_decay_hl; /* priority decay half life in
				     * seconds */
	uint32_t priority_calc_period; /* seconds between priority decay
					* calculation */
	uint16_t priority_favor_small; /* favor small jobs over large */
	uint16_t priority_flags; /* set some flags for priority configuration,
				  * see PRIORITY_FLAGS_* above */
	uint32_t priority_max_age; /* time when not to add any more
				    * priority to a job if reached */
	uint16_t priority_reset_period; /* when to clear usage,
					 * see PRIORITY_RESET_* */
	char *priority_type;    /* priority type plugin */
	uint32_t priority_weight_age; /* weight for age factor */
	uint32_t priority_weight_fs; /* weight for Fairshare factor */
	uint32_t priority_weight_js; /* weight for Job Size factor */
	uint32_t priority_weight_part; /* weight for Partition factor */
	uint32_t priority_weight_qos; /* weight for QOS factor */
	uint16_t private_data;	/* block viewing of information,
				 * see PRIVATE_DATA_* */
	char *proctrack_type;	/* process tracking plugin type */
	char *prolog;		/* pathname of job prolog run by slurmd */
	char *prolog_slurmctld;	/* pathname of job prolog run by slurmctld */
	uint16_t propagate_prio_process; /* process priority propagation,
					  * see PROP_PRIO_* */
	char *propagate_rlimits;/* Propagate (all/specific) resource limits */
	char *propagate_rlimits_except;/* Propagate all rlimits except these */
	char *reboot_program;	/* program to reboot the node */
	uint16_t reconfig_flags;/* see RECONFIG_* */
	char *resume_program;	/* program to make nodes full power */
	uint16_t resume_rate;	/* nodes to make full power, per minute */
	uint16_t resume_timeout;/* time required in order to perform a node
				 * resume operation */
	uint16_t resv_over_run;	/* how long a running job can exceed
				 * reservation time */
	uint16_t ret2service;	/* 1 return DOWN node to service at
				 * registration */
	char *salloc_default_command; /* default salloc command */
	char *sched_logfile;    /* where slurm Scheduler log gets written */
	uint16_t sched_log_level;  /* configured level of slurm Scheduler log */
	char *sched_params;	/* SchedulerParameters OR
				 * contents of scheduler plugin config file */
	uint16_t sched_time_slice;	/* gang scheduler slice time, secs */
	char *schedtype;	/* type of scheduler to use */
	uint16_t schedport;	/* port for scheduler connection */
	uint16_t schedrootfltr;	/* 1 if rootOnly partitions should be
				 * filtered from scheduling (if needed) */
	char *select_type;	/* type of node selector to use */
	void *select_conf_key_pairs; /* key-pair list which can be
				      * listed with slurm_print_key_pairs */
	uint16_t select_type_param; /* Parameters
				     * describing the select_type plugin */
	char *slurm_conf;	/* pathname of slurm config file */
	uint32_t slurm_user_id;	/* uid of slurm_user_name */
	char *slurm_user_name;	/* user that slurmctld runs as */
	uint32_t slurmd_user_id;/* uid of slurmd_user_name */
	char *slurmd_user_name;	/* user that slurmd runs as */
	uint16_t slurmctld_debug; /* slurmctld logging level */
	char *slurmctld_logfile;/* where slurmctld error log gets written */
	char *slurmctld_pidfile;/* where to put slurmctld pidfile         */
	uint32_t slurmctld_port;  /* default communications port to slurmctld */
	uint16_t slurmctld_port_count; /* number of slurmctld comm ports */
	uint16_t slurmctld_timeout;/* seconds that backup controller waits
				    * on non-responding primarly controller */
	uint16_t slurmd_debug;	/* slurmd logging level */
	char *slurmd_logfile;	/* where slurmd error log gets written */
	char *slurmd_pidfile;   /* where to put slurmd pidfile           */
	uint32_t slurmd_port;	/* default communications port to slurmd */
	char *slurmd_spooldir;	/* where slurmd put temporary state info */
	uint16_t slurmd_timeout;/* how long slurmctld waits for slurmd before
				 * considering node DOWN */
	char *srun_epilog;      /* srun epilog program */
	char *srun_prolog;      /* srun prolog program */
	char *state_save_location;/* pathname of slurmctld state save
				   * directory */
	char *suspend_exc_nodes;/* nodes to not make power saving */
	char *suspend_exc_parts;/* partitions to not make power saving */
	char *suspend_program;	/* program to make nodes power saving */
	uint16_t suspend_rate;	/* nodes to make power saving, per minute */
	uint32_t suspend_time;	/* node idle for this long before power save mode */
	uint16_t suspend_timeout;/* time required in order to perform a node
				  * suspend operation */
	char *switch_type;	/* switch or interconnect type */
	char *task_epilog;	/* pathname of task launch epilog */
	char *task_plugin;	/* task launch plugin */
	uint16_t task_plugin_param;	/* see CPU_BIND_* */
	char *task_prolog;	/* pathname of task launch prolog */
	char *tmp_fs;		/* pathname of temporary file system */
	char *topology_plugin;	/* network topology plugin */
	uint16_t track_wckey;    /* see if we are using wckey or not */
	uint16_t tree_width;    /* number of threads per node to span */
	char *unkillable_program; /* program run by the slurmstepd when
				   * processes in a job step are unkillable */
	uint16_t unkillable_timeout; /* time in seconds, after processes in a
				      * job step have been signalled, before
				      * they are considered "unkillable". */
	uint16_t use_pam;	/* enable/disable PAM support */
	char *version;		/* version of slurmctld */
	uint16_t vsize_factor;	/* virtual memory limit size factor */
	uint16_t wait_time;	/* default job --wait time */
	uint16_t z_16;		/* reserved for future use */
	uint32_t z_32;		/* reserved for future use */
	char *z_char;		/* reserved for future use */
} slurm_ctl_conf_t;

typedef struct slurmd_status_msg {
	time_t booted;			/* when daemon was started */
	time_t last_slurmctld_msg;	/* time of last slurmctld message */
	uint16_t slurmd_debug;		/* logging level */
	uint16_t actual_cpus;		/* actual logical processor count */
	uint16_t actual_boards;  	/* actual total boards count      */
	uint16_t actual_sockets;	/* actual total sockets count     */
	uint16_t actual_cores;		/* actual core per socket count   */
	uint16_t actual_threads;	/* actual thread per core count */
	uint32_t actual_real_mem;	/* actual real memory in MB */
	uint32_t actual_tmp_disk;	/* actual temp disk space in MB */
	uint32_t pid;			/* process ID */
	char *hostname;			/* local hostname */
	char *slurmd_logfile;		/* slurmd log file location */
	char *step_list;		/* list of active job steps */
	char *version;			/* version running */
} slurmd_status_t;

typedef struct submit_response_msg {
	uint32_t job_id;	/* job ID */
	uint32_t step_id;	/* step ID */
	uint32_t error_code;	/* error code for warning message */
} submit_response_msg_t;

/* NOTE: If setting node_addr and/or node_hostname then comma separate names
 * and include an equal number of node_names */
typedef struct slurm_update_node_msg {
	char *features;		/* new feature for node */
	char *gres;		/* new generic resources for node */
	char *node_addr;	/* communication name (optional) */
	char *node_hostname;	/* node's hostname (optional) */
	char *node_names;	/* nodelist expression */
	uint16_t node_state;	/* see enum node_states */
	char *reason;		/* reason for node being DOWN or DRAINING */
	uint32_t reason_uid;	/* user ID of sending (needed if user
				 * root is sending message) */
	uint32_t weight;	/* new weight for node */
} update_node_msg_t;

typedef struct slurm_update_front_end_msg {
	char *name;		/* comma separated list of front end nodes */
	uint16_t node_state;	/* see enum node_states */
	char *reason;		/* reason for node being DOWN or DRAINING */
	uint32_t reason_uid;	/* user ID of sending (needed if user
				 * root is sending message) */
} update_front_end_msg_t;

typedef struct partition_info update_part_msg_t;

typedef struct job_sbcast_cred_msg {
	uint32_t      job_id;		/* assigned job id */
	slurm_addr_t  *node_addr;	/* network addresses */
	uint32_t      node_cnt;		/* count of nodes */
	char         *node_list;	/* assigned list of nodes */
	sbcast_cred_t *sbcast_cred;	/* opaque data structure */
} job_sbcast_cred_msg_t;

/* Opaque data type for slurm_step_ctx_* functions */
typedef struct slurm_step_ctx_struct slurm_step_ctx_t;

#define STAT_COMMAND_RESET	0x0000
#define STAT_COMMAND_GET	0x0001
typedef struct stats_info_request_msg {
	uint16_t command_id;
} stats_info_request_msg_t;

typedef struct stats_info_response_msg {
	uint32_t parts_packed;
	time_t req_time;
	time_t req_time_start;
	uint32_t server_thread_count;
	uint32_t agent_queue_size;

	uint32_t schedule_cycle_max;
	uint32_t schedule_cycle_last;
	uint32_t schedule_cycle_sum;
	uint32_t schedule_cycle_counter;
	uint32_t schedule_cycle_depth;
	uint32_t schedule_queue_len;

	uint32_t jobs_submitted;
	uint32_t jobs_started;
	uint32_t jobs_completed;
	uint32_t jobs_canceled;
	uint32_t jobs_failed;

	uint32_t bf_backfilled_jobs;
	uint32_t bf_last_backfilled_jobs;
	uint32_t bf_cycle_counter;
	uint32_t bf_cycle_sum;
	uint32_t bf_cycle_last;
	uint32_t bf_cycle_max;
	uint32_t bf_last_depth;
	uint32_t bf_last_depth_try;
	uint32_t bf_depth_sum;
	uint32_t bf_depth_try_sum;
	uint32_t bf_queue_len;
	uint32_t bf_queue_len_sum;
	time_t   bf_when_last_cycle;
	uint32_t bf_active;
} stats_info_response_msg_t;

#define TRIGGER_FLAG_PERM		0x0001

#define TRIGGER_RES_TYPE_JOB            0x0001
#define TRIGGER_RES_TYPE_NODE           0x0002
#define TRIGGER_RES_TYPE_SLURMCTLD      0x0003
#define TRIGGER_RES_TYPE_SLURMDBD       0x0004
#define TRIGGER_RES_TYPE_DATABASE       0x0005
#define TRIGGER_RES_TYPE_FRONT_END      0x0006

#define TRIGGER_TYPE_UP                 0x00000001
#define TRIGGER_TYPE_DOWN               0x00000002
#define TRIGGER_TYPE_FAIL               0x00000004
#define TRIGGER_TYPE_TIME               0x00000008
#define TRIGGER_TYPE_FINI               0x00000010
#define TRIGGER_TYPE_RECONFIG           0x00000020
#define TRIGGER_TYPE_BLOCK_ERR          0x00000040
#define TRIGGER_TYPE_IDLE               0x00000080
#define TRIGGER_TYPE_DRAINED            0x00000100
#define TRIGGER_TYPE_PRI_CTLD_FAIL      0x00000200
#define TRIGGER_TYPE_PRI_CTLD_RES_OP    0x00000400
#define TRIGGER_TYPE_PRI_CTLD_RES_CTRL  0x00000800
#define TRIGGER_TYPE_PRI_CTLD_ACCT_FULL 0x00001000
#define TRIGGER_TYPE_BU_CTLD_FAIL       0x00002000
#define TRIGGER_TYPE_BU_CTLD_RES_OP     0x00004000
#define TRIGGER_TYPE_BU_CTLD_AS_CTRL    0x00008000
#define TRIGGER_TYPE_PRI_DBD_FAIL       0x00010000
#define TRIGGER_TYPE_PRI_DBD_RES_OP     0x00020000
#define TRIGGER_TYPE_PRI_DB_FAIL        0x00040000
#define TRIGGER_TYPE_PRI_DB_RES_OP      0x00080000


typedef struct trigger_info {
	uint16_t flags;		/* TRIGGER_FLAG_* */
	uint32_t trig_id;	/* trigger ID */
	uint16_t res_type;	/* TRIGGER_RES_TYPE_* */
	char *   res_id;	/* resource ID */
	uint32_t trig_type;	/* TRIGGER_TYPE_* */
	uint16_t offset;	/* seconds from trigger, 0x8000 origin */
	uint32_t user_id;	/* user requesting trigger */
	char *   program;	/* program to execute */
} trigger_info_t;

typedef struct trigger_info_msg {
	uint32_t record_count;		/* number of records */
	trigger_info_t *trigger_array;	/* the trigger records */
} trigger_info_msg_t;

/*****************************************************************************\
 *	RESOURCE ALLOCATION FUNCTIONS
\*****************************************************************************/

/*
 * slurm_init_job_desc_msg - initialize job descriptor with
 *	default values
 * OUT job_desc_msg - user defined job descriptor
 */
extern void slurm_init_job_desc_msg PARAMS((job_desc_msg_t * job_desc_msg));

/*
 * slurm_allocate_resources - allocate resources for a job request
 *   If the requested resources are not immediately available, the slurmctld
 *   will send the job_alloc_resp_msg to the sepecified node and port.
 * IN job_desc_msg - description of resource allocation request
 * OUT job_alloc_resp_msg - response to request.  This only represents
 *   a job allocation if resources are immediately.  Otherwise it just contains
 *   the job id of the enqueued job request.
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 * NOTE: free the response using slurm_free_resource_allocation_response_msg()
 */
extern int slurm_allocate_resources PARAMS(
	(job_desc_msg_t * job_desc_msg ,
	 resource_allocation_response_msg_t ** job_alloc_resp_msg));

/*
 * slurm_allocate_resources_blocking
 *	allocate resources for a job request.  This call will block until
 *	the allocation is granted, or the specified timeout limit is reached.
 * IN req - description of resource allocation request
 * IN timeout - amount of time, in seconds, to wait for a response before
 * 	giving up.
 *	A timeout of zero will wait indefinitely.
 * IN pending_callback - If the allocation cannot be granted immediately,
 *      the controller will put the job in the PENDING state.  If
 *      pending callback is not NULL, it will be called with the job_id
 *      of the pending job as the sole parameter.
 *
 * RET allocation structure on success, NULL on error set errno to
 *	indicate the error (errno will be ETIMEDOUT if the timeout is reached
 *      with no allocation granted)
 * NOTE: free the response using slurm_free_resource_allocation_response_msg()
 */
extern resource_allocation_response_msg_t *
slurm_allocate_resources_blocking PARAMS(
	(const job_desc_msg_t *user_req, time_t timeout,
	 void (*pending_callback)(uint32_t job_id)));

/*
 * slurm_free_resource_allocation_response_msg - free slurm resource
 *	allocation response message
 * IN msg - pointer to allocation response message
 * NOTE: buffer is loaded by slurm_allocate_resources
 */
extern void slurm_free_resource_allocation_response_msg PARAMS(
	(resource_allocation_response_msg_t * msg));

/*
 * slurm_free_job_alloc_info_response_msg - free slurm resource
 *	allocation lookup message
 * IN msg - pointer to job allocation info response message
 * NOTE: buffer is loaded by slurm_allocation_lookup
 */
extern void slurm_free_job_alloc_info_response_msg PARAMS(
	(job_alloc_info_response_msg_t * msg));

/*
 * slurm_allocation_lookup - retrieve info for an existing resource allocation
 * IN job_id - job allocation identifier
 * OUT resp - job allocation information
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 * NOTE: free "info" using slurm_free_job_alloc_info_response_msg
 */
extern int slurm_allocation_lookup PARAMS(
	(uint32_t job_id, job_alloc_info_response_msg_t **info));

/*
 * slurm_allocation_lookup_lite - retrieve minor info for an existing
 *				  resource allocation
 * IN job_id - job allocation identifier
 * OUT resp - job allocation information
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 * NOTE: free the response using slurm_free_resource_allocation_response_msg()
 */
extern int slurm_allocation_lookup_lite PARAMS(
	(uint32_t job_id, resource_allocation_response_msg_t **info));

/*
 * slurm_read_hostfile - Read a SLURM hostfile specified by "filename".
 *	"filename" must contain a list of SLURM NodeNames, one per line.
 *	Reads up to "n" number of hostnames from the file. Returns a
 *	string representing a hostlist ranged string of the contents of
 *	the file.  This is a helper function, it does not contact any
 *	SLURM daemons.
 *
 * IN filename - name of SLURM Hostlist file to be read.
 * IN n - number of NodeNames required
 * RET - a string representing the hostlist.  Returns NULL if there are
 *	fewer than "n" hostnames in the file, or if an error occurs.
 *
 * NOTE: Returned string must be freed with free().
 */
extern char *slurm_read_hostfile PARAMS((char *filename, int n));

/*
 * slurm_allocation_msg_thr_create - startup a message handler talking
 * with the controller dealing with messages from the controller during an
 * allocation.
 * IN port - port we are listening for messages on from the controller
 * IN callbacks - callbacks for different types of messages
 * RET allocation_msg_thread_t * or NULL on failure
 */
extern allocation_msg_thread_t *slurm_allocation_msg_thr_create PARAMS(
	(uint16_t *port, const slurm_allocation_callbacks_t *callbacks));

/*
 * slurm_allocation_msg_thr_destroy - shutdown the message handler talking
 * with the controller dealing with messages from the controller during an
 * allocation.
 * IN msg_thr - allocation_msg_thread_t pointer allocated with
 *              slurm_allocation_msg_thr_create
 */
extern void slurm_allocation_msg_thr_destroy PARAMS(
	(allocation_msg_thread_t * msg_thr));
/*
 * slurm_submit_batch_job - issue RPC to submit a job for later execution
 * NOTE: free the response using slurm_free_submit_response_response_msg
 * IN job_desc_msg - description of batch job request
 * OUT slurm_alloc_msg - response to request
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_submit_batch_job PARAMS(
	(job_desc_msg_t * job_desc_msg,
	 submit_response_msg_t ** slurm_alloc_msg));

/*
 * slurm_free_submit_response_response_msg - free slurm
 *	job submit response message
 * IN msg - pointer to job submit response message
 * NOTE: buffer is loaded by slurm_submit_batch_job
 */
extern void slurm_free_submit_response_response_msg PARAMS(
	(submit_response_msg_t *msg));

/*
 * slurm_job_will_run - determine if a job would execute immediately if
 *	submitted now
 * IN job_desc_msg - description of resource allocation request
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_job_will_run PARAMS((job_desc_msg_t * job_desc_msg));

/*
 * slurm_sbcast_lookup - retrieve info for an existing resource allocation
 *	including a credential needed for sbcast
 * IN jobid - job allocation identifier
 * OUT info - job allocation information including a credential for sbcast
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 * NOTE: free the "resp" using slurm_free_sbcast_cred_msg
 */
extern int slurm_sbcast_lookup PARAMS((uint32_t jobid,
				       job_sbcast_cred_msg_t **info));

extern void slurm_free_sbcast_cred_msg PARAMS((job_sbcast_cred_msg_t * msg));

/*****************************************************************************\
 *	JOB/STEP SIGNALING FUNCTIONS
\*****************************************************************************/

/*
 * slurm_kill_job - send the specified signal to all steps of an existing job
 * IN job_id     - the job's id
 * IN signal     - signal number
 * IN batch_flag - 1 to signal batch shell only, otherwise 0
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_kill_job PARAMS((uint32_t job_id, uint16_t signal,
				  uint16_t batch_flag));

/*
 * slurm_kill_job_step - send the specified signal to an existing job step
 * IN job_id  - the job's id
 * IN step_id - the job step's id
 * IN signal  - signal number
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_kill_job_step PARAMS((uint32_t job_id, uint32_t step_id,
				       uint16_t signal));

/*
 * slurm_signal_job - send the specified signal to all steps of an existing job
 * IN job_id     - the job's id
 * IN signal     - signal number
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_signal_job PARAMS((uint32_t job_id, uint16_t signal));

/*
 * slurm_signal_job_step - send the specified signal to an existing job step
 * IN job_id  - the job's id
 * IN step_id - the job step's id - use SLURM_BATCH_SCRIPT as the step_id
 *              to send a signal to a job's batch script
 * IN signal  - signal number
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_signal_job_step PARAMS((uint32_t job_id, uint32_t step_id,
					 uint16_t signal));


/*****************************************************************************\
 *	JOB/STEP COMPLETION FUNCTIONS
\*****************************************************************************/

/*
 * slurm_complete_job - note the completion of a job and all of its steps
 * IN job_id - the job's id
 * IN job_return_code - the highest exit code of any task of the job
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_complete_job PARAMS((uint32_t job_id,
				      uint32_t job_return_code));

/*
 * slurm_terminate_job - terminates all steps of an existing job by sending
 * 	a REQUEST_TERMINATE_JOB rpc to all slurmd in the the job allocation,
 *      and then calls slurm_complete_job().
 * IN job_id     - the job's id
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_terminate_job PARAMS((uint32_t job_id));

/*
 * slurm_terminate_job_step - terminates a job step by sending a
 * 	REQUEST_TERMINATE_TASKS rpc to all slurmd of a job step, and then
 *	calls slurm_complete_job_step() after verifying that all
 *	nodes in the job step no longer have running tasks from the job
 *	step.  (May take over 35 seconds to return.)
 * IN job_id  - the job's id
 * IN step_id - the job step's id - use SLURM_BATCH_SCRIPT as the step_id
 *              to terminate a job's batch script
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_terminate_job_step PARAMS((uint32_t job_id, uint32_t step_id));

/*****************************************************************************\
 *	SLURM TASK SPAWNING FUNCTIONS
\*****************************************************************************/

/*
 * slurm_step_ctx_params_t_init - This initializes parameters
 *	in the structure that you will pass to slurm_step_ctx_create().
 *	This function will NOT allocate any new memory.
 * IN ptr - pointer to a structure allocated by the user.  The structure will
 *      be initialized.
 */
extern void slurm_step_ctx_params_t_init PARAMS((slurm_step_ctx_params_t *ptr));

/*
 * slurm_step_ctx_create - Create a job step and its context.
 * IN step_params - job step parameters
 * RET the step context or NULL on failure with slurm errno set
 * NOTE: Free allocated memory using slurm_step_ctx_destroy.
 */
extern slurm_step_ctx_t *slurm_step_ctx_create PARAMS(
	(const slurm_step_ctx_params_t *step_params));

/*
 * slurm_step_ctx_create_no_alloc - Create a job step and its context without
 *                                  getting an allocation.
 * IN step_params - job step parameters
 * IN step_id     - since we are faking it give me the id to use
 * RET the step context or NULL on failure with slurm errno set
 * NOTE: Free allocated memory using slurm_step_ctx_destroy.
 */
extern slurm_step_ctx_t *
slurm_step_ctx_create_no_alloc PARAMS(
	(const slurm_step_ctx_params_t *step_params, uint32_t step_id));

/*
 * slurm_step_ctx_get - get parameters from a job step context.
 * IN ctx - job step context generated by slurm_step_ctx_create
 * RET SLURM_SUCCESS or SLURM_ERROR (with slurm_errno set)
 */
extern int slurm_step_ctx_get PARAMS((slurm_step_ctx_t *ctx,
				      int ctx_key, ...));

/*
 * slurm_jobinfo_ctx_get - get parameters from jobinfo context.
 * IN jobinfo - job information from context, returned by slurm_step_ctx_get()
 * IN data_type - type of data required, specific to the switch type
 * OUT data - the requested data type
 * RET SLURM_SUCCESS or SLURM_ERROR (with slurm_errno set)
 */
extern int slurm_jobinfo_ctx_get PARAMS((switch_jobinfo_t *jobinfo,
					 int data_type, void *data));

/*
 * slurm_step_ctx_daemon_per_node_hack - Hack the step context
 *	to run a single process per node, regardless of the settings
 *	selected at slurm_step_ctx_create time.
 *
 *	This is primarily used when launching 1 task per node as done
 * 	with IBM's PE where we want to launch a single pmd daemon
 *	on each node regardless of the number of tasks running on each
 *	node.
 * IN ctx - job step context generated by slurm_step_ctx_create
 * IN node_list - node list of nodes to run on
 * IN node_cnt - number of nodes to run on
 * IN/OUT curr_task_num - task_id of newest task, initialze to zero
 * RET SLURM_SUCCESS or SLURM_ERROR (with slurm_errno set)
 */
extern int slurm_step_ctx_daemon_per_node_hack PARAMS(
	(slurm_step_ctx_t *ctx, char *node_list, uint32_t node_cnt,
	 uint32_t *curr_task_num));

/*
 * slurm_step_ctx_destroy - free allocated memory for a job step context.
 * IN ctx - job step context generated by slurm_step_ctx_create
 * RET SLURM_SUCCESS or SLURM_ERROR (with slurm_errno set)
 */
extern int slurm_step_ctx_destroy PARAMS((slurm_step_ctx_t *ctx));

/*
 * slurm_step_launch_params_t_init - initialize a user-allocated
 *      slurm_step_launch_params_t structure with default values.
 *	default values.  This function will NOT allocate any new memory.
 * IN ptr - pointer to a structure allocated by the use.  The structure will
 *      be initialized.
 */
extern void slurm_step_launch_params_t_init
PARAMS((slurm_step_launch_params_t *ptr));

/*
 * slurm_step_launch - launch a parallel job step
 * IN ctx - job step context generated by slurm_step_ctx_create
 * IN params - job step parameters
 * IN callbacks - Identify functions to be called when various events occur
 * RET SLURM_SUCCESS or SLURM_ERROR (with errno set)
 */
extern int slurm_step_launch PARAMS(
	(slurm_step_ctx_t *ctx,
	 const slurm_step_launch_params_t *params,
	 const slurm_step_launch_callbacks_t *callbacks));

/*
 * slurm_step_launch - Add tasks to a step that was already started
 * IN ctx - job step context generated by slurm_step_ctx_create
 * IN params - job step parameters
 * IN node_list - list of extra nodes to add
 * IN start_nodeid - in the global scheme which node id is the first
 *                   node in node_list.
 * RET SLURM_SUCCESS or SLURM_ERROR (with errno set)
 */
extern int slurm_step_launch_add PARAMS(
	(slurm_step_ctx_t *ctx,
	 const slurm_step_launch_params_t *params,
	 char *node_list, int start_nodeid));

/*
 * Block until all tasks have started.
 */
extern int slurm_step_launch_wait_start PARAMS((slurm_step_ctx_t *ctx));

/*
 * Block until all tasks have finished (or failed to start altogether).
 */
extern void slurm_step_launch_wait_finish PARAMS((slurm_step_ctx_t *ctx));

/*
 * Abort an in-progress launch, or terminate the fully launched job step.
 *
 * Can be called from a signal handler.
 */
extern void slurm_step_launch_abort PARAMS((slurm_step_ctx_t *ctx));

/*
 * Forward a signal to all those nodes with running tasks
 */
extern void slurm_step_launch_fwd_signal PARAMS((slurm_step_ctx_t *ctx,
						 int signo));

/*
 * Wake tasks stopped for debugging on nodes with running tasks
 */
extern void slurm_step_launch_fwd_wake PARAMS((slurm_step_ctx_t *ctx));

/*
 * Specify the plugin name to be used. This may be needed to specify the
 * non-default MPI plugin when using SLURM API to launch tasks.
 * IN plugin name - "none", "pmi2", etc.
 * RET SLURM_SUCCESS or SLURM_ERROR (with errno set)
 */
extern int slurm_mpi_plugin_init PARAMS((char *plugin_name));

/*****************************************************************************\
 *	SLURM CONTROL CONFIGURATION READ/PRINT/UPDATE FUNCTIONS
\*****************************************************************************/

/*
 * slurm_api_version - Return a single number reflecting the SLURM API's
 *	version number. Use the macros SLURM_VERSION_NUM, SLURM_VERSION_MAJOR,
 *	SLURM_VERSION_MINOR, and SLURM_VERSION_MICRO to work with this value
 * RET API's version number
 */
extern long slurm_api_version PARAMS((void));

/*
 * slurm_load_ctl_conf - issue RPC to get slurm control configuration
 *	information if changed since update_time
 * IN update_time - time of current configuration data
 * IN slurm_ctl_conf_ptr - place to store slurm control configuration
 *	pointer
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 * NOTE: free the response using slurm_free_ctl_conf
 */
extern int slurm_load_ctl_conf PARAMS(
	(time_t update_time,
	 slurm_ctl_conf_t  **slurm_ctl_conf_ptr));

/*
 * slurm_free_ctl_conf - free slurm control information response message
 * IN msg - pointer to slurm control information response message
 * NOTE: buffer is loaded by slurm_load_ctl_conf
 */
extern void slurm_free_ctl_conf PARAMS((slurm_ctl_conf_t* slurm_ctl_conf_ptr));

/*
 * slurm_print_ctl_conf - output the contents of slurm control configuration
 *	message as loaded using slurm_load_ctl_conf
 * IN out - file to write to
 * IN slurm_ctl_conf_ptr - slurm control configuration pointer
 */
extern void slurm_print_ctl_conf PARAMS(
	(FILE * out, slurm_ctl_conf_t* slurm_ctl_conf_ptr));

/*
 * slurm_ctl_conf_2_key_pairs - put the slurm_ctl_conf_t variables into
 *	a List of opaque data type config_key_pair_t
 * IN slurm_ctl_conf_ptr - slurm control configuration pointer
 * RET List of opaque data type config_key_pair_t
 */
extern void *slurm_ctl_conf_2_key_pairs PARAMS(
	(slurm_ctl_conf_t* slurm_ctl_conf_ptr));

/*
 * slurm_load_slurmd_status - issue RPC to get the status of slurmd
 *	daemon on this machine
 * IN slurmd_status_ptr - place to store slurmd status information
 * RET 0 or -1 on error
 * NOTE: free the response using slurm_free_slurmd_status()
 */
extern int slurm_load_slurmd_status PARAMS(
	(slurmd_status_t **slurmd_status_ptr));

/*
 * slurm_free_slurmd_status - free slurmd state information
 * IN msg - pointer to slurmd state information
 * NOTE: buffer is loaded by slurm_load_slurmd_status
 */
extern void slurm_free_slurmd_status PARAMS(
	(slurmd_status_t* slurmd_status_ptr));

/*
 * slurm_print_slurmd_status - output the contents of slurmd status
 *	message as loaded using slurm_load_slurmd_status
 * IN out - file to write to
 * IN slurmd_status_ptr - slurmd status pointer
 */
void slurm_print_slurmd_status PARAMS(
	(FILE* out, slurmd_status_t * slurmd_status_ptr));

/*
 * slurm_print_key_pairs - output the contents of key_pairs
 *	which is a list of opaque data type config_key_pair_t
 * IN out - file to write to
 * IN key_pairs - List contianing key pairs to be printed
 * IN title - title of key pair list
 */
void slurm_print_key_pairs PARAMS(
	(FILE* out, void* key_pairs, char *title));

/*
 * slurm_init_update_step_msg - initialize step update message with default
 *	values before calling slurm_update_step()
 * OUT step_msg - step update messasge descriptor
 */
extern void slurm_init_update_step_msg PARAMS(
	(step_update_request_msg_t * step_msg));

/* Get scheduling statistics */
extern int  slurm_get_statistics PARAMS((stats_info_response_msg_t **buf,
					 stats_info_request_msg_t *req));

/* Reset scheduling statistics */
extern int  slurm_reset_statistics PARAMS((stats_info_request_msg_t *req));

/*****************************************************************************\
 *	SLURM JOB RESOURCES READ/PRINT FUNCTIONS
\*****************************************************************************/

/*
 * slurm_job_cpus_allocated_on_node_id -
 *                        get the number of cpus allocated to a job
 *			  on a node by node id
 * IN job_resrcs_ptr	- pointer to job_resources structure
 * IN node_id		- zero-origin node id in allocation
 * RET number of CPUs allocated to job on this node or -1 on error
 */
extern int slurm_job_cpus_allocated_on_node_id PARAMS(
	(job_resources_t *job_resrcs_ptr, int node_id));

/*
 * slurm_job_cpus_allocated_on_node -
 *                        get the number of cpus allocated to a job
 *			  on a node by node name
 * IN job_resrcs_ptr	- pointer to job_resources structure
 * IN node_name		- name of node
 * RET number of CPUs allocated to job on this node or -1 on error
 */
extern int slurm_job_cpus_allocated_on_node PARAMS(
	(job_resources_t *job_resrcs_ptr, const char *node_name));

/*****************************************************************************\
 *	SLURM JOB CONTROL CONFIGURATION READ/PRINT/UPDATE FUNCTIONS
\*****************************************************************************/

/*
 * slurm_free_job_info_msg - free the job information response message
 * IN msg - pointer to job information response message
 * NOTE: buffer is loaded by slurm_load_jobs.
 */
extern void slurm_free_job_info_msg PARAMS((job_info_msg_t * job_buffer_ptr));

/*
 * slurm_get_end_time - get the expected end time for a given slurm job
 * IN jobid     - slurm job id
 * end_time_ptr - location in which to store scheduled end time for job
 * RET 0 or -1 on error
 */
extern int slurm_get_end_time PARAMS((uint32_t jobid, time_t *end_time_ptr));

/*
 * slurm_get_rem_time - get the expected time remaining for a given job
 * IN jobid     - slurm job id
 * RET remaining time in seconds or -1 on error
 */
extern long slurm_get_rem_time PARAMS((uint32_t jobid));

/*
 * slurm_job_node_ready - report if nodes are ready for job to execute now
 * IN job_id - slurm job id
 * RET: READY_* values defined above
 */
extern int slurm_job_node_ready(uint32_t job_id);

/*
 * slurm_load_job - issue RPC to get job information for one job ID
 * IN job_info_msg_pptr - place to store a job configuration pointer
 * IN job_id -  ID of job we want information about
 * IN show_flags - job filtering options
 * RET 0 or -1 on error
 * NOTE: free the response using slurm_free_job_info_msg
 */
extern int slurm_load_job PARAMS((job_info_msg_t **resp, uint32_t job_id,
				  uint16_t show_flags));

/*
 * slurm_load_jobs - issue RPC to get slurm all job configuration
 *	information if changed since update_time
 * IN update_time - time of current configuration data
 * IN job_info_msg_pptr - place to store a job configuration pointer
 * IN show_flags - job filtering options
 * RET 0 or -1 on error
 * NOTE: free the response using slurm_free_job_info_msg
 */
extern int slurm_load_jobs PARAMS(
	(time_t update_time, job_info_msg_t **job_info_msg_pptr,
	 uint16_t show_flags));

/*
 * slurm_notify_job - send message to the job's stdout,
 *	usable only by user root
 * IN job_id - slurm job_id or 0 for all jobs
 * IN message - arbitrary message
 * RET 0 or -1 on error
 */
extern int slurm_notify_job PARAMS((uint32_t job_id, char *message));

/*
 * slurm_pid2jobid - issue RPC to get the slurm job_id given a process_id
 *	on this machine
 * IN job_pid - process_id of interest on this machine
 * OUT job_id_ptr - place to store a slurm job_id
 * RET 0 or -1 on error
 */
extern int slurm_pid2jobid PARAMS((pid_t job_pid, uint32_t * job_id_ptr));

/*
 * slurm_print_job_info - output information about a specific Slurm
 *	job based upon message as loaded using slurm_load_jobs
 * IN out - file to write to
 * IN job_ptr - an individual job information record pointer
 * IN one_liner - print as a single line if true
 */
extern void slurm_print_job_info PARAMS((FILE*, slurm_job_info_t * job_ptr,
					 int one_liner));

/*
 * slurm_print_job_info_msg - output information about all Slurm
 *	jobs based upon message as loaded using slurm_load_jobs
 * IN out - file to write to
 * IN job_info_msg_ptr - job information message pointer
 * IN one_liner - print as a single line if true
 */
extern void slurm_print_job_info_msg PARAMS(
	(FILE * out, job_info_msg_t * job_info_msg_ptr, int one_liner));

/*
 * slurm_sprint_job_info - output information about a specific Slurm
 *	job based upon message as loaded using slurm_load_jobs
 * IN job_ptr - an individual job information record pointer
 * IN one_liner - print as a single line if true
 * RET out - char * containing formatted output (must be freed after call)
 *           NULL is returned on failure.
 */
extern char *slurm_sprint_job_info PARAMS((slurm_job_info_t * job_ptr,
					   int one_liner));

/*
 * slurm_update_job - issue RPC to a job's configuration per request,
 *	only usable by user root or (for some parameters) the job's owner
 * IN job_msg - description of job updates
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_update_job PARAMS((job_desc_msg_t * job_msg));

/*****************************************************************************\
 *	SLURM JOB STEP CONFIGURATION READ/PRINT/UPDATE FUNCTIONS
\*****************************************************************************/

/*
 * slurm_get_job_steps - issue RPC to get specific slurm job step
 *	configuration information if changed since update_time.
 *	a job_id value of NO_VAL implies all jobs, a step_id value of
 *	NO_VAL implies all steps
 * IN update_time - time of current configuration data
 * IN job_id - get information for specific job id, NO_VAL for all jobs
 * IN step_id - get information for specific job step id, NO_VAL for all
 *	job steps
 * IN step_response_pptr - place to store a step response pointer
 * IN show_flags - job step filtering options
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 * NOTE: free the response using slurm_free_job_step_info_response_msg
 */
extern int slurm_get_job_steps PARAMS(
	(time_t update_time, uint32_t job_id, uint32_t step_id,
	 job_step_info_response_msg_t **step_response_pptr,
	 uint16_t show_flags));

/*
 * slurm_free_job_step_info_response_msg - free the job step
 *	information response message
 * IN msg - pointer to job step information response message
 * NOTE: buffer is loaded by slurm_get_job_steps.
 */
extern void slurm_free_job_step_info_response_msg PARAMS(
	(job_step_info_response_msg_t * msg));

/*
 * slurm_print_job_step_info_msg - output information about all Slurm
 *	job steps based upon message as loaded using slurm_get_job_steps
 * IN out - file to write to
 * IN job_step_info_msg_ptr - job step information message pointer
 * IN one_liner - print as a single line if true
 */
extern void slurm_print_job_step_info_msg PARAMS(
	(FILE * out, job_step_info_response_msg_t * job_step_info_msg_ptr,
	 int one_liner));

/*
 * slurm_print_job_step_info - output information about a specific Slurm
 *	job step based upon message as loaded using slurm_get_job_steps
 * IN out - file to write to
 * IN job_ptr - an individual job step information record pointer
 * IN one_liner - print as a single line if true
 */
extern void slurm_print_job_step_info PARAMS(
	(FILE * out, job_step_info_t * step_ptr, int one_liner));

/*
 * slurm_job_step_layout_get - get the slurm_step_layout_t structure for
 *      a particular job step
 *
 * IN job_id
 * IN step_id
 * RET pointer to a slurm_step_layout_t (free with
 *   slurm_free_step_layout) on success, and NULL on error.
 */
extern slurm_step_layout_t *slurm_job_step_layout_get PARAMS(
	(uint32_t job_id, uint32_t step_id));

/*
 * slurm_sprint_job_step_info - output information about a specific Slurm
 *	job step based upon message as loaded using slurm_get_job_steps
 * IN job_ptr - an individual job step information record pointer
 * IN one_liner - print as a single line if true
 * RET out - char * containing formatted output (must be freed after call)
 *           NULL is returned on failure.
 */
extern char *slurm_sprint_job_step_info PARAMS((job_step_info_t * step_ptr,
						int one_liner));

/*
 * slurm_job_step_stat - status a current step
 *
 * IN job_id
 * IN step_id
 * IN node_list, optional, if NULL then all nodes in step are returned.
 * OUT resp
 * RET SLURM_SUCCESS on success SLURM_ERROR else
 */
extern int slurm_job_step_stat PARAMS((uint32_t job_id, uint32_t step_id,
				       char *node_list,
				       job_step_stat_response_msg_t **resp));

/*
 * slurm_job_step_get_pids - get the complete list of pids for a given
 *      job step
 *
 * IN job_id
 * IN step_id
 * OUT resp
 * RET SLURM_SUCCESS on success SLURM_ERROR else
 */
extern int slurm_job_step_get_pids PARAMS(
	(uint32_t job_id, uint32_t step_id,
	 char *node_list,
	 job_step_pids_response_msg_t **resp));

extern void slurm_job_step_layout_free PARAMS((slurm_step_layout_t *layout));
extern void slurm_job_step_pids_free PARAMS((job_step_pids_t *object));
extern void slurm_job_step_pids_response_msg_free PARAMS((void *object));
extern void slurm_job_step_stat_free(job_step_stat_t *object);
extern void slurm_job_step_stat_response_msg_free(void *object);

/* Update the time limit of a job step,
 * IN step_msg - step update messasge descriptor
 * RET 0 or -1 on error */
extern int slurm_update_step PARAMS((step_update_request_msg_t * step_msg));

/*****************************************************************************\
 *	SLURM NODE CONFIGURATION READ/PRINT/UPDATE FUNCTIONS
\*****************************************************************************/

/*
 * slurm_load_node - issue RPC to get slurm all node configuration information
 *	if changed since update_time
 * IN update_time - time of current configuration data
 * IN node_info_msg_pptr - place to store a node configuration pointer
 * IN show_flags - node filtering options
 * RET 0 or a slurm error code
 * NOTE: free the response using slurm_free_node_info_msg
 */
extern int slurm_load_node PARAMS(
	(time_t update_time, node_info_msg_t **node_info_msg_pptr,
	 uint16_t show_flags));

/*
 * slurm_free_node_info_msg - free the node information response message
 * IN msg - pointer to node information response message
 * NOTE: buffer is loaded by slurm_load_node.
 */
extern void slurm_free_node_info_msg PARAMS(
	(node_info_msg_t * node_buffer_ptr));

/*
 * slurm_print_node_info_msg - output information about all Slurm nodes
 *	based upon message as loaded using slurm_load_node
 * IN out - file to write to
 * IN node_info_msg_ptr - node information message pointer
 * IN one_liner - print as a single line if true
 */
extern void slurm_print_node_info_msg PARAMS(
	(FILE * out, node_info_msg_t * node_info_msg_ptr, int one_liner));

/*
 * slurm_print_node_table - output information about a specific Slurm nodes
 *	based upon message as loaded using slurm_load_node
 * IN out - file to write to
 * IN node_ptr - an individual node information record pointer
 * IN node_scaling - number of nodes each node represents
 * IN one_liner - print as a single line if true
 */
extern void slurm_print_node_table PARAMS(
	(FILE * out, node_info_t * node_ptr,
	 int node_scaling, int one_liner));

/*
 * slurm_sprint_node_table - output information about a specific Slurm nodes
 *	based upon message as loaded using slurm_load_node
 * IN node_ptr - an individual node information record pointer
 * IN one_liner - print as a single line if true
 * IN node_scaling - number of nodes each node represents
 * RET out - char * containing formatted output (must be freed after call)
 *           NULL is returned on failure.
 */
extern char *slurm_sprint_node_table PARAMS((node_info_t * node_ptr,
					     int node_scaling,
					     int one_liner));

/*
 * slurm_init_update_node_msg - initialize node update message
 * OUT update_node_msg - user defined node descriptor
 */
void slurm_init_update_node_msg PARAMS((update_node_msg_t * update_node_msg));

/*
 * slurm_update_node - issue RPC to a node's configuration per request,
 *	only usable by user root
 * IN node_msg - description of node updates
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_update_node PARAMS((update_node_msg_t * node_msg));


/*****************************************************************************\
 *	SLURM FRONT_END CONFIGURATION READ/PRINT/UPDATE FUNCTIONS
\*****************************************************************************/

/*
 * slurm_load_front_end - issue RPC to get slurm all front_end configuration
 *	information if changed since update_time
 * IN update_time - time of current configuration data
 * IN front_end_info_msg_pptr - place to store a front_end configuration pointer
 * RET 0 or a slurm error code
 * NOTE: free the response using slurm_free_front_end_info_msg
 */
extern int slurm_load_front_end PARAMS(
	(time_t update_time, front_end_info_msg_t **resp));

/*
 * slurm_free_front_end_info_msg - free the front_end information response
 *	message
 * IN msg - pointer to front_end information response message
 * NOTE: buffer is loaded by slurm_load_front_end.
 */
extern void slurm_free_front_end_info_msg PARAMS(
	(front_end_info_msg_t * front_end_buffer_ptr));

/*
 * slurm_print_front_end_info_msg - output information about all Slurm
 *	front_ends based upon message as loaded using slurm_load_front_end
 * IN out - file to write to
 * IN front_end_info_msg_ptr - front_end information message pointer
 * IN one_liner - print as a single line if true
 */
extern void slurm_print_front_end_info_msg PARAMS(
	(FILE * out, front_end_info_msg_t * front_end_info_msg_ptr,
	 int one_liner));
/*
 * slurm_print_front_end_table - output information about a specific Slurm
 *	front_ends based upon message as loaded using slurm_load_front_end
 * IN out - file to write to
 * IN front_end_ptr - an individual front_end information record pointer
 * IN one_liner - print as a single line if true
 */
extern void slurm_print_front_end_table PARAMS(
	(FILE * out, front_end_info_t * front_end_ptr, int one_liner));

/*
 * slurm_sprint_front_end_table - output information about a specific Slurm
 *	front_end based upon message as loaded using slurm_load_front_end
 * IN front_end_ptr - an individual front_end information record pointer
 * IN one_liner - print as a single line if true
 * RET out - char * containing formatted output (must be freed after call)
 *           NULL is returned on failure.
 */
extern char *slurm_sprint_front_end_table PARAMS(
	(front_end_info_t * front_end_ptr, int one_liner));

/*
 * slurm_init_update_front_end_msg - initialize front_end node update message
 * OUT update_front_end_msg - user defined node descriptor
 */
void slurm_init_update_front_end_msg PARAMS(
	(update_front_end_msg_t * update_front_end_msg));

/*
 * slurm_update_front_end - issue RPC to a front_end node's configuration per
 *	request, only usable by user root
 * IN front_end_msg - description of front_end node updates
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_update_front_end PARAMS(
	(update_front_end_msg_t * front_end_msg));

/*****************************************************************************\
 *	SLURM SWITCH TOPOLOGY CONFIGURATION READ/PRINT FUNCTIONS
\*****************************************************************************/

/*
 * slurm_load_topo - issue RPC to get slurm all switch topology configuration
 *	information
 * IN node_info_msg_pptr - place to store a node configuration pointer
 * RET 0 or a slurm error code
 * NOTE: free the response using slurm_free_topo_info_msg
 */
extern int slurm_load_topo PARAMS(
	(topo_info_response_msg_t **topo_info_msg_pptr));

/*
 * slurm_free_topo_info_msg - free the switch topology configuration
 *	information response message
 * IN msg - pointer to switch topology configuration response message
 * NOTE: buffer is loaded by slurm_load_topo.
 */
extern void slurm_free_topo_info_msg PARAMS((topo_info_response_msg_t *msg));

/*
 * slurm_print_topo_info_msg - output information about all switch topology
 *	configuration information based upon message as loaded using
 *	slurm_load_topo
 * IN out - file to write to
 * IN topo_info_msg_ptr - switch topology information message pointer
 * IN one_liner - print as a single line if not zero
 */
extern void slurm_print_topo_info_msg PARAMS(
	(FILE * out, topo_info_response_msg_t *topo_info_msg_ptr,
	 int one_liner));

/*
 * slurm_print_topo_record - output information about a specific Slurm topology
 *	record based upon message as loaded using slurm_load_topo
 * IN out - file to write to
 * IN topo_ptr - an individual switch information record pointer
 * IN one_liner - print as a single line if not zero
 * RET out - char * containing formatted output (must be freed after call)
 *	   NULL is returned on failure.
 */
extern void slurm_print_topo_record PARAMS((FILE * out, topo_info_t *topo_ptr,
					    int one_liner));

/*****************************************************************************\
 *	SLURM SELECT READ/PRINT/UPDATE FUNCTIONS
\*****************************************************************************/

/*
 * slurm_get_select_jobinfo - get data from a select job credential
 * IN jobinfo  - updated select job credential
 * IN data_type - type of data to enter into job credential
 * IN/OUT data - the data to enter into job credential
 * RET 0 or -1 on error
 */
extern int slurm_get_select_jobinfo PARAMS(
	(dynamic_plugin_data_t *jobinfo, enum select_jobdata_type data_type,
	 void *data));

/*
 * slurm_get_select_nodeinfo - get data from a select node credential
 * IN nodeinfo  - updated select node credential
 * IN data_type - type of data to enter into node credential
 * IN state     - state of node query
 * IN/OUT data  - the data to enter into node credential
 * RET 0 or -1 on error
 */
extern int slurm_get_select_nodeinfo PARAMS(
	(dynamic_plugin_data_t *nodeinfo, enum select_nodedata_type data_type,
	 enum node_states state, void *data));

/*****************************************************************************\
 *	SLURM PARTITION CONFIGURATION READ/PRINT/UPDATE FUNCTIONS
\*****************************************************************************/

/*
 * slurm_init_part_desc_msg - initialize partition descriptor with
 *	default values
 * IN/OUT update_part_msg - user defined partition descriptor
 */
extern void slurm_init_part_desc_msg PARAMS(
	(update_part_msg_t * update_part_msg));

/*
 * slurm_load_partitions - issue RPC to get slurm all partition configuration
 *	information if changed since update_time
 * IN update_time - time of current configuration data
 * IN partition_info_msg_pptr - place to store a partition configuration
 *	pointer
 * IN show_flags - partitions filtering options
 * RET 0 or a slurm error code
 * NOTE: free the response using slurm_free_partition_info_msg
 */
extern int slurm_load_partitions PARAMS(
	(time_t update_time, partition_info_msg_t **part_buffer_ptr,
	 uint16_t show_flags));

/*
 * slurm_free_partition_info_msg - free the partition information
 *	response message
 * IN msg - pointer to partition information response message
 * NOTE: buffer is loaded by slurm_load_partitions
 */
extern void slurm_free_partition_info_msg PARAMS(
	(partition_info_msg_t * part_info_ptr));

/*
 * slurm_print_partition_info_msg - output information about all Slurm
 *	partitions based upon message as loaded using slurm_load_partitions
 * IN out - file to write to
 * IN part_info_ptr - partitions information message pointer
 * IN one_liner - print as a single line if true
 */
extern void slurm_print_partition_info_msg PARAMS(
	(FILE * out, partition_info_msg_t * part_info_ptr, int one_liner));

/*
 * slurm_print_partition_info - output information about a specific Slurm
 *	partition based upon message as loaded using slurm_load_partitions
 * IN out - file to write to
 * IN part_ptr - an individual partition information record pointer
 * IN one_liner - print as a single line if true
 */
extern void slurm_print_partition_info PARAMS(
	(FILE *out , partition_info_t * part_ptr, int one_liner));

/*
 * slurm_sprint_partition_info - output information about a specific Slurm
 *	partition based upon message as loaded using slurm_load_partitions
 * IN part_ptr - an individual partition information record pointer
 * IN one_liner - print as a single line if true
 * RET out - char * with formatted output (must be freed after call)
 *           NULL is returned on failure.
 */
extern char *slurm_sprint_partition_info PARAMS(
	(partition_info_t * part_ptr, int one_liner));

/*
 * slurm_create_partition - create a new partition, only usable by user root
 * IN part_msg - description of partition configuration
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_create_partition PARAMS((update_part_msg_t * part_msg));

/*
 * slurm_update_partition - issue RPC to update a partition's configuration
 *	per request, only usable by user root
 * IN part_msg - description of partition updates
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_update_partition PARAMS((update_part_msg_t * part_msg));

/*
 * slurm_delete_partition - issue RPC to delete a partition, only usable
 *	by user root
 * IN part_msg - description of partition to delete
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_delete_partition PARAMS((delete_part_msg_t * part_msg));

/*****************************************************************************\
 *	SLURM RESERVATION CONFIGURATION READ/PRINT/UPDATE FUNCTIONS
\*****************************************************************************/

/*
 * slurm_init_resv_desc_msg - initialize reservation descriptor with
 *	default values
 * OUT job_desc_msg - user defined partition descriptor
 */
extern void slurm_init_resv_desc_msg PARAMS(
	(resv_desc_msg_t * update_resv_msg));
/*
 * slurm_create_reservation - create a new reservation, only usable by user root
 * IN resv_msg - description of reservation
 * RET name of reservation on success (caller must free the memory),
 *	otherwise return NULL and set errno to indicate the error
 */
extern char * slurm_create_reservation PARAMS(
	(resv_desc_msg_t * resv_msg));

/*
 * slurm_update_reservation - modify an existing reservation, only usable by
 *	user root
 * IN resv_msg - description of reservation
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_update_reservation PARAMS((resv_desc_msg_t * resv_msg));

/*
 * slurm_delete_reservation - issue RPC to delete a reservation, only usable
 *	by user root
 * IN resv_msg - description of reservation to delete
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_delete_reservation PARAMS(
	(reservation_name_msg_t * resv_msg));

/*
 * slurm_load_reservations - issue RPC to get all slurm reservation
 *	configuration information if changed since update_time
 * IN update_time - time of current configuration data
 * IN reserve_info_msg_pptr - place to store a reservation configuration
 *	pointer
 * RET 0 or a slurm error code
 * NOTE: free the response using slurm_free_reservation_info_msg
 */
extern int slurm_load_reservations PARAMS((time_t update_time,
					   reserve_info_msg_t **resp));

/*
 * slurm_print_reservation_info_msg - output information about all Slurm
 *	reservations based upon message as loaded using slurm_load_reservation
 * IN out - file to write to
 * IN resv_info_ptr - reservation information message pointer
 * IN one_liner - print as a single line if true
 */
void slurm_print_reservation_info_msg PARAMS((FILE* out,
					      reserve_info_msg_t *resv_info_ptr,
					      int one_liner));

/*
 * slurm_print_reservation_info - output information about a specific Slurm
 *	reservation based upon message as loaded using slurm_load_reservation
 * IN out - file to write to
 * IN resv_ptr - an individual reservation information record pointer
 * IN one_liner - print as a single line if true
 */
void slurm_print_reservation_info PARAMS((FILE* out,
					  reserve_info_t * resv_ptr,
					  int one_liner));

/*
 * slurm_sprint_reservation_info - output information about a specific Slurm
 *	reservation based upon message as loaded using slurm_load_reservations
 * IN resv_ptr - an individual reservation information record pointer
 * IN one_liner - print as a single line if true
 * RET out - char * containing formatted output (must be freed after call)
 *           NULL is returned on failure.
 */
char *slurm_sprint_reservation_info PARAMS((reserve_info_t * resv_ptr,
					    int one_liner));

/*
 * slurm_free_reservation_info_msg - free the reservation information
 *	response message
 * IN msg - pointer to reservation information response message
 * NOTE: buffer is loaded by slurm_load_reservation
 */
extern void slurm_free_reservation_info_msg PARAMS(
	(reserve_info_msg_t * resv_info_ptr));

/*****************************************************************************\
 *	SLURM PING/RECONFIGURE/SHUTDOWN FUNCTIONS
\*****************************************************************************/

/*
 * slurm_ping - issue RPC to have Slurm controller (slurmctld)
 * IN controller - 1==primary controller, 2==secondary controller
 * RET 0 or a slurm error code
 */
extern int slurm_ping PARAMS((int primary));

/*
 * slurm_reconfigure - issue RPC to have Slurm controller (slurmctld)
 *	reload its configuration file
 * RET 0 or a slurm error code
 */
extern int slurm_reconfigure PARAMS((void));

/*
 * slurm_shutdown - issue RPC to have Slurm controller (slurmctld)
 *	cease operations, both the primary and backup controller
 *	are shutdown.
 * IN options - 0: all slurm daemons are shutdown
 *              1: slurmctld generates a core file
 *              2: only the slurmctld is shutdown (no core file)
 * RET 0 or a slurm error code
 */
extern int slurm_shutdown PARAMS((uint16_t options));

/*
 * slurm_takeover - issue RPC to have Slurm backup controller (slurmctld)
 *	take over the primary controller.
 *
 * RET 0 or a slurm error code
 */
extern int slurm_takeover PARAMS((void));

/*
 * slurm_set_debugflags - issue RPC to set slurm controller debug flags
 * IN debug_flags_plus  - debug flags to be added
 * IN debug_flags_minus - debug flags to be removed
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_set_debugflags PARAMS((uint32_t debug_flags_plus,
					uint32_t debug_flags_minus));

/*
 * slurm_set_debug_level - issue RPC to set slurm controller debug level
 * IN debug_level - requested debug level
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_set_debug_level PARAMS((uint32_t debug_level));

/*
 * slurm_set_schedlog_level - issue RPC to set slurm scheduler log level
 * IN schedlog_level - requested scheduler log level
 * RET 0 on success, otherwise return -1 and set errno to indicate the error
 */
extern int slurm_set_schedlog_level PARAMS((uint32_t schedlog_level));

/*****************************************************************************\
 *      SLURM JOB SUSPEND FUNCTIONS
\*****************************************************************************/

/*
 * slurm_suspend - suspend execution of a job.
 * IN job_id  - job on which to perform operation
 * RET 0 or a slurm error code
 */
extern int slurm_suspend PARAMS((uint32_t job_id));

/*
 * slurm_resume - resume execution of a previously suspended job.
 * IN job_id  - job on which to perform operation
 * RET 0 or a slurm error code
 */
extern int slurm_resume PARAMS((uint32_t job_id));

/*
 * slurm_requeue - re-queue a batch job, if already running
 *	then terminate it first
 * IN job_id  - job on which to perform operation
 * RET 0 or a slurm error code
 */
extern int slurm_requeue PARAMS((uint32_t job_id));

/*****************************************************************************\
 *      SLURM JOB CHECKPOINT FUNCTIONS
\*****************************************************************************/

/*
 * slurm_checkpoint_able - determine if the specified job step can presently
 *	be checkpointed
 * IN job_id  - job on which to perform operation
 * IN step_id - job step on which to perform operation
 * OUT start_time - time at which checkpoint request was issued
 * RET 0 (can be checkpoined) or a slurm error code
 */
extern int slurm_checkpoint_able PARAMS((uint32_t job_id, uint32_t step_id,
					 time_t *start_time));

/*
 * slurm_checkpoint_disable - disable checkpoint requests for some job step
 * IN job_id  - job on which to perform operation
 * IN step_id - job step on which to perform operation
 * RET 0 or a slurm error code
 */
extern int slurm_checkpoint_disable PARAMS((uint32_t job_id,
					    uint32_t step_id));


/*
 * slurm_checkpoint_enable - enable checkpoint requests for some job step
 * IN job_id  - job on which to perform operation
 * IN step_id - job step on which to perform operation
 * RET 0 or a slurm error code
 */
extern int slurm_checkpoint_enable PARAMS((uint32_t job_id,
					   uint32_t step_id));

/*
 * slurm_checkpoint_create - initiate a checkpoint requests for some job step.
 *	the job will continue execution after the checkpoint operation completes
 * IN job_id  - job on which to perform operation
 * IN step_id - job step on which to perform operation
 * IN max_wait - maximum wait for operation to complete, in seconds
 * IN image_dir - directory to store image files
 * RET 0 or a slurm error code
 */
extern int slurm_checkpoint_create PARAMS((uint32_t job_id,
					   uint32_t step_id,
					   uint16_t max_wait,
					   char *image_dir));

/*
 * slurm_checkpoint_requeue - initiate a checkpoint requests for some job.
 *	the job will be requeued after the checkpoint operation completes
 * IN job_id  - job on which to perform operation
 * IN max_wait - maximum wait for operation to complete, in seconds
 * IN image_dir - directory used to get/put checkpoint images
 * RET 0 or a slurm error code
 */
extern int slurm_checkpoint_requeue PARAMS((uint32_t job_id, uint16_t max_wait,
					    char *image_dir));

/*
 * slurm_checkpoint_vacate - initiate a checkpoint requests for some job step.
 *	the job will terminate after the checkpoint operation completes
 * IN job_id  - job on which to perform operation
 * IN step_id - job step on which to perform operation
 * IN max_wait - maximum wait for operation to complete, in seconds
 * IN image_dir - directory to store image files
 * RET 0 or a slurm error code
 */
extern int slurm_checkpoint_vacate PARAMS((uint32_t job_id,
					   uint32_t step_id,
					   uint16_t max_wait,
					   char *image_dir));

/*
 * slurm_checkpoint_restart - restart execution of a checkpointed job step.
 * IN job_id  - job on which to perform operation
 * IN step_id - job step on which to perform operation
 * IN stick - stick to nodes previously running on
 * IN image_dir - directory to find checkpoint image files
 * RET 0 or a slurm error code
 */
extern int slurm_checkpoint_restart PARAMS((uint32_t job_id,
					    uint32_t step_id,
					    uint16_t stick,
					    char *image_dir));

/*
 * slurm_checkpoint_complete - note the completion of a job step's checkpoint
 *	operation.
 * IN job_id  - job on which to perform operation
 * IN step_id - job step on which to perform operation
 * IN begin_time - time at which checkpoint began
 * IN error_code - error code, highest value for all complete calls is preserved
 * IN error_msg - error message, preserved for highest error_code
 * RET 0 or a slurm error code
 */
extern int slurm_checkpoint_complete PARAMS((uint32_t job_id,
					     uint32_t step_id,
					     time_t begin_time,
					     uint32_t error_code,
					     char *error_msg));

/*
 * slurm_checkpoint_task_complete - note the completion of a task's checkpoint
 *	operation.
 * IN job_id  - job on which to perform operation
 * IN step_id - job step on which to perform operation
 * IN task_id - task which completed the operation
 * IN begin_time - time at which checkpoint began
 * IN error_code - error code, highest value for all complete calls is preserved
 * IN error_msg - error message, preserved for highest error_code
 * RET 0 or a slurm error code
 */
extern int slurm_checkpoint_task_complete PARAMS((uint32_t job_id,
						  uint32_t step_id,
						  uint32_t task_id,
						  time_t begin_time,
						  uint32_t error_code,
						  char *error_msg));

/*
 * slurm_checkpoint_error - gather error information for the last checkpoint
 *	operation for some job step
 * IN job_id  - job on which to perform operation
 * IN step_id - job step on which to perform operation
 * OUT error_code - error number associated with the last checkpoint operation,
 *	this value is dependent upon the checkpoint plugin used and may be
 *	completely unrelated to slurm error codes, the highest value for all
 *	complete calls is preserved
 * OUT error_msg - error message, preserved for highest error_code, value
 *	must be freed by the caller to prevent memory leak
 * RET 0 or a slurm error code
 */
extern int slurm_checkpoint_error PARAMS((uint32_t job_id, uint32_t step_id,
					  uint32_t *error_code,
					  char **error_msg));

/*
 * slurm_checkpoint_tasks - send checkpoint request to tasks of
 *     specified step
 * IN job_id: job ID of step
 * IN step_id: step ID of step
 * IN image_dir: location to store ckpt images. parameter to plugin.
 * IN max_wait: seconds to wait for the operation to complete
 * IN nodelist: nodes to send the request
 * RET: 0 on success, non-zero on failure with errno set
 */
extern int slurm_checkpoint_tasks PARAMS((uint32_t job_id, uint16_t step_id,
					  time_t begin_time, char *image_dir,
					  uint16_t max_wait, char *nodelist));


/*****************************************************************************\
 *      SLURM TRIGGER FUNCTIONS
\*****************************************************************************/

/*
 * slurm_set_trigger - Set an event trigger
 * RET 0 or a slurm error code
 */
extern int slurm_set_trigger PARAMS((trigger_info_t * trigger_set));

/*
 * slurm_clear_trigger - Clear (remove) an existing event trigger
 * RET 0 or a slurm error code
 */
extern int slurm_clear_trigger PARAMS((trigger_info_t * trigger_clear));

/*
 * slurm_get_triggers - Get all event trigger information
 * Use slurm_free_trigger_msg() to free the memory allocated by this function
 * RET 0 or a slurm error code
 */
extern int slurm_get_triggers PARAMS((trigger_info_msg_t ** trigger_get));

/*
 * slurm_pull_trigger - Pull an event trigger
 * RET 0 or a slurm error code
 */
extern int slurm_pull_trigger PARAMS((trigger_info_t * trigger_pull));

/*
 * slurm_free_trigger_msg - Free data structure returned by
 * slurm_get_triggers()
 */
extern void slurm_free_trigger_msg PARAMS((trigger_info_msg_t * trigger_free));

/*
 * slurm_init_trigger_msg - initialize trigger clear/update message
 * OUT trigger_info_msg - user defined trigger descriptor
 */
void slurm_init_trigger_msg PARAMS((trigger_info_t *trigger_info_msg));

END_C_DECLS

#endif
