/*
 * linux/libgtop/libgtop_syms.c
 * Copyright (C) 1999 Martin Baulig
 */

#include <linux/config.h>
#include <linux/module.h>

#include <linux/sched.h>

#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/swap.h>

extern unsigned long total_forks;

EXPORT_SYMBOL(task);
EXPORT_SYMBOL(pidhash);
EXPORT_SYMBOL(avenrun);
EXPORT_SYMBOL(nr_running);
EXPORT_SYMBOL(nr_tasks);
EXPORT_SYMBOL(last_pid);
EXPORT_SYMBOL(total_forks);
EXPORT_SYMBOL(si_swapinfo);

extern void scheduling_functions_start_here(void);
extern void scheduling_functions_end_here(void);
EXPORT_SYMBOL(scheduling_functions_start_here);
EXPORT_SYMBOL(scheduling_functions_end_here);
