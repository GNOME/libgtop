#ifndef _LINUX_TABLE_H
#define _LINUX_TABLE_H
#include <linux/types.h>

#define TABLE_VERSION	0
#define TABLE_CPU	1
#define TABLE_MEM	2
#define TABLE_SWAP	3
#define TABLE_LOADAVG	4
#define TABLE_UPTIME	5

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
};

/* Load average */

struct table_loadavg
{
	unsigned long loadavg [3];
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

/* Union */

union table
{
	struct table_cpu cpu;
	struct table_mem mem;
	struct table_swap swap;
	struct table_loadavg loadavg;
	struct table_uptime uptime;
};

#ifdef __KERNEL__

#endif /* __KERNEL__ */

#endif /* _LINUX_IPC_H */


