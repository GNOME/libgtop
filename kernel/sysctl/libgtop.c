/*
 * linux/libgtop/module.c
 * Copyright (C) 1999 Martin Baulig
 */

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/tty.h>
#include <linux/user.h>
#include <linux/a.out.h>
#include <linux/string.h>
#include <linux/mman.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/config.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/swap.h>
#include <linux/slab.h>
#include <linux/smp.h>
#include <linux/signal.h>

#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <asm/io.h>

#include <linux/sysctl.h>
#include <linux/module.h>

#include <linux/libgtop.h>

EXPORT_NO_SYMBOLS;

static int system_ctl_handler (ctl_table *table, int *name, int nlen,
			       void *oldval, size_t *oldlenp, void *newval,
			       size_t newlen, void **context);

static int proc_ctl_handler (ctl_table *table, int *name, int nlen,
			     void *oldval, size_t *oldlenp, void *newval,
			     size_t newlen, void **context);

static int libgtop_sysctl_version = 1;
static int libgtop_update_expensive = 5000;

static struct ctl_table_header *libgtop_sysctl_header = NULL;

static libgtop_stat_t libgtop_stat;
static unsigned int libgtop_mem_timestamp = 0;
static libgtop_mem_t libgtop_mem;
static unsigned int libgtop_swap_timestamp = 0;
static libgtop_swap_t libgtop_swap;
static libgtop_proclist_t libgtop_proclist;

static ctl_table libgtop_table[];
static ctl_table proc_table[];

static ctl_table libgtop_root_table[] = {
    {CTL_LIBGTOP, "libgtop", NULL, 0, 0555, libgtop_table},
    {0}
};

#ifdef MODULE
static
#endif
ctl_table libgtop_table[] = {
    {LIBGTOP_VERSION, "version", &libgtop_sysctl_version,
     sizeof (int), 0444, NULL, &proc_dointvec},
    {LIBGTOP_PROC, NULL, NULL, 0, 0555, proc_table},
    {LIBGTOP_UPDATE_EXPENSIVE, "update_expensive",
     &libgtop_update_expensive, sizeof (int), 0664, NULL, &proc_dointvec},
    {LIBGTOP_STAT, NULL, &libgtop_stat, sizeof (libgtop_stat),
     0444, NULL, NULL, &system_ctl_handler},
    {LIBGTOP_MEM, NULL, &libgtop_mem, sizeof (libgtop_mem),
     0444, NULL, NULL, &system_ctl_handler},
    {LIBGTOP_SWAP, NULL, &libgtop_swap, sizeof (libgtop_swap),
     0444, NULL, NULL, &system_ctl_handler},
    {LIBGTOP_PROCLIST, NULL, &libgtop_proclist, sizeof (libgtop_proclist),
     0444, NULL, NULL, &system_ctl_handler},
    {0}
};

static ctl_table proc_table[] = {
    {CTL_ANY, NULL, NULL, 0, 04444, NULL, NULL, &proc_ctl_handler},
    {0}
};

#ifdef MODULE
static void
libgtop_sysctl_register(void)
{
    static int initialized = 0;

    if (initialized == 1)
	return;

    libgtop_sysctl_header = register_sysctl_table(libgtop_root_table, 1);
    initialized = 1;
}

static void
libgtop_sysctl_unregister(void)
{
    unregister_sysctl_table(libgtop_sysctl_header);
}

int init_module(void)
{
    libgtop_sysctl_register();
    return 0;
}

void cleanup_module(void)
{
    libgtop_sysctl_unregister();
}

#endif /* MODULE */

static int
libgtop_sysctl (ctl_table *table, int nlen, int *name)
{
    extern unsigned long total_forks;
    int index, tindex, tty, which, arg;
    libgtop_stat_t *lstat;
    libgtop_mem_t *mem;
    libgtop_swap_t *swap;
    libgtop_proclist_t *proclist;
    struct task_struct *tsk = NULL;
    struct sysinfo i;

    switch (table->ctl_name) {
    case LIBGTOP_STAT:
	lstat = table->data;
	lstat->cpu.total = jiffies;
	lstat->cpu.user  = kstat.cpu_user;
	lstat->cpu.nice  = kstat.cpu_nice;
	lstat->cpu.sys   = kstat.cpu_system;
	lstat->cpu.idle  = jiffies*smp_num_cpus -
	    (lstat->cpu.user + lstat->cpu.nice + lstat->cpu.sys);
#ifdef __SMP__
	for (i = 0; i < smp_num_cpus; i++) {
	    lstat->xcpu[i].user = kstat.per_cpu_user[cpu_logical_map(i)];
	    lstat->xcpu[i].nice = kstat.per_cpu_nice[cpu_logical_map(i)];
	    lstat->xcpu[i].sys = kstat.per_cpu_system[cpu_logical_map(i)];
	    lstat->xcpu[i].idle = jiffies -
		(lstat->xcpu[i].user + lstat->xcpu[i].nice +
		 lstat->xcpu[i].sys);
	}

	lstat->ncpu = smp_num_cpus;
#else
	lstat->ncpu = 0;
#endif

	lstat->frequency = HZ;

	lstat->loadavg [0] = (double) avenrun [0] / (1 << FSHIFT);
	lstat->loadavg [1] = (double) avenrun [1] / (1 << FSHIFT);
	lstat->loadavg [2] = (double) avenrun [2] / (1 << FSHIFT);

	lstat->pgpgin = kstat.pgpgin;
	lstat->pgpgout = kstat.pgpgout;
	lstat->pswpin = kstat.pswpin;
	lstat->pswpout = kstat.pswpout;

	lstat->context_swtch = kstat.context_swtch;
	lstat->boot_time = xtime.tv_sec - jiffies / HZ;
	lstat->total_forks = total_forks;
	break;
    case LIBGTOP_MEM:
	if (jiffies - libgtop_mem_timestamp < libgtop_update_expensive)
	    return 0;
	libgtop_mem_timestamp = jiffies;

	mem = table->data;
	si_meminfo (&i);

	mem->totalram = i.totalram;
	mem->freeram = i.freeram;
	mem->sharedram = i.sharedram;
	mem->bufferram = i.bufferram;
	return 0;
    case LIBGTOP_SWAP:
	if (jiffies - libgtop_swap_timestamp < libgtop_update_expensive)
	    return 0;
	libgtop_swap_timestamp = jiffies;

	swap = table->data;
	si_swapinfo (&i);

	swap->totalswap = i.totalswap;
	swap->freeswap = i.freeswap;
	return 0;
    case LIBGTOP_PROCLIST:
	proclist = table->data;

	if (nlen == 1) {
	    which = 0;
	    arg = 0;
	} else if (nlen == 2) {
	    which = name [1];
	    arg = 0;
	} else if (nlen == 3) {
	    which = name [1];
	    arg = name [2];
	} else {
	    return -EINVAL;
	}

	tsk = task [0];
	read_lock (&tasklist_lock);
	for (index = tindex = 0; index < nr_tasks;
	     index++, tsk = tsk->next_task) {
	    if (tsk->pid == 0) continue;
	    switch (which & LIBGTOP_PROCLIST_MASK) {
	    case LIBGTOP_PROCLIST_PID:
		if (tsk->pid != arg) continue;
		break;
	    case LIBGTOP_PROCLIST_PGRP:
		if (tsk->pgrp != arg) continue;
		break;
	    case LIBGTOP_PROCLIST_SESSION:
		if (tsk->session != arg) continue;
		break;
	    case LIBGTOP_PROCLIST_TTY:
		tty = tsk->tty ?
		    kdev_t_to_nr (tsk->tty->device) : 0;
		if (tty != arg) continue;
		break;
	    case LIBGTOP_PROCLIST_UID:
		if (tsk->uid != arg) continue;
		break;
	    case LIBGTOP_PROCLIST_RUID:
		if (tsk->euid != arg) continue;
		break;
	    }

	    if ((which & LIBGTOP_EXCLUDE_IDLE) && (tsk->state != 0))
		continue;

	    if ((which & LIBGTOP_EXCLUDE_NOTTY) && (tsk->tty == NULL))
		continue;
			
	    proclist->pids [tindex++] = tsk->pid;
	}

	proclist->nr_running = nr_running;
	proclist->last_pid = last_pid;
	proclist->nr_tasks = tindex;
	read_unlock(&tasklist_lock);
	return 0;
    default:
	return -EINVAL;
    }

    return 0;
}

static int
system_ctl_handler (ctl_table *table, int *name, int nlen,
		    void *oldval, size_t *oldlenp, void *newval,
		    size_t newlen, void **context)
{
    int ret, len, len_name;

    if (!table->data || !table->maxlen)
	return -ENOTDIR;

    if (!oldval || !oldlenp || get_user(len, oldlenp))
	return -EFAULT;

    if (!name || !nlen || get_user(len_name, name))
	return -EFAULT;

    if (len != table->maxlen)
	return -EFAULT;

    ret = libgtop_sysctl (table, nlen, name);
    if (ret) return ret;

    if(copy_to_user(oldval, table->data, len))
	return -EFAULT;

    return 1;
}

static int
proc_ctl_handler (ctl_table *table, int *name, int nlen,
		  void *oldval, size_t *oldlenp, void *newval,
		  size_t newlen, void **context)
{
    int ret, len;

#if 0
    printk ("proc_ctl_handler: %p - %p - %d - (%p,%p) - (%p,%d) - %p\n",
	    table, name, nlen, oldval, oldlenp, newval, newlen, context);
#endif

    if (!name || !nlen || get_user(len, name))
	return -EFAULT;

#if 0
    printk ("FUNCTION: %d - %d - %p\n", table->ctl_name,
	    *name, table->de);
#endif

    if (!table->data || !table->maxlen)
	return -ENOTDIR;

    if (!oldval || !oldlenp || get_user(len, oldlenp))
	return -EFAULT;

    if (len != table->maxlen)
	return -EFAULT;

    return -ENOSYS;
}
