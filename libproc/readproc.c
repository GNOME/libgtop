/*
 * New Interface to Process Table -- PROCTAB Stream (a la Directory streams)
 * Copyright(C) 1996. Charles L. Blake.
 */
#include "proc/version.h"
#include "proc/readproc.h"
#include "proc/devname.h"
#include "proc/ps.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/stat.h>

#define Do(x) (flags & PROC_ ## x)	/* convenient shorthand */

/* initiate a process table scan
 */
PROCTAB* openproc(int flags, ...) {
    va_list ap;
    PROCTAB* PT = xmalloc(sizeof(PROCTAB));

    if (!Do(PID) && !(PT->procfs = opendir("/proc")))
    	return NULL;
    PT->flags = flags;
    va_start(ap, flags);		/*  Init args list */
    if (Do(PID))
    	PT->pids = va_arg(ap, pid_t*);
    else if (Do(TTY))
    	PT->ttys = va_arg(ap, dev_t*);
    else if (Do(UID)) {
    	PT->uids = va_arg(ap, uid_t*);
	PT->nuid = va_arg(ap, int);
    } else if (Do(STAT))
    	PT->stats = va_arg(ap, char*);
    va_end(ap);				/*  Clean up args list */
    if (Do(ANYTTY) && Do(TTY))
	PT->flags = PT->flags & ~PROC_TTY; /* turn off TTY flag */
    return PT;
}

/* terminate a process table scan
 */
void closeproc(PROCTAB* PT) {
    if (PT->procfs) closedir(PT->procfs);
    if (PT)         free(PT);
}

/* deallocate the space allocated by readproc if the passed rbuf was NULL
 */
void freeproc(proc_t* p) {
    if (!p)	/* in case p is NULL */
	return;
    /* ptrs are after strings to avoid copying memory when building them. */
    /* so free is called on the address of the address of strvec[0]. */
    if (p->cmdline)
	free((void*)*p->cmdline);
    if (p->environ)
	free((void*)*p->environ);
    free(p);
}

/* stat2proc() makes sure it can handle arbitrary executable file basenames
   for `cmd', i.e. those with embedded whitespace or embedded ')'s.  Such names
   confuse %s (see scanf(3)), so the string is split and %39c is used instead.
   (except for embedded ')' "(%[^)]c)" would work.
*/
void stat2proc(char* S, proc_t* P) {
    char* tmp = strrchr(S, ')');	/* split into "PID (cmd" and "<rest>" */
    *tmp = '\0';			/* replace trailing ')' with NUL */
    /* parse these two strings separately, skipping the leading "(". */
    memset(P->cmd, 0, sizeof P->cmd);	/* clear even though *P xcalloc'd ?! */
    sscanf(S, "%d (%39c", &P->pid, P->cmd);
    sscanf(tmp + 2,			/* skip space after ')' too */
	   "%c %d %d %d %d %d %lu %lu %lu %lu %lu %ld %ld %ld %ld %d "
	   "%d %lu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %LX %LX %LX %LX %lu",
	   &P->state, &P->ppid, &P->pgrp, &P->session, &P->tty, &P->tpgid,
	   &P->flags, &P->min_flt, &P->cmin_flt, &P->maj_flt, &P->cmaj_flt,
	   &P->utime, &P->stime, &P->cutime, &P->cstime, &P->priority, &P->nice,
	   &P->timeout, &P->it_real_value, &P->start_time, &P->vsize, &P->rss,
	   &P->rss_rlim, &P->start_code, &P->end_code, &P->start_stack,
	   &P->kstk_esp, &P->kstk_eip, &P->signal, &P->blocked, &P->sigignore,
	   &P->sigcatch, &P->wchan);
    if (P->tty == 0)
	P->tty = -1;  /* the old notty val, update elsewhere bef. moving to 0 */
    if (linux_version_code < LINUX_VERSION(1,3,39)) {
	P->priority = 2*15 - P->priority;	/* map old meanings to new */
	P->nice = 15 - P->nice;
    }
    if (linux_version_code < LINUX_VERSION(1,1,30) && P->tty != -1)
	P->tty = 4*0x100 + P->tty;		/* when tty wasn't full devno */
}

void statm2proc(char* s, proc_t* P) {
    sscanf(s, "%ld %ld %ld %ld %ld %ld %ld",
	   &P->size, &P->resident, &P->share,
	   &P->trs, &P->lrs, &P->drs, &P->dt);
}

void nulls2sep(char* str, int len, char sep) {
    int i;
    for (i = 0; i < len; i++)
    	if (str[i] == 0)
    	    str[i] = sep;
}

int file2str(char *directory, char *what, char *ret, int cap) {
    static char filename[80];
    int fd, num_read;

    sprintf(filename, "%s/%s", directory, what);
    if ( (fd       = open(filename, O_RDONLY, 0)) == -1 ) return -1;
    if ( (num_read = read(fd, ret, cap - 1))      <= 0 ) return -1;
    ret[num_read] = 0;
    close(fd);
    return num_read;
}

char** file2strvec(char* directory, char* what) {
    char buf[2048];	/* read buf bytes at a time */
    char *p, *rbuf = 0, *endbuf, **q, **ret;
    int fd, tot = 0, n, c, end_of_file = 0;
    int align;

    sprintf(buf, "%s/%s", directory, what);
    if ( (fd = open(buf, O_RDONLY, 0) ) == -1 ) return NULL;

    /* read whole file into a memory buffer, allocating as we go */
    while ((n = read(fd, buf, sizeof buf - 1)) > 0) {
	if (n < sizeof buf - 1)
	    end_of_file = 1;
	if (n == 0 && rbuf == 0)
	    return NULL;	/* process died between our open and read */
	if (n < 0) {
	    if (rbuf)
		free(rbuf);
	    return NULL;	/* read error */
	}
	if (end_of_file && buf[n-1])		/* last read char not null */
	    buf[n++] = '\0';			/* so append null-terminator */
	rbuf = xrealloc(rbuf, tot + n);		/* allocate more memory */
	memcpy(rbuf + tot, buf, n);		/* copy buffer into it */
	tot += n;				/* increment total byte ctr */
	if (end_of_file)
	    break;
    }
    close(fd);
    if (n <= 0 && !end_of_file) {
	if (rbuf) free(rbuf);
	return NULL;		/* read error */
    }
    endbuf = rbuf + tot;			/* count space for pointers */
    align = (sizeof(char*)-1) - ((tot + sizeof(char*)-1) & (sizeof(char*)-1));
    for (c = 0, p = rbuf; p < endbuf; p++)
    	if (!*p)
	    c += sizeof(char*);
    c += sizeof(char*);				/* one extra for NULL term */

    rbuf = xrealloc(rbuf, tot + c + align);	/* make room for ptrs AT END */
    endbuf = rbuf + tot;			/* addr just past data buf */
    q = ret = (char**) (endbuf+align);		/* ==> free(*ret) to dealloc */
    *q++ = p = rbuf;				/* point ptrs to the strings */
    endbuf--;					/* do not traverse final NUL */
    while (++p < endbuf) 
    	if (!*p)				/* NUL char implies that */
	    *q++ = p+1;				/* next string -> next char */

    *q = 0;					/* null ptr list terminator */
    return ret;
}


/* These are some nice GNU C expression subscope "inline" functions.
   The can be used with arbitrary types and evaluate their arguments
   exactly once.
*/

/* Test if item X of type T is present in the 0 terminated list L */
#   define XinL(T, X, L) ( {			\
	    T  x = (X), *l = (L);		\
	    while (*l && *l != x) l++;		\
	    *l == x;				\
	} )

/* Test if item X of type T is present in the list L of length N */
#   define XinLN(T, X, L, N) ( {		\
	    T x = (X), *l = (L);		\
	    int i = 0, n = (N);			\
	    while (i < n && l[i] != x) i++;	\
	    i < n && l[i] == x;			\
	} )

/* readproc: return a pointer to a proc_t filled with requested info about the
 * next process available matching the restriction set.  If no more such
 * processes are available, return a null pointer (boolean false).  Use the
 * passed buffer instead of allocating space if it is non-NULL.  */

/* This is optimized so that if a PID list is given, only those files are
 * searched for in /proc.  If other lists are given in addition to the PID list,
 * the same logic can follow through as for the no-PID list case.  This is
 * fairly complex, but it does try to not to do any unnecessary work.
 * Unfortunately, the reverse filtering option in which any PID *except* the
 * ones listed is pursued.
 */
#define flags (PT->flags)
proc_t* readproc(PROCTAB* PT, proc_t* rbuf) {
    static struct direct *ent;		/* dirent handle */
    static struct stat sb;		/* stat buffer */
    static char path[32], sbuf[256];	/* bufs for stat,statm */
    int allocated = 0, matched = 0;	/* flags */
    proc_t *p = NULL;

    /* loop until a proc matching restrictions is found or no more processes */
    /* I know this could be a while loop -- this way is easier to indent ;-) */
next_proc:				/* get next PID for consideration */
    if (Do(PID)) {
	if (!*PT->pids)			/* set to next item in pids */
	    return NULL;
	sprintf(path, "/proc/%d", *(PT->pids)++);
	matched = 1;
    } else {					/* get next numeric /proc ent */
	while ((ent = readdir(PT->procfs)) &&
	       (*ent->d_name < '0' || *ent->d_name > '9'))
	    ;
	if (!ent || !ent->d_name)
	    return NULL;
	sprintf(path, "/proc/%s", ent->d_name);
    }
    if (stat(path, &sb) == -1)		/* no such dirent (anymore) */
	goto next_proc;
    if (Do(UID) && !XinLN(uid_t, sb.st_uid, PT->uids, PT->nuid))
	goto next_proc;			/* not one of the requested uids */

    if (!allocated) {				 /* assign mem for return buf */
	p = rbuf ? rbuf : xcalloc(p, sizeof *p); /* passed buf or alloced mem */
	allocated = 1;				 /* remember space is set up */
    }
    p->uid = sb.st_uid;			/* need a way to get real uid */

    if ((file2str(path, "stat", sbuf, sizeof sbuf)) == -1)
	goto next_proc;			/* error reading /proc/#/stat */
    stat2proc(sbuf, p);				/* parse /proc/#/stat */

    if (!matched && Do(TTY) && !XinL(dev_t, p->tty, PT->ttys))
	goto next_proc;			/* not one of the requested ttys */

    if (!matched && Do(ANYTTY) && p->tty == -1)
	goto next_proc;			/* no controlling terminal */

    if (!matched && Do(STAT) && !strchr(PT->stats,p->state))
	goto next_proc;			/* not one of the requested states */

    if (Do(FILLMEM)) {				/* read, parse /proc/#/statm */
	if ((file2str(path, "statm", sbuf, sizeof sbuf)) != -1 )
	    statm2proc(sbuf, p);		/* ignore statm errors here */
    }						/* statm fields just zero */

    /* some number->text resolving which is time consuming */
    if (Do(FILLTTY))
	dev_to_tty(p->ttyc, p->tty);
    if (Do(FILLUSR))
	strncpy(p->user, user_from_uid(p->uid), sizeof p->user);

    if (Do(FILLCMD))				/* read+parse /proc/#/cmdline */
	p->cmdline = file2strvec(path, "cmdline");
    if (Do(FILLENV))				/* read+parse /proc/#/environ */
	p->environ = file2strvec(path, "environ");
    
    if (p->state == 'Z')			/* fixup cmd for zombies */
	strncat(p->cmd," <zombie>", sizeof p->cmd);

    return p;
}
#undef flags

/* Convenient wrapper around openproc and readproc to slurp in the whole process
 * tree subset satisfying the constraints of flags and the optional PID list.
 * Free allocated memory with freeproctree().  The tree structure is a classic
 * left-list children + right-list siblings.  The algorithm is a two-pass of the
 * process table.  Since most process trees will have children with strictly
 * increasing PIDs, most of the structure will be picked up in the first pass.
 * The second loop then cleans up any nodes which turn out to have preceeded
 * their parent in /proc order.
 */

/* Traverse tree 't' breadth-first looking for a process with pid p */
proc_t* LookupPID(proc_t* t, pid_t p) {
    proc_t* tmp = NULL;
    if (!t)
	return NULL;
    if (t->pid == p)				/* look here/terminate recursion */
	return t;
    if ((tmp = LookupPID(t->l, p)))		/* recurse over children */
	return tmp;
    for (; t; t=t->r)				/* recurse over siblings */
	if ((tmp = LookupPID(tmp, p)))
	    return tmp;
    return NULL;
}

proc_t* readproctree(int flags, ...) {
    static proc_t tree;
    PROCTAB* PT = NULL;
    proc_t *node, *tmp=NULL, *tmp2=NULL;
    va_list ap;
    
    /* pass through apropriate arguments to openproc */
    va_start(ap, flags);
    if (Do(UID)) {
	/* temporary variables to ensure that va_arg() instances
	 * are called in the right order
	 */
	uid_t* u;
	int i;

	u = va_arg(ap, uid_t*);
	i = va_arg(ap, int);
	PT = openproc(flags, u, i);
    }
    else if (Do(PID) || Do(TTY) || Do(STAT))
	PT = openproc(flags, va_arg(ap, void*));
    else
	PT = openproc(flags);
    va_end(ap);

    /* first pass: build tree, putting orphans on the first level */
    tree.l = tree.r = NULL;
    while ((node = readproc(PT,0)))
	if ((tmp = LookupPID(&tree, node->ppid))) {
	    node->r = tmp->l->r;	/* node --> left list of parent */
	    tmp->l->r = node;
	} else {
	    node->r = tree.r;		/* node --> right list of 'tree' */
	    tree.r = node;
	}
    /* second pass: scan tree for PPIDs of level-1 nodes moving links as necessary */
    for (node = &tree; node; node = node->r)
	if ((tmp = LookupPID(&tree, node->r->ppid))) {
	    tmp2 = node->r;			/* unlink from right list of 'tree' */
	    node->r = node->r->r;
	    tmp2->r = tmp->l->r;		/* insert as child of found node */
	    tmp->l->r = node;
	}
    closeproc(PT);
    return &tree;
}

/* Convenient wrapper around openproc and readproc to slurp in the whole process
 * table subset satisfying the constraints of flags and the optional PID list.
 * Free allocated memory with freeproctab().  Access via tab[N]->member.  The
 * pointer list is NULL terminated.
 */
proc_t** readproctab(int flags, ...) {
    PROCTAB* PT = NULL;
    proc_t** tab = NULL;
    int n = 0;
    va_list ap;

    va_start(ap, flags);		/* pass through args to openproc */
    if (Do(UID)) {
	/* temporary variables to ensure that va_arg() instances
	 * are called in the right order
	 */
	uid_t* u;
	int i;

	u = va_arg(ap, uid_t*);
	i = va_arg(ap, int);
	PT = openproc(flags, u, i);
    }
    else if (Do(PID) || Do(TTY) || Do(STAT))
	PT = openproc(flags, va_arg(ap, void*)); /* assume ptr sizes same */
    else
	PT = openproc(flags);
    va_end(ap);
    do {					/* read table: */
	tab = xrealloc(tab, (n+1)*sizeof(proc_t*));/* realloc as we go, using */
	tab[n] = readproc(PT, NULL);		  /* final null to terminate */
    } while (tab[n++]);				  /* stop when NULL reached */
    closeproc(PT);
    return tab;
}

/* deallocate a table of pointers to proc structures
 */
void freeproctab(proc_t** tab) {
    proc_t** p;
    for(p = tab; *p; p++)
	freeproc(*p);
    free(tab);
}
