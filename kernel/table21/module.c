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

extern int (*table_function_ptr) (int, union table *, const void *);

int table_fkt (int, union table *, const void *);

EXPORT_NO_SYMBOLS;

int
init_module(void)
{
	printk ("init_module () = %p - %d, %d\n",
		table_fkt, sizeof (union table), sizeof (sigset_t));
	table_function_ptr = table_fkt;
	return 0;
}

void
cleanup_module(void)
{
	table_function_ptr = 0;
}

#define LOAD_INT(x) ((x) >> FSHIFT)
#define LOAD_FRAC(x) LOAD_INT(((x) & (FIXED_1-1)) * 100)

#ifdef CONFIG_DEBUG_MALLOC
int get_malloc(char * buffer);
#endif

static void collect_sigign_sigcatch(struct task_struct *p, sigset_t *ign,
				    sigset_t *catch)
{
	struct k_sigaction *k;
	int i;

	sigemptyset(ign);
	sigemptyset(catch);

#if 0
	printk ("collect_sigign_sigcatch: %p - %p\n",
		p, p->sig);
#endif

	if (p->sig) {
		k = p->sig->action;
		for (i = 1; i <= _NSIG; ++i, ++k) {
#if 0
			printk ("signal: %d - %p (%p, %p)\n",
				i, k->sa.sa_handler, SIG_IGN, SIG_DFL);
#endif
			if (k->sa.sa_handler == SIG_IGN)
				sigaddset(ign, i);
			else if (k->sa.sa_handler != SIG_DFL)
				sigaddset(catch, i);
		}
	}
}

/*
 * These bracket the sleeping functions..
 */
extern void scheduling_functions_start_here(void);
extern void scheduling_functions_end_here(void);
#define first_sched	((unsigned long) scheduling_functions_start_here)
#define last_sched	((unsigned long) scheduling_functions_end_here)

static unsigned long get_wchan(struct task_struct *p)
{
	if (!p || p == current || p->state == TASK_RUNNING)
		return 0;
#if defined(__i386__)
	{
		unsigned long ebp, eip;
		unsigned long stack_page;
		int count = 0;

		stack_page = 4096 + (unsigned long)p;
		if (!stack_page)
			return 0;
		ebp = p->tss.ebp;
		do {
			if (ebp < stack_page || ebp >= 4092+stack_page)
				return 0;
			eip = *(unsigned long *) (ebp+4);
			if (eip < first_sched || eip >= last_sched)
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
	    if (pc >= first_sched && pc < last_sched) {
		schedule_frame = ((unsigned long *)p->tss.ksp)[6];
		return ((unsigned long *)schedule_frame)[12];
	    }
	    return pc;
	}	
#elif defined(__mc68000__)
	{
	    unsigned long fp, pc;
	    unsigned long stack_page;
	    int count = 0;
	    extern int sys_pause (void);

	    stack_page = p->kernel_stack_page;
	    if (!stack_page)
		    return 0;
	    fp = ((struct switch_stack *)p->tss.ksp)->a6;
	    do {
		    if (fp < stack_page || fp >= 4088+stack_page)
			    return 0;
		    pc = ((unsigned long *)fp)[1];
		/* FIXME: This depends on the order of these functions. */
		    if (pc < first_sched || pc >= last_sched)
		      return pc;
		    fp = *(unsigned long *) fp;
	    } while (count++ < 16);
	}
#elif defined(__powerpc__)
	return (p->tss.wchan);
#elif defined (CONFIG_ARM)
	{
		unsigned long fp, lr;
		unsigned long stack_page;
		int count = 0;

		stack_page = 4096 + (unsigned long)p;
		fp = get_css_fp (&p->tss);
		do {
			if (fp < stack_page || fp > 4092+stack_page)
				return 0;
			lr = pc_pointer (((unsigned long *)fp)[-1]);
			if (lr < first_sched || lr > last_sched)
				return lr;
			fp = *(unsigned long *) (fp - 12);
		} while (count ++ < 16);
	}
#endif
	return 0;
}

#if defined(__i386__)
# define KSTK_EIP(tsk)	(((unsigned long *)(4096+(unsigned long)(tsk)))[1019])
# define KSTK_ESP(tsk)	(((unsigned long *)(4096+(unsigned long)(tsk)))[1022])
#elif defined(__alpha__)
  /*
   * See arch/alpha/kernel/ptrace.c for details.
   */
# define PT_REG(reg)		(PAGE_SIZE - sizeof(struct pt_regs)	\
				 + (long)&((struct pt_regs *)0)->reg)
# define KSTK_EIP(tsk) \
    (*(unsigned long *)(PT_REG(pc) + PAGE_SIZE + (unsigned long)(tsk)))
# define KSTK_ESP(tsk)	((tsk) == current ? rdusp() : (tsk)->tss.usp)
#elif defined(CONFIG_ARM)
# define KSTK_EIP(tsk)	(((unsigned long *)(4096+(unsigned long)(tsk)))[1022])
# define KSTK_ESP(tsk)	(((unsigned long *)(4096+(unsigned long)(tsk)))[1020])
#elif defined(__mc68000__)
#define	KSTK_EIP(tsk)	\
    ({			\
	unsigned long eip = 0;	 \
 	if ((tsk)->tss.esp0 > PAGE_SIZE && \
	    MAP_NR((tsk)->tss.esp0) < max_mapnr) \
	      eip = ((struct pt_regs *) (tsk)->tss.esp0)->pc;	 \
        eip; })
#define	KSTK_ESP(tsk)	((tsk) == current ? rdusp() : (tsk)->tss.usp)
#elif defined(__powerpc__)
#define KSTK_EIP(tsk)	((tsk)->tss.regs->nip)
#define KSTK_ESP(tsk)	((tsk)->tss.regs->gpr[1])
#elif defined (__sparc_v9__)
# define KSTK_EIP(tsk)  ((tsk)->tss.kregs->tpc)
# define KSTK_ESP(tsk)  ((tsk)->tss.kregs->u_regs[UREG_FP])
#elif defined(__sparc__)
# define KSTK_EIP(tsk)  ((tsk)->tss.kregs->pc)
# define KSTK_ESP(tsk)  ((tsk)->tss.kregs->u_regs[UREG_FP])
#endif

/* Gcc optimizes away "strlen(x)" for constant x */
#define ADDBUF(buffer, string) \
do { memcpy(buffer, string, strlen(string)); \
     buffer += strlen(string); } while (0)

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
		if (MAP_NR(pte_page(page)) >= max_mapnr)
			continue;
		if (atomic_read(&mem_map[MAP_NR(pte_page(page))].count) > 1)
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

int
table_fkt (int type, union table *buf, const void *param)
{
	union table tbl;
	struct sysinfo i;
	struct task_struct *tsk = NULL;
	struct proclist_args plistargs;
	int index, tindex, err, tty;
	sigset_t sigign, sigcatch;
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
		copy_from_user (&pid, param, sizeof (pid_t));

		read_lock (&tasklist_lock);
		tsk = find_task_by_pid (pid);
		/* FIXME!! This should be done after the last use */
		read_unlock(&tasklist_lock);

		if (tsk == NULL)
			return -ESRCH;
		break;
	case TABLE_PROCLIST:
		err = verify_area (VERIFY_READ, param,
				   sizeof (struct proclist_args));
		if (err)
			return err;
		copy_from_user (&plistargs, param,
				sizeof (struct proclist_args));
		break;
	}

	/* Main function dispatcher */

	switch (type) {
	case TABLE_PROCLIST:
		tsk = task [0];
		read_lock (&tasklist_lock);
		for (index = tindex = 0; index < nr_tasks;
		     index++, tsk = tsk->next_task) {
			if (tsk->pid == 0) continue;
			switch (plistargs.which & TABLE_KERN_PROC_MASK) {
			case TABLE_KERN_PROC_PID:
				if (tsk->pid != plistargs.arg) continue;
				break;
			case TABLE_KERN_PROC_PGRP:
				if (tsk->pgrp != plistargs.arg) continue;
				break;
			case TABLE_KERN_PROC_SESSION:
				if (tsk->session != plistargs.arg) continue;
			case TABLE_KERN_PROC_TTY:
				tty = tsk->tty ?
					kdev_t_to_nr (tsk->tty->device) : 0;
				if (tty != plistargs.arg) continue;
				break;
			case TABLE_KERN_PROC_UID:
				if (tsk->uid != plistargs.arg) continue;
				break;
			case TABLE_KERN_PROC_RUID:
				if (tsk->euid != plistargs.arg) continue;
				break;
			}

			if ((plistargs.which & TABLE_EXCLUDE_IDLE) &&
			    (tsk->state != 0))
				continue;

			if ((plistargs.which & TABLE_EXCLUDE_NOTTY) &&
			    (tsk->tty == NULL))
				continue;
			
			tbl.proclist.pids [tindex++] = tsk->pid;
		}
		tbl.proclist.nr_running = nr_running;
		tbl.proclist.last_pid = last_pid;
		tbl.proclist.nr_tasks = tindex;
		read_unlock(&tasklist_lock);
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
		tbl.uptime.idle   = task[0]->times.tms_utime +
			task[0]->times.tms_stime;
		break;
	case TABLE_PROC_STATE:
		tbl.proc_state.uid = tsk->uid;
		tbl.proc_state.gid = tsk->gid;
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
		memcpy (&tbl.proc_signal.signal, &tsk->signal,
			sizeof (tbl.proc_signal.signal));

		memcpy (&tbl.proc_signal.blocked, &tsk->blocked,
			sizeof (tbl.proc_signal.blocked));

		collect_sigign_sigcatch (tsk, &sigign, &sigcatch);

		memcpy (&tbl.proc_signal.ignored, &sigign,
			sizeof (tbl.proc_signal.ignored));

		memcpy (&tbl.proc_signal.caught, &sigcatch,
			sizeof (tbl.proc_signal.caught));

#if 0
		printk ("PROC_SIGNAL: (%lu, %lu) - (%lu, %lu)\n",
			tbl.proc_signal.ignored.sig [0],
			tbl.proc_signal.ignored.sig [1],
			tbl.proc_signal.caught.sig [0],
			tbl.proc_signal.caught.sig [1]);
#endif
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
			tbl.proc_mem.start_mmap = tsk->mm->mmap ?
				tsk->mm->mmap->vm_start : 0;
			tbl.proc_mem.arg_start = tsk->mm->arg_start;
			tbl.proc_mem.arg_end = tsk->mm->arg_end;
			tbl.proc_mem.env_start = tsk->mm->env_start;
			tbl.proc_mem.env_end = tsk->mm->env_end;
			tbl.proc_mem.rss = tsk->mm->rss << PAGE_SHIFT;
			tbl.proc_mem.total_vm = tsk->mm->total_vm;
			tbl.proc_mem.locked_vm = tsk->mm->locked_vm;
		}
		tbl.proc_mem.rlim = tsk->rlim ? tsk->rlim[RLIMIT_RSS].rlim_cur : 0;
		break;
	case TABLE_PROC_SEGMENT:
		if (tsk->mm && tsk->mm != &init_mm) {
			unsigned long vsize = 0;
			int size = 0, resident = 0, share = 0;
			int trs = 0, lrs = 0, drs = 0, srs = 0, dt = 0;
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

				/* Well, shared library seem to get mapped
				 * above 0x40000000 and are executable,
				 * so I use this hack to get their size.
				 */

				if (vma->vm_flags & VM_GROWSDOWN)
					srs += pages;	/* stack */
				else if ((vma->vm_flags & VM_EXEC) &&
					 (vma->vm_start > 0x40000000))
					lrs += pages;	/* library */
				else if (vma->vm_flags & VM_EXECUTABLE)
					trs += pages;	/* text */
				else
					drs += pages;

				vma = vma->vm_next;
			}

			tbl.proc_segment.vsize = vsize;
			tbl.proc_segment.size = size << PAGE_SHIFT;
			tbl.proc_segment.resident = resident << PAGE_SHIFT;
			tbl.proc_segment.shared = share << PAGE_SHIFT;
			tbl.proc_segment.trs = trs << PAGE_SHIFT;
			tbl.proc_segment.lrs = lrs << PAGE_SHIFT;
			tbl.proc_segment.drs = drs << PAGE_SHIFT;
			tbl.proc_segment.srs = srs << PAGE_SHIFT;
			tbl.proc_segment.dt = dt << PAGE_SHIFT;
		}
		break;
	case TABLE_PROC_TIME:
		tbl.proc_time.utime = tsk->times.tms_utime;
		tbl.proc_time.stime = tsk->times.tms_stime;
		tbl.proc_time.cutime = tsk->times.tms_cutime;
		tbl.proc_time.cstime = tsk->times.tms_cstime;

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
		
		tbl.proc_kernel.kesp = KSTK_EIP(tsk);
		tbl.proc_kernel.keip = KSTK_ESP(tsk);
		
		tbl.proc_kernel.nswap = tsk->nswap;
		tbl.proc_kernel.cnswap = tsk->cnswap;

		tbl.proc_kernel.wchan = get_wchan (tsk);
		break;
	default:
		return -EINVAL;
	}

	err = verify_area (VERIFY_WRITE, buf, sizeof (struct table));
	if (err)
		return err;

	copy_to_user (buf, &tbl, sizeof (union table));

	return 0;
}
