#ifndef _LINUX_LIBGTOP_H
#define _LINUX_LIBGTOP_H 1

#include <linux/tasks.h>

enum {
    LIBGTOP_VERSION = 1,
    LIBGTOP_UPDATE_EXPENSIVE,
    LIBGTOP_STAT = 101,
    LIBGTOP_MEM,
    LIBGTOP_SWAP,
    LIBGTOP_PROCLIST = 201,
    LIBGTOP_PROC_STATE = 211,
    LIBGTOP_PROC_KERNEL,
    LIBGTOP_PROC_SEGMENT,
    LIBGTOP_PROC_MEM,
    LIBGTOP_PROC_SIGNAL,
    LIBGTOP_PROC_ARGS = 251,
    LIBGTOP_PROC_MAPS,
    LIBGTOP_NETLOAD = 301
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

#define LIBGTOP_NSIG			4

#define LIBGTOP_PROCLIST_MASK		15
#define LIBGTOP_MAX_GROUPS		32

#define LIBGTOP_EXCLUDE_IDLE		0x1000
#define LIBGTOP_EXCLUDE_SYSTEM		0x2000
#define LIBGTOP_EXCLUDE_NOTTY		0x4000

#define LIBGTOP_TASK_RUNNING		1
#define LIBGTOP_TASK_INTERRUPTIBLE	2
#define LIBGTOP_TASK_UNINTERRUPTIBLE	4
#define LIBGTOP_TASK_ZOMBIE		8
#define LIBGTOP_TASK_STOPPED		16
#define LIBGTOP_TASK_SWAPPING		32

#define LIBGTOP_VM_READ		0x0001	/* currently active flags */
#define LIBGTOP_VM_WRITE	0x0002
#define LIBGTOP_VM_EXEC		0x0004
#define LIBGTOP_VM_SHARED	0x0008

#define LIBGTOP_VM_MAYREAD	0x0010	/* limits for mprotect() etc */
#define LIBGTOP_VM_MAYWRITE	0x0020
#define LIBGTOP_VM_MAYEXEC	0x0040
#define LIBGTOP_VM_MAYSHARE	0x0080

#define LIBGTOP_VM_GROWSDOWN	0x0100	/* general info on the segment */
#define LIBGTOP_VM_GROWSUP	0x0200
#define LIBGTOP_VM_SHM		0x0400	/* shared memory area, don't swap out */
#define LIBGTOP_VM_DENYWRITE	0x0800	/* ETXTBSY on write attempts.. */

#define LIBGTOP_VM_EXECUTABLE	0x1000
#define LIBGTOP_VM_LOCKED	0x2000
#define LIBGTOP_VM_IO		0x4000  /* Memory mapped I/O or similar */

#define LIBGTOP_MAP_PATH_LEN	(PAGE_SIZE - sizeof (libgtop_proc_maps_header_t))

#ifndef min
#define min(a,b) ((a < b) ? a : b)
#endif

typedef struct libgtop_stat libgtop_stat_t;

typedef struct libgtop_cpu libgtop_cpu_t;
typedef struct libgtop_mem libgtop_mem_t;
typedef struct libgtop_swap libgtop_swap_t;
typedef struct libgtop_proclist libgtop_proclist_t;

typedef struct libgtop_proc_state libgtop_proc_state_t;
typedef struct libgtop_proc_kernel libgtop_proc_kernel_t;
typedef struct libgtop_proc_segment libgtop_proc_segment_t;
typedef struct libgtop_proc_mem libgtop_proc_mem_t;
typedef struct libgtop_proc_signal libgtop_proc_signal_t;

typedef struct libgtop_proc_maps_header libgtop_proc_maps_header_t;
typedef struct libgtop_proc_maps libgtop_proc_maps_t;

typedef struct libgtop_netload libgtop_netload_t;

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
    long priority, counter, def_priority;
    long utime, stime, cutime, cstime, start_time;
    long per_cpu_utime [NR_CPUS], per_cpu_stime [NR_CPUS];
    int has_cpu, processor, last_processor;

    unsigned long context;
    unsigned long start_code, end_code, start_data, end_data;
    unsigned long start_brk, brk, start_stack, start_mmap;
    unsigned long arg_start, arg_end, env_start, env_end;
    unsigned long rss, rlim, total_vm, locked_vm;

    unsigned long policy, rt_priority;
    unsigned long it_real_value, it_prof_value, it_virt_value;
    unsigned long it_real_incr, it_prof_incr, it_virt_incr;

    unsigned long keip, kesp;
    unsigned long min_flt, maj_flt, cmin_flt, cmaj_flt;
    unsigned long nswap, cnswap;

    int ngroups, groups [LIBGTOP_MAX_GROUPS];
};

struct libgtop_proc_kernel
{
    unsigned long wchan;
};

struct libgtop_proc_segment
{
    unsigned long vsize, data, exec, stack, lib;
};

struct libgtop_proc_mem
{
    libgtop_proc_segment_t segment;
    int size, resident, share, trs, lrs, drs, dt;
    unsigned long rss, rlim;
};

struct libgtop_proc_signal
{
    unsigned long signal [LIBGTOP_NSIG];
    unsigned long blocked [LIBGTOP_NSIG];
    unsigned long ignore [LIBGTOP_NSIG];
    unsigned long catch [LIBGTOP_NSIG];
};

struct libgtop_proc_maps_header
{
    unsigned long start, end, offset, perm;
    off_t filename_offset;
    ino_t inode;
    dev_t device;
} __attribute__ ((aligned (64)));

struct libgtop_proc_maps
{
    libgtop_proc_maps_header_t header;
    char filename [LIBGTOP_MAP_PATH_LEN];
};

struct libgtop_netload
{
    unsigned long rx_packets;		/* total packets received	*/
    unsigned long tx_packets;		/* total packets transmitted	*/
    unsigned long rx_bytes;		/* total bytes received 	*/
    unsigned long tx_bytes;		/* total bytes transmitted	*/
    unsigned long rx_errors;		/* bad packets received		*/
    unsigned long tx_errors;		/* packet transmit problems	*/
    unsigned long rx_dropped;		/* no space in linux buffers	*/
    unsigned long tx_dropped;		/* no space available in linux	*/
    unsigned long multicast;		/* multicast packets received	*/
    unsigned long collisions;
    
    /* detailed rx_errors: */
    unsigned long rx_length_errors;
    unsigned long rx_over_errors;	/* receiver ring buff overflow	*/
    unsigned long rx_crc_errors;	/* recved pkt with crc error	*/
    unsigned long rx_frame_errors;	/* recv'd frame alignment error */
    unsigned long rx_fifo_errors;	/* recv'r fifo overrun		*/
    unsigned long rx_missed_errors;	/* receiver missed packet	*/
    
    /* detailed tx_errors */
    unsigned long tx_aborted_errors;
    unsigned long tx_carrier_errors;
    unsigned long tx_fifo_errors;
    unsigned long tx_heartbeat_errors;
    unsigned long tx_window_errors;
    
    /* for cslip etc */
    unsigned long rx_compressed;
    unsigned long tx_compressed;
};

#endif
