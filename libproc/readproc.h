/*
 * New Interface to Process Table -- PROCTAB Stream (a la Directory streams)
 * Copyright(C) 1996. Charles L. Blake.
 */

/* Basic data structure which holds all information we can get about a process.
 * (unless otherwise specified, fields are read from /proc/#/stat)
 */
typedef struct proc_s {
    char
    	user[10],	/* user name corresponding to owner of process */
    	cmd[40],	/* basename of executable file in call to exec(2) */
    	state,		/* single-char code for process state (S=sleeping) */
    	ttyc[5],	/* string representation of controlling tty device */
	**environ,	/* environment string vector (/proc/#/environ) */
	**cmdline;	/* command line string vector (/proc/#/cmdline) */
    int
    	uid,		/* user id */
    	pid,		/* process id */
    	ppid,		/* pid of parent process */
	pgrp,		/* process group id */
	session,	/* session id */
	tty,		/* full device number of controlling terminal */
	tpgid,		/* terminal process group id */
	priority,	/* kernel scheduling priority */
	nice;		/* standard unix nice level of process */
    long long
	signal,		/* mask of pending signals */
	blocked,	/* mask of blocked signals */
	sigignore,	/* mask of ignored signals */
	sigcatch;	/* mask of caught  signals */
    long
	start_time,	/* start time of process -- seconds since 1-1-70 */
	utime,		/* user-mode CPU time accumulated by process */
	stime,		/* kernel-mode CPU time accumulated by process */
	cutime,		/* cumulative utime of process and reaped children */
	cstime,		/* cumulative stime of process and reaped children */
    /* the next 7 members come from /proc/#/statm */
	size,		/* total # of pages of memory */
	resident,	/* number of resident set (non-swapped) pages (4k) */
	share,		/* number of pages of shared (mmap'd) memory */
	trs,		/* text resident set size */
	lrs,		/* shared-lib resident set size */
	drs,		/* data resident set size */
	dt;		/* dirty pages */
    unsigned
        pcpu;           /* %CPU usage (is not filled in by readproc!!!) */
    unsigned long
	vsize,		/* number of pages of virtual memory ... */
	rss,		/* resident set size from /proc/#/stat */
	rss_rlim,	/* resident set size ... ? */
	timeout,	/* ? */
	it_real_value,	/* ? */
	flags,		/* kernel flags for the process */
	min_flt,	/* number of minor page faults since process start */
	maj_flt,	/* number of major page faults since process start */
	cmin_flt,	/* cumulative min_flt of process and child processes */
	cmaj_flt,	/* cumulative maj_flt of process and child processes */
	start_code,	/* address of beginning of code segment */
	end_code,	/* address of end of code segment */
	start_stack,	/* address of the bottom of stack for the process */
	kstk_esp,	/* kernel stack pointer */
	kstk_eip,	/* kernel stack pointer */
	wchan;		/* address of kernel wait channel proc is sleeping in */
    struct proc_s *l,	/* ptrs for building arbitrary linked structs */
                  *r;	/* (i.e. singly/doubly-linked lists and trees */
} proc_t;

/* PROCTAB: data structure holding the persistent information readproc needs
 * from openproc().  The setup is intentionally similar to the dirent interface
 * and other system table interfaces (utmp+wtmp come to mind).
 */
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
typedef struct {
    DIR*	procfs;
    int		flags;
    pid_t*	pids;	/* pids of the procs */
    dev_t*	ttys;	/* devnos of the cttys */
    uid_t*	uids;	/* uids of procs */
    int		nuid;	/* cannot really sentinel-terminate unsigned short[] */
    char*	stats;	/* status chars (actually output into /proc//stat) */
} PROCTAB;

/* initialize a PROCTAB structure holding needed call-to-call persistent data
 */
PROCTAB* openproc(int flags, ... /* pid_t*|uid_t*|dev_t*|char* [, int n] */ );

/* Convenient wrapper around openproc and readproc to slurp in the whole process
 * table subset satisfying the constraints of flags and the optional PID list.
 * Free allocated memory with freeproctab().  Access via tab[N]->member.  The
 * pointer list is NULL terminated.
 */
proc_t** readproctab(int flags, ... /* same as openproc */ );

/* Convenient wrapper around openproc and readproc to slurp in the whole process
 * tree subset satisfying the constraints of flags and the optional PID list.
 */
proc_t* readproctree(int flags, ... /* same as openproc */ );

/* clean-up open files, etc from the openproc()
 */
void closeproc(PROCTAB* PT);

/* retrieve the next process matching the criteria set by the openproc()
 */
proc_t* readproc(PROCTAB* PT, proc_t* return_buf);

/* deallocate space allocated by readproc
 */
void freeproc(proc_t* p);

/* deallocate space allocated by readproctab
 */
void freeproctab(proc_t** p);

/* openproc/readproctab:
 *   
 * Return PROCTAB* / *proc_t[] or NULL on error ((probably) "/proc" cannot be
 * opened.)  By default readproc will consider all processes as valid to parse
 * and return, but not actually fill in the cmdline, environ, and /proc/#/statm
 * derived memory fields.
 *
 * `flags' (a bitwise-or of PROC_* below) modifies the default behavior.  The
 * "fill" options will cause more of the proc_t to be filled in.  The "filter"
 * options all use the second argument as the pointer to a list of objects:
 * process status', process id's, user id's, and tty device numbers.  The third
 * argument is the length of the list (currently only used for lists of user
 * id's since unsigned short[] supports no convenient termination sentinel.)
 */
#define PROC_FILLMEM 0x1   /* read statm into the appropriate proc_t entries */
#define PROC_FILLCMD 0x2   /* alloc and fill in `cmdline' part of proc_t */
#define PROC_FILLENV 0x4   /* alloc and fill in `environ' part of proc_t */
#define PROC_FILLTTY 0x8   /* resolve device number -> tty name via psdevtab */
#define PROC_FILLUSR 0x10  /* resolve user id number -> user name via passwd */
                       /* consider only processes with one of the passed: */
#define PROC_PID     0x100  /* process id numbers ( 0   terminated) */
#define PROC_TTY     0x200  /* ctty device nos.   ( 0   terminated) */
#define PROC_UID     0x400  /* user id numbers    ( length needed ) */
#define PROC_STAT    0x800  /* status fields      ('\0' terminated) */
#define PROC_ANYTTY  0x1000 /* proc must have a controlling terminal */

/* utility functions which may be of general interest: */

/* slurp /proc/DIR/FILE into a single large string into the passed
   buffer.  return the number of bytes actually used.  used for stat,statm
 */
int file2str(char *dir, char *file, char *buf, int buf_size);

/* convert a file of null terminated strings into an argv-style string vector
 * which may be de-allocated with a single free() on a dereference of the return
 * value, e.g. free(*ret).  used for cmdline, environ.
 */
char** file2strvec(char* directory, char* what);

/* parse /proc/#/stat  entries in string s into a proc_t
 */
void stat2proc(char* S, proc_t*);

/* parse /proc/#/statm entries in string s into a proc_t
 */
void statm2proc(char* s, proc_t*);

/* convert a memory buffer with nulls into a single string,
   replacing the nulls with sep.  No longer used.
 */
void nulls2sep(char* str, int len, char sep);
