/*
 * linux/table/table_impl.c
 * Copyright (C) 1998 Martin Baulig
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

#include <linux/module.h>
#include <linux/table.h>

#include "version.h"

extern void scheduling_functions_start_here(void);
extern void scheduling_functions_end_here(void);

int (*table_function_ptr) (int, union table *, const void *) = 0;

EXPORT_SYMBOL(table_function_ptr);

EXPORT_SYMBOL(nr_running);
EXPORT_SYMBOL(pidhash);
EXPORT_SYMBOL(task);
EXPORT_SYMBOL(si_swapinfo);
EXPORT_SYMBOL(scheduling_functions_start_here);
EXPORT_SYMBOL(scheduling_functions_end_here);
EXPORT_SYMBOL(avenrun);
EXPORT_SYMBOL(nr_tasks);
EXPORT_SYMBOL(last_pid);
EXPORT_SYMBOL(page_cache_size);
EXPORT_SYMBOL(init_mm);

asmlinkage int
sys_table (int type, union table *buf, const void *param)
{
	if (table_function_ptr == 0)
		return -ENOSYS;

	return (*table_function_ptr) (type, buf, param);
}
