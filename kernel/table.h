#ifndef _LINUX_TABLE_H
#define _LINUX_TABLE_H

#ifdef _KERNEL
#include <linux/types.h>
#else
#define NR_TASKS		512
#endif

#define TABLE_VERSION		0
#define TABLE_CPU		1
#define TABLE_MEM		2
#define TABLE_SWAP		3
#define TABLE_LOADAVG		4
#define TABLE_UPTIME		5
#define TABLE_PROCLIST		6
#define TABLE_PROC_UID		7
#define TABLE_PROC_MEM		8
#define TABLE_PROC_SEGMENT	9
#define TABLE_PROC_TIME		10
#define TABLE_PROC_STATE	11
#define TABLE_PROC_SIGNAL	12
#define TABLE_PROC_KERNEL	13

/* CPU Usage (in jiffies = 1/100th seconds) */

struct table_cpu
{
	unsigned long total;		/* Total CPU Time		*/
	unsigned long user;		/* CPU Time in User Mode	*/
	unsigned long nice;		/* CPU Time in User Mode (nice)	*/
	unsigned long sys;		/* CPU Time in System Mode	*/
	unsigned long idle;		/* CPU Time in the Idle Task	*/
	unsigned long frequency;	/* Tick frequency		*/
};

/* Memory Usage (in bytes) */

struct table_mem
{
	unsigned long total;		/* Total physical memory	*/
	unsigned long used;		/* Used memory size		*/
	unsigned long free;		/* Free memory size		*/
	unsigned long shared;		/* Shared memory size		*/
	unsigned long buffer;		/* Size of buffers		*/
	unsigned long cached;		/* Size of cached memory       	*/
};

/* Swap Space (in bytes) */

struct table_swap
{
	unsigned long total;		/* Total swap space		*/
	unsigned long used;		/* Used swap space		*/
	unsigned long free;		/* Free swap space		*/
	unsigned long pagein;		/* Total # of pages swapped in	*/
	unsigned long pageout;		/* Total # of pages swapped out	*/
};

/* Load average */

struct table_loadavg
{
	double loadavg [3];
	unsigned nr_running;
	unsigned nr_tasks;
	unsigned last_pid;
};

/* Uptime */

struct table_uptime
{
	unsigned long uptime;
	unsigned long idle;
};

/* Process list. */

struct table_proclist
{
	int nr_running, nr_tasks, last_pid;
	unsigned pids [NR_TASKS];
};

/* Information about processes. */

struct table_proc_state
{
	long state;
	unsigned long flags;
	char comm[16];
};

struct table_proc_uid
{
	int uid, euid, suid, fsuid;
	int gid, egid, sgid, fsgid;
	int pid, pgrp, ppid;
	int session;
	unsigned int tty;
	int tpgid;
	long priority;
	long counter;
	long def_priority;
};

struct table_proc_mem
{
	unsigned long context;
	unsigned long start_code, end_code, start_data, end_data;
	unsigned long start_brk, brk, start_stack, start_mmap;
	unsigned long arg_start, arg_end, env_start, env_end;
	unsigned long rss, rlim, total_vm, locked_vm;
};

struct table_proc_segment
{
	unsigned long vsize;
	unsigned long size, resident, shared;
	unsigned long trs, lrs, drs, srs, dt;
};

struct table_proc_time
{
	long utime, stime, cutime, cstime, start_time;
	unsigned long timeout, policy, rt_priority;
	unsigned long it_real_value, it_prof_value, it_virt_value;
	unsigned long it_real_incr, it_prof_incr, it_virt_incr;
};

struct table_proc_signal
{
	unsigned long long signal,
		blocked,	/* bitmap of masked signals */
		ignored,	/* mask of ignored signals */
		caught;		/* mask of caught signals */
};

struct table_proc_kernel
{
	unsigned long keip, kesp, wchan;
	unsigned long min_flt, maj_flt, cmin_flt, cmaj_flt;
	unsigned long nswap, cnswap;
};

/* Union */

union table
{
	struct table_cpu cpu;
	struct table_mem mem;
	struct table_swap swap;
	struct table_loadavg loadavg;
	struct table_uptime uptime;
	struct table_proclist proclist;
	struct table_proc_uid proc_uid;
	struct table_proc_mem proc_mem;
	struct table_proc_segment proc_segment;
	struct table_proc_time proc_time;
	struct table_proc_state proc_state;	
	struct table_proc_signal proc_signal;
	struct table_proc_kernel proc_kernel;
};

#endif /* _LINUX_IPC_H */


