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

#if defined(__i386__)
# define KSTK_EIP(tsk)	(((unsigned long *)tsk->kernel_stack_page)[1019])
# define KSTK_ESP(tsk)	(((unsigned long *)tsk->kernel_stack_page)[1022])
#elif defined(__alpha__)
  /*
   * See arch/alpha/kernel/ptrace.c for details.
   */
# define PT_REG(reg)		(PAGE_SIZE - sizeof(struct pt_regs)	\
				 + (long)&((struct pt_regs *)0)->reg)
# define KSTK_EIP(tsk)	(*(unsigned long *)(tsk->kernel_stack_page + PT_REG(pc)))
# define KSTK_ESP(tsk)	((tsk) == current ? rdusp() : (tsk)->tss.usp)
#elif defined(__sparc__)
# define PT_REG(reg)            (PAGE_SIZE - sizeof(struct pt_regs)     \
                                 + (long)&((struct pt_regs *)0)->reg)
# define KSTK_EIP(tsk)  (*(unsigned long *)(tsk->kernel_stack_page + PT_REG(pc)))
# define KSTK_ESP(tsk)  (*(unsigned long *)(tsk->kernel_stack_page + PT_REG(u_regs[UREG_FP])))
#endif

static struct task_struct *
get_task (pid_t pid)
{
	struct task_struct ** p;
	
	p = task;
	while (++p < task+NR_TASKS) {
		if (*p && (*p)->pid == pid)
			return *p;
	}

	return NULL;
}

static inline void statm_pte_range(pmd_t * pmd, unsigned long address, unsigned long size,
	int * pages, int * shared, int * dirty, int * total)
{
	pte_t * pte;
	unsigned long end;

	if (pmd_none(*pmd))
		return;
	if (pmd_bad(*pmd)) {
		printk("statm_pte_range: bad pmd (%08lx)\n", pmd_val(*pmd));
		pmd_clear(pmd);
		return;
	}
	pte = pte_offset(pmd, address);
	address &= ~PMD_MASK;
	end = address + size;
	if (end > PMD_SIZE)
		end = PMD_SIZE;
	do {
		pte_t page = *pte;

		address += PAGE_SIZE;
		pte++;
		if (pte_none(page))
			continue;
		++*total;
		if (!pte_present(page))
			continue;
		++*pages;
		if (pte_dirty(page))
			++*dirty;
		if (pte_page(page) >= high_memory)
			continue;
		if (mem_map[MAP_NR(pte_page(page))].count > 1)
			++*shared;
	} while (address < end);
}

static inline void statm_pmd_range(pgd_t * pgd, unsigned long address, unsigned long size,
	int * pages, int * shared, int * dirty, int * total)
{
	pmd_t * pmd;
	unsigned long end;

	if (pgd_none(*pgd))
		return;
	if (pgd_bad(*pgd)) {
		printk("statm_pmd_range: bad pgd (%08lx)\n", pgd_val(*pgd));
		pgd_clear(pgd);
		return;
	}
	pmd = pmd_offset(pgd, address);
	address &= ~PGDIR_MASK;
	end = address + size;
	if (end > PGDIR_SIZE)
		end = PGDIR_SIZE;
	do {
		statm_pte_range(pmd, address, end - address, pages, shared, dirty, total);
		address = (address + PMD_SIZE) & PMD_MASK;
		pmd++;
	} while (address < end);
}

static void statm_pgd_range(pgd_t * pgd, unsigned long address, unsigned long end,
	int * pages, int * shared, int * dirty, int * total)
{
	while (address < end) {
		statm_pmd_range(pgd, address, end - address, pages, shared, dirty, total);
		address = (address + PGDIR_SIZE) & PGDIR_MASK;
		pgd++;
	}
}

static unsigned long
get_wchan (struct task_struct *p)
{
	if (!p || p == current || p->state == TASK_RUNNING)
		return 0;
#if defined(__i386__)
	{
		unsigned long ebp, eip;
		unsigned long stack_page;
		int count = 0;

		stack_page = p->kernel_stack_page;
		if (!stack_page)
			return 0;
		ebp = p->tss.ebp;
		do {
			if (ebp < stack_page || ebp >= 4092+stack_page)
				return 0;
			eip = *(unsigned long *) (ebp+4);
			if (eip < (unsigned long) interruptible_sleep_on
			    || eip >= (unsigned long) add_timer)
				return eip;
			ebp = *(unsigned long *) ebp;
		} while (count++ < 16);
	}
#elif defined(__alpha__)
	/*
	 * This one depends on the frame size of schedule().  Do a
	 * "disass schedule" in gdb to find the frame size.  Also, the
	 * code assumes that sleep_on() follows immediately after
	 * interruptible_sleep_on() and that add_timer() follows
	 * immediately after interruptible_sleep().  Ugly, isn't it?
	 * Maybe adding a wchan field to task_struct would be better,
	 * after all...
	 */
	{
		unsigned long schedule_frame;
		unsigned long pc;
		
		pc = thread_saved_pc(&p->tss);
		if (pc >= (unsigned long) interruptible_sleep_on && pc < (unsigned long) add_timer) {
			schedule_frame = ((unsigned long *)p->tss.ksp)[6];
			return ((unsigned long *)schedule_frame)[12];
		}
		return pc;
	}
#endif
	return 0;
}

asmlinkage int
sys_table (int type, union table *buf, const void *param)
{
	union table tbl;
	struct sysinfo i;
	struct task_struct *tsk = NULL;
	struct ip_chain *chain;
	struct ip_fwkernel *rule;
	char devname [9];
	int index, err;
	pid_t pid;

	if (type == TABLE_VERSION)
		return _TABLE_VERSION;

	if (!buf)
		return -EFAULT;

	memset (&tbl, 0, sizeof (union table));

	/* For TABLE_PROC_*, read pid and get task_struct */

	switch (type) {
	case TABLE_PROC_UID:
	case TABLE_PROC_MEM:
	case TABLE_PROC_SEGMENT:
	case TABLE_PROC_TIME:
	case TABLE_PROC_STATE:
	case TABLE_PROC_SIGNAL:
	case TABLE_PROC_KERNEL:
	  err = verify_area (VERIFY_READ, param, sizeof (pid_t));
		if (err)
			return err;
		memcpy_fromfs (&pid, param, sizeof (pid_t));

		tsk = get_task (pid);
		if (tsk == NULL)
			return -ESRCH;
		break;
	case TABLE_NETACCT:
		err = verify_area (VERIFY_READ, param, 5);
		if (err)
			return err;
		copy_from_user (devname, param, 5);
		devname [5] = 0;

		break;
	}

	/* Main function dispatcher */

	switch (type) {
	case TABLE_PROCLIST:
		tsk = task [0];
		for (index = 0; index < nr_tasks; index++) {
			tbl.proclist.pids [index] = tsk->pid;
			tsk = tsk->next_task;
		}
		tbl.proclist.nr_running = nr_running;
		tbl.proclist.nr_tasks = nr_tasks;
		tbl.proclist.last_pid = last_pid;
		break;
	case TABLE_CPU:
		tbl.cpu.total = jiffies;    
		tbl.cpu.user  = kstat.cpu_user;
		tbl.cpu.nice  = kstat.cpu_nice;
		tbl.cpu.sys   = kstat.cpu_system;
		tbl.cpu.idle  = tbl.cpu.total -
			(tbl.cpu.user + tbl.cpu.nice + tbl.cpu.sys);
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
		tbl.swap.pagein = kstat.pswpin;
		tbl.swap.pageout = kstat.pswpout;
		break;
	case TABLE_LOADAVG:
		tbl.loadavg.loadavg [0] = (double) avenrun [0] / (1 << FSHIFT);
		tbl.loadavg.loadavg [1] = (double) avenrun [1] / (1 << FSHIFT);
		tbl.loadavg.loadavg [2] = (double) avenrun [2] / (1 << FSHIFT);
		tbl.loadavg.nr_running = nr_running;
		tbl.loadavg.nr_tasks = nr_tasks;
		tbl.loadavg.last_pid = last_pid;
		break;
	case TABLE_UPTIME:
		tbl.uptime.uptime = jiffies;
		tbl.uptime.idle   = task[0]->utime + task[0]->stime;
		break;
	case TABLE_PROC_STATE:
		tbl.proc_state.state = tsk->state;
		tbl.proc_state.flags = tsk->flags;
		memcpy (tbl.proc_state.comm, tsk->comm,
			sizeof (tbl.proc_state.comm));
		break;
	case TABLE_PROC_UID:
		tbl.proc_uid.uid = tsk->uid;
		tbl.proc_uid.euid = tsk->euid;
		tbl.proc_uid.suid = tsk->suid;
		tbl.proc_uid.fsuid = tsk->fsuid;
		
		tbl.proc_uid.gid = tsk->gid;
		tbl.proc_uid.egid = tsk->egid;
		tbl.proc_uid.sgid = tsk->sgid;
		tbl.proc_uid.fsgid = tsk->fsgid;
		
		tbl.proc_uid.pid = tsk->pid;
		tbl.proc_uid.pgrp = tsk->pgrp;
		tbl.proc_uid.ppid = tsk->p_pptr->pid;
		
		tbl.proc_uid.session = tsk->session;
	        tbl.proc_uid.tty = tsk->tty ?
			kdev_t_to_nr (tsk->tty->device) : 0;
		tbl.proc_uid.tpgid = tsk->tty ? tsk->tty->pgrp : -1;
		
		tbl.proc_uid.priority = tsk->priority;
		tbl.proc_uid.counter = tsk->counter;
		tbl.proc_uid.def_priority = DEF_PRIORITY;
		break;
	case TABLE_PROC_SIGNAL:
		tbl.proc_signal.signal = tsk->signal;
		tbl.proc_signal.blocked = tsk->blocked;

		if (tsk->sig) {
			struct sigaction * action = tsk->sig->action;
			unsigned long sig_ign = 0, sig_caught = 0;
			unsigned long bit = 1;
			int i;
			
			for (i = 0; i < 32; i++) {
				switch((unsigned long) action->sa_handler) {
				case 0:
					break;
				case 1:
					sig_ign |= bit;
					break;
				default:
					sig_caught |= bit;
				}
				bit <<= 1;
				action++;
			}

			tbl.proc_signal.ignored = sig_ign;
			tbl.proc_signal.caught  = sig_caught;
		} else {
			tbl.proc_signal.ignored = 0;
			tbl.proc_signal.caught  = 0;
		}
		break;
	case TABLE_PROC_MEM:
		if (tsk->mm && tsk->mm != &init_mm) {
			tbl.proc_mem.context = tsk->mm->context;
			tbl.proc_mem.start_code = tsk->mm->start_code;
			tbl.proc_mem.end_code = tsk->mm->end_code;
			tbl.proc_mem.start_data = tsk->mm-> start_data;
			tbl.proc_mem.end_data = tsk->mm->end_data;
			tbl.proc_mem.start_brk = tsk->mm->start_brk;
			tbl.proc_mem.brk = tsk->mm->brk;
			tbl.proc_mem.start_stack = tsk->mm->start_stack;
			tbl.proc_mem.start_mmap = tsk->mm->start_mmap;
			tbl.proc_mem.arg_start = tsk->mm->arg_start;
			tbl.proc_mem.arg_end = tsk->mm->arg_end;
			tbl.proc_mem.env_start = tsk->mm->env_start;
			tbl.proc_mem.env_end = tsk->mm->env_end;
			tbl.proc_mem.rss = tsk->mm->rss;
			tbl.proc_mem.total_vm = tsk->mm->total_vm;
			tbl.proc_mem.locked_vm = tsk->mm->locked_vm;
		}
		tbl.proc_mem.rlim = tsk->rlim ?
			tsk->rlim[RLIMIT_RSS].rlim_cur : 0;
		break;
	case TABLE_PROC_SEGMENT:
		if (tsk->mm && tsk->mm != &init_mm) {
			unsigned long vsize = 0;
			int size = 0, resident = 0, share = 0;
			int trs = 0, lrs = 0, drs = 0, dt = 0;
			struct vm_area_struct * vma = tsk->mm->mmap;

			while (vma) {
				pgd_t *pgd = pgd_offset(tsk->mm, vma->vm_start);
				int pages = 0, shared = 0, dirty = 0, total = 0;
				
				vsize += vma->vm_end - vma->vm_start;

				statm_pgd_range (pgd, vma->vm_start, vma->vm_end,
						 &pages, &shared, &dirty, &total);
				resident += pages;
				share += shared;
				dt += dirty;
				size += total;
				if (vma->vm_flags & VM_EXECUTABLE)
					trs += pages;	/* text */
				else if (vma->vm_flags & VM_GROWSDOWN)
					drs += pages;	/* stack */
				else if (vma->vm_end > 0x60000000)
					lrs += pages;	/* library */
				else
					drs += pages;
				vma = vma->vm_next;
			}

			tbl.proc_segment.vsize = vsize;
			tbl.proc_segment.size = size;
			tbl.proc_segment.resident = resident;
			tbl.proc_segment.shared = share;
			tbl.proc_segment.trs = trs;
			tbl.proc_segment.lrs = lrs;
			tbl.proc_segment.dt = dt;
		}
		break;
	case TABLE_PROC_TIME:
		tbl.proc_time.utime = tsk->utime;
		tbl.proc_time.stime = tsk->stime;
		tbl.proc_time.cutime = tsk->cutime;
		tbl.proc_time.cstime = tsk->cstime;

		tbl.proc_time.start_time = tsk->start_time;
		tbl.proc_time.timeout = tsk->timeout;
		tbl.proc_time.policy = tsk->policy;
		tbl.proc_time.rt_priority = tsk->rt_priority;

		tbl.proc_time.it_real_value = tsk->it_real_value;
		tbl.proc_time.it_prof_value = tsk->it_prof_value;
		tbl.proc_time.it_virt_value = tsk->it_virt_value;
		tbl.proc_time.it_real_incr = tsk->it_real_incr;
		tbl.proc_time.it_prof_incr = tsk->it_prof_incr;
		tbl.proc_time.it_virt_incr = tsk->it_virt_incr;
		break;
	case TABLE_PROC_KERNEL:
		tbl.proc_kernel.min_flt = tsk->min_flt;
		tbl.proc_kernel.cmin_flt = tsk->cmin_flt;
		tbl.proc_kernel.maj_flt = tsk->maj_flt;
		tbl.proc_kernel.cmaj_flt = tsk->cmaj_flt;
		
		tbl.proc_kernel.kesp = tsk->kernel_stack_page ? KSTK_EIP(tsk) : 0;
		tbl.proc_kernel.keip = tsk->kernel_stack_page ? KSTK_ESP(tsk) : 0;
		
		tbl.proc_kernel.nswap = tsk->nswap;
		tbl.proc_kernel.cnswap = tsk->cnswap;

		tbl.proc_kernel.wchan = get_wchan (tsk);
		break;
	case TABLE_NETACCT:
		for (chain = ip_fw_chains; chain; chain = chain->next) {
			for (rule = chain->chain; rule; rule = rule->next) {
				const char *name = rule->ipfw.fw_vianame;
				int k;
				
				if (name [0] && !strncmp (param, name, 5))
					continue;
				
				for (k = 0; k < NUM_SLOTS; k++) {
					tbl.netacct.packets +=
						rule->counters[k].pcnt;
					tbl.netacct.bytes +=
						rule->counters[k].bcnt;
				}
			}
		}
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
