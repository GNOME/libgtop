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

#include <asm/segment.h>
#include <asm/pgtable.h>
#include <asm/io.h>

#include <linux/table.h>

#include "version.h"

asmlinkage int
sys_table (int type, union table *buf)
{
	union table tbl;
	struct sysinfo i;
	int err;

	if (type == TABLE_VERSION)
		return _TABLE_VERSION;

	if (!buf)
		return -EFAULT;

	switch (type) {
	case TABLE_CPU:
		tbl.cpu.total = jiffies;    
		tbl.cpu.user  = kstat.cpu_user;
		tbl.cpu.nice  = kstat.cpu_nice;
		tbl.cpu.sys   = kstat.cpu_system;
		tbl.cpu.idle  = tbl.cpu.total - (tbl.cpu.user + tbl.cpu.nice + tbl.cpu.sys);
		tbl.cpu.frequency = HZ;
		break;
	case TABLE_MEM:
		si_meminfo (&i);
		tbl.mem.total  = i.totalram;
		tbl.mem.used   = i.totalram - i.freeram;
		tbl.mem.free   = i.freeram;
		tbl.mem.shared = i.sharedram;
		tbl.mem.buffer = i.bufferram;
		tbl.mem.cached = page_cache_size << PAGE_SHIFT;
		break;
	case TABLE_SWAP:
		si_swapinfo (&i);
		tbl.swap.total = i.totalswap;
		tbl.swap.used  = i.totalswap - i.freeswap;
		tbl.swap.free  = i.freeswap;
		break;
	case TABLE_LOADAVG:
		tbl.loadavg.loadavg [0] = avenrun [0];
		tbl.loadavg.loadavg [1] = avenrun [1];
		tbl.loadavg.loadavg [2] = avenrun [2];
		tbl.loadavg.nr_running = nr_running;
		tbl.loadavg.nr_tasks = nr_tasks;
		tbl.loadavg.last_pid = last_pid;
		break;
	case TABLE_UPTIME:
		tbl.uptime.uptime = jiffies;
		tbl.uptime.idle   = task[0]->utime + task[0]->stime;
		break;
	default:
		return -EINVAL;
	}

	err = verify_area (VERIFY_WRITE, buf, sizeof (struct table));
	if (err)
		return err;

	memcpy_tofs (buf, &tbl, sizeof (union table));
	return 0;
}
