#ifndef _LINUX_LIBGTOP_H
#define _LINUX_LIBGTOP_H 1

#include <linux/tasks.h>

enum {
    LIBGTOP_VERSION = 1,
    LIBGTOP_PROC,
    LIBGTOP_UPDATE_EXPENSIVE,
    LIBGTOP_STAT,
    LIBGTOP_MEM,
    LIBGTOP_SWAP,
    LIBGTOP_PROCLIST,
    LIBGTOP_PROC_STATE
};

enum {
    LIBGTOP_PROCLIST_ALL = 0,
    LIBGTOP_PROCLIST_PID,
    LIBGTOP_PROCLIST_PGRP,
    LIBGTOP_PROCLIST_SESSION,
    LIBGTOP_PROCLIST_TTY,
    LIBGTOP_PROCLIST_UID,
    LIBGTOP_PROCLIST_RUID
};

#define LIBGTOP_PROCLIST_MASK		15	

#define LIBGTOP_EXCLUDE_IDLE		0x1000
#define LIBGTOP_EXCLUDE_SYSTEM		0x2000
#define LIBGTOP_EXCLUDE_NOTTY		0x4000

typedef struct libgtop_stat libgtop_stat_t;

typedef struct libgtop_cpu libgtop_cpu_t;
typedef struct libgtop_mem libgtop_mem_t;
typedef struct libgtop_swap libgtop_swap_t;
typedef struct libgtop_proclist libgtop_proclist_t;

typedef struct libgtop_proc_state libgtop_proc_state_t;

struct libgtop_cpu
{
    unsigned long total;		/* Total CPU Time		*/
    unsigned long user;			/* CPU Time in User Mode	*/
    unsigned long nice;			/* CPU Time in User Mode (nice)	*/
    unsigned long sys;			/* CPU Time in System Mode	*/
    unsigned long idle;			/* CPU Time in the Idle Task	*/
};

struct libgtop_mem
{
    unsigned long totalram;		/* Total usable main memory size */
    unsigned long freeram;		/* Available memory size */
    unsigned long sharedram;		/* Amount of shared memory */
    unsigned long bufferram;		/* Memory used by buffers */
    unsigned long cachedram;
};

struct libgtop_swap
{
    unsigned long totalswap;		/* Total swap space size */
    unsigned long freeswap;		/* swap space still available */
};

struct libgtop_proclist
{
    int count;
    int nr_running, nr_tasks, last_pid;
    unsigned pids [NR_TASKS];
};

struct libgtop_stat
{
    int ncpu;				/* Number of CPUs		*/
    unsigned long frequency;		/* Tick frequency (HZ)		*/
    libgtop_cpu_t cpu;			/* CPU statistics		*/
    libgtop_cpu_t xcpu [NR_CPUS];	/* SMP per-CPU statistics	*/
    double loadavg [3];			/* Load average			*/
    unsigned long total_forks;		/* Total # of forks		*/
    unsigned int context_swtch;		/* Total # of context switches	*/
    unsigned long boot_time;		/* Boot time (seconds s. epoch)	*/
    unsigned int pgpgin, pgpgout;	/* # of pages paged in/out	*/
    unsigned int pswpin, pswpout;	/* # of swap pgs brought in/out	*/
};

struct libgtop_proc_state
{
    long state;
    unsigned long flags;
    char comm [16];
    int uid, euid, suid, fsuid;
    int gid, egid, sgid, fsgid;
    int pid, pgrp, ppid;
    int session;
    unsigned int tty;
    int tpgid;
    long priority;
    long counter;
    long def_priority;
    long utime, stime, cutime, cstime, start_time;
    unsigned long policy, rt_priority;
    unsigned long it_real_value, it_prof_value, it_virt_value;
    unsigned long it_real_incr, it_prof_incr, it_virt_incr;
    unsigned long keip, kesp;
    unsigned long min_flt, maj_flt, cmin_flt, cmaj_flt;
    unsigned long nswap, cnswap;
};

#endif
