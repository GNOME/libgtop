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
static libgtop_proc_state_t libgtop_proc_state;
static libgtop_proc_kernel_t libgtop_proc_kernel;
static libgtop_proc_segment_t libgtop_proc_segment;
static libgtop_proc_mem_t libgtop_proc_mem;
static libgtop_proc_signal_t libgtop_proc_signal;

static ctl_table libgtop_table[];
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
    {LIBGTOP_PROC_STATE, NULL, &libgtop_proc_state,
     sizeof (libgtop_proc_state), 0444, NULL, NULL, &proc_ctl_handler},
    {LIBGTOP_PROC_KERNEL, NULL, &libgtop_proc_kernel,
     sizeof (libgtop_proc_kernel), 0444, NULL, NULL, &proc_ctl_handler},
    {LIBGTOP_PROC_SEGMENT, NULL, &libgtop_proc_segment,
     sizeof (libgtop_proc_segment), 0444, NULL, NULL, &proc_ctl_handler},
    {LIBGTOP_PROC_MEM, NULL, &libgtop_proc_mem,
     sizeof (libgtop_proc_mem), 0444, NULL, NULL, &proc_ctl_handler},
    {LIBGTOP_PROC_SIGNAL, NULL, &libgtop_proc_signal,
     sizeof (libgtop_proc_signal), 0444, NULL, NULL, &proc_ctl_handler},
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

/*
 * These bracket the sleeping functions..
 */
extern void scheduling_functions_start_here(void);
extern void scheduling_functions_end_here(void);
#define first_sched	((unsigned long) scheduling_functions_start_here)
#define last_sched	((unsigned long) scheduling_functions_end_here)

static unsigned long
get_wchan (struct task_struct *p)
{
    if (!p || p == current || p->state == TASK_RUNNING)
	return 0;
#if defined(__i386__)
    {
	unsigned long ebp, esp, eip;
	unsigned long stack_page;
	int count = 0;

	stack_page = (unsigned long)p;
	esp = p->tss.esp;
	if (!stack_page || esp < stack_page || esp >= 8188+stack_page)
	    return 0;
	/* include/asm-i386/system.h:switch_to() pushes ebp last. */
	ebp = *(unsigned long *) esp;
	do {
	    if (ebp < stack_page || ebp >= 8188+stack_page)
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

	stack_page = (unsigned long)p;
	fp = ((struct switch_stack *)p->tss.ksp)->a6;
	do {
	    if (fp < stack_page+sizeof(struct task_struct) ||
		fp >= 8184+stack_page)
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
#elif defined (__sparc__)
    {
	unsigned long pc, fp, bias = 0;
	unsigned long task_base = (unsigned long) p;
	struct reg_window *rw;
	int count = 0;

#ifdef __sparc_v9__
	bias = STACK_BIAS;
#endif
	fp = p->tss.ksp + bias;
	do {
	    /* Bogus frame pointer? */
	    if (fp < (task_base + sizeof(struct task_struct)) ||
		fp >= (task_base + (2 * PAGE_SIZE)))
		break;
	    rw = (struct reg_window *) fp;
	    pc = rw->ins[7];
	    if (pc < first_sched || pc >= last_sched)
		return pc;
	    fp = rw->ins[6] + bias;
	} while (++count < 16);
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
;

static inline void
task_mem (struct task_struct *p, libgtop_proc_segment_t *proc_segment)
{
    struct mm_struct * mm = p->mm;
    
    if (mm && mm != &init_mm) {
	struct vm_area_struct * vma = mm->mmap;
	unsigned long data = 0, stack = 0;
	unsigned long exec = 0, lib = 0;
	unsigned long vsize = 0;

	for (vma = mm->mmap; vma; vma = vma->vm_next) {
	    unsigned long len = (vma->vm_end - vma->vm_start) >> 10;
	    vsize += len;
	    if (!vma->vm_file) {
		data += len;
		if (vma->vm_flags & VM_GROWSDOWN)
		    stack += len;
		continue;
	    }
	    if (vma->vm_flags & VM_WRITE)
		continue;
	    if (vma->vm_flags & VM_EXEC) {
		exec += len;
		if (vma->vm_flags & VM_EXECUTABLE)
		    continue;
		lib += len;
	    }
	}

	proc_segment->vsize = vsize;
	proc_segment->data = data;
	proc_segment->stack = stack;
	proc_segment->exec = exec;
	proc_segment->lib = lib;
    }
}

static inline void
statm_pte_range (pmd_t * pmd, unsigned long address, unsigned long size,
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

static inline void
statm_pmd_range (pgd_t * pgd, unsigned long address, unsigned long size,
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
	statm_pte_range (pmd, address, end - address, pages,
			 shared, dirty, total);
	address = (address + PMD_SIZE) & PMD_MASK;
	pmd++;
    } while (address < end);
}

static void
statm_pgd_range (pgd_t * pgd, unsigned long address, unsigned long end,
		 int * pages, int * shared, int * dirty, int * total)
{
    while (address < end) {
	statm_pmd_range (pgd, address, end - address, pages,
			 shared, dirty, total);
	address = (address + PGDIR_SIZE) & PGDIR_MASK;
		pgd++;
	}
}

static void
get_statm (struct task_struct *tsk, libgtop_proc_mem_t *proc_mem)
{
    int size=0, resident=0, share=0, trs=0, lrs=0, drs=0, dt=0;
    unsigned long data=0, stack=0, exec=0, lib=0;
    unsigned long vsize = 0;

    if (tsk->mm && tsk->mm != &init_mm) {
	struct vm_area_struct * vma = tsk->mm->mmap;

	while (vma) {
	    unsigned long len = (vma->vm_end - vma->vm_start) >> 10;
	    pgd_t *pgd = pgd_offset(tsk->mm, vma->vm_start);
	    int pages = 0, shared = 0, dirty = 0, total = 0;

	    vsize += len;

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

	    if (!vma->vm_file) {
		data += len;
		if (vma->vm_flags & VM_GROWSDOWN)
		    stack += len;
		vma = vma->vm_next;
		continue;
	    }
	    if (vma->vm_flags & VM_WRITE) {
		vma = vma->vm_next;
		continue;
	    }
	    if (vma->vm_flags & VM_EXEC) {
		exec += len;
		if (vma->vm_flags & VM_EXECUTABLE) {
		    vma = vma->vm_next;
		    continue;
		}
		lib += len;
	    }

	    vma = vma->vm_next;
	}
    }

    proc_mem->segment.vsize = vsize;    
    proc_mem->segment.data = data;
    proc_mem->segment.stack = stack;
    proc_mem->segment.exec = exec;
    proc_mem->segment.lib = lib;

    proc_mem->size = size;
    proc_mem->resident = resident;
    proc_mem->share = share;
    proc_mem->trs = trs;
    proc_mem->lrs = lrs;
    proc_mem->drs = drs;
    proc_mem->dt = dt; 
}

static void
collect_sigign_sigcatch (struct task_struct *p, sigset_t *ign,
			 sigset_t *catch)
{
    struct k_sigaction *k;
    int i;

    sigemptyset(ign);
    sigemptyset(catch);

    if (p->sig) {
	k = p->sig->action;
	for (i = 1; i <= _NSIG; ++i, ++k) {
	    if (k->sa.sa_handler == SIG_IGN)
		sigaddset(ign, i);
	    else if (k->sa.sa_handler != SIG_DFL)
		sigaddset(catch, i);
	}
    }
}

static void 
task_sig (struct task_struct *p, libgtop_proc_signal_t *proc_signal)
{
    sigset_t ignore, catch;
    int i, nsig;

    if (_NSIG_WORDS > LIBGTOP_NSIG)
	nsig = LIBGTOP_NSIG;
    else
	nsig = _NSIG_WORDS;

    collect_sigign_sigcatch (p, &ignore, &catch);

    for (i = 0; i < nsig; i++) {
	proc_signal->signal [i] = p->signal.sig [i];
	proc_signal->blocked [i] = p->blocked.sig [i];
	proc_signal->ignore [i] = ignore.sig [i];
	proc_signal->catch [i] = catch.sig [i];
    }
}

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
#if 0
	mem->cachedram = page_cache_size * PAGE_SIZE;
#endif
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
	for (index = tindex = 0; (index <= nr_tasks) && tsk->next_task;
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

	proclist->count = tindex;
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
libgtop_sysctl_proc (ctl_table *table, int nlen, int *name,
		     struct task_struct *tsk)
{
    libgtop_proc_state_t *proc_state;
    libgtop_proc_kernel_t *proc_kernel;
    libgtop_proc_segment_t *proc_segment;
    libgtop_proc_signal_t *proc_signal;
    libgtop_proc_mem_t *proc_mem;
#ifdef __SMP__
    int i;
#endif

    switch (table->ctl_name) {
    case LIBGTOP_PROC_STATE:
	proc_state = table->data;
	memset (proc_state, 0, sizeof (libgtop_proc_state_t));
	
	proc_state->uid = tsk->uid;
	proc_state->gid = tsk->gid;
	proc_state->state = tsk->state;
	proc_state->flags = tsk->flags;
	memcpy (proc_state->comm, tsk->comm, sizeof (proc_state->comm));
	proc_state->uid = tsk->uid;
	proc_state->euid = tsk->euid;
	proc_state->suid = tsk->suid;
	proc_state->fsuid = tsk->fsuid;
		
	proc_state->gid = tsk->gid;
	proc_state->egid = tsk->egid;
	proc_state->sgid = tsk->sgid;
	proc_state->fsgid = tsk->fsgid;
		
	proc_state->pid = tsk->pid;
	proc_state->pgrp = tsk->pgrp;
	proc_state->ppid = tsk->p_pptr->pid;

	proc_state->session = tsk->session;
	proc_state->tty = tsk->tty ?
	    kdev_t_to_nr (tsk->tty->device) : 0;
	proc_state->tpgid = tsk->tty ? tsk->tty->pgrp : -1;

	proc_state->priority = tsk->priority;
	proc_state->counter = tsk->counter;
	proc_state->def_priority = DEF_PRIORITY;

	proc_state->utime = tsk->times.tms_utime;
	proc_state->stime = tsk->times.tms_stime;
	proc_state->cutime = tsk->times.tms_cutime;
	proc_state->cstime = tsk->times.tms_cstime;

	proc_state->start_time = tsk->start_time;

#ifdef __SMP__
	for (i = 0; i < NR_CPUS; i++) {
	    proc_state->per_cpu_utime [i] = tsk->per_cpu_utime [i];
	    proc_state->per_cpu_stime [i] = tsk->per_cpu_stime [i];
	}
#endif

	proc_state->has_cpu = tsk->has_cpu;
	proc_state->processor = tsk->processor;
	proc_state->last_processor = tsk->last_processor;

	proc_state->policy = tsk->policy;
	proc_state->rt_priority = tsk->rt_priority;

	proc_state->it_real_value = tsk->it_real_value;
	proc_state->it_prof_value = tsk->it_prof_value;
	proc_state->it_virt_value = tsk->it_virt_value;
	proc_state->it_real_incr = tsk->it_real_incr;
	proc_state->it_prof_incr = tsk->it_prof_incr;
	proc_state->it_virt_incr = tsk->it_virt_incr;
	
	proc_state->min_flt = tsk->min_flt;
	proc_state->cmin_flt = tsk->cmin_flt;
	proc_state->maj_flt = tsk->maj_flt;
	proc_state->cmaj_flt = tsk->cmaj_flt;
		
	proc_state->nswap = tsk->nswap;
	proc_state->cnswap = tsk->cnswap;

	proc_state->kesp = KSTK_ESP(tsk);
	proc_state->keip = KSTK_EIP(tsk);

	if (tsk->mm && tsk->mm != &init_mm) {
	    proc_state->context = tsk->mm->context;
	    proc_state->start_code = tsk->mm->start_code;
	    proc_state->end_code = tsk->mm->end_code;
	    proc_state->start_data = tsk->mm-> start_data;
	    proc_state->end_data = tsk->mm->end_data;
	    proc_state->start_brk = tsk->mm->start_brk;
	    proc_state->brk = tsk->mm->brk;
	    proc_state->start_stack = tsk->mm->start_stack;
	    proc_state->start_mmap = tsk->mm->mmap ?
		tsk->mm->mmap->vm_start : 0;
	    proc_state->arg_start = tsk->mm->arg_start;
	    proc_state->arg_end = tsk->mm->arg_end;
	    proc_state->env_start = tsk->mm->env_start;
	    proc_state->env_end = tsk->mm->env_end;
	    proc_state->rss = tsk->mm->rss << PAGE_SHIFT;
	    proc_state->total_vm = tsk->mm->total_vm;
	    proc_state->locked_vm = tsk->mm->locked_vm;

	}
	proc_state->rlim = tsk->rlim ? tsk->rlim[RLIMIT_RSS].rlim_cur : 0;
	break;
    case LIBGTOP_PROC_KERNEL:
	proc_kernel = table->data;
	memset (proc_kernel, 0, sizeof (libgtop_proc_kernel_t));

	proc_kernel->wchan = get_wchan (tsk);
	break;
    case LIBGTOP_PROC_SEGMENT:
	proc_segment = table->data;
	memset (proc_segment, 0, sizeof (libgtop_proc_segment_t));

	task_mem (tsk, proc_segment);
	break;
    case LIBGTOP_PROC_MEM:
	proc_mem = table->data;
	memset (proc_mem, 0, sizeof (libgtop_proc_mem_t));

	get_statm (tsk, proc_mem);
	/* Use LIBGTOP_PROC_STAT if you only want rss and rlim. */
	proc_mem->rss = tsk->mm->rss << PAGE_SHIFT;
	proc_mem->rlim = tsk->rlim ? tsk->rlim[RLIMIT_RSS].rlim_cur : 0;
	break;
    case LIBGTOP_PROC_SIGNAL:
	proc_signal = table->data;
	memset (proc_signal, 0, sizeof (libgtop_proc_signal_t));

	task_sig (tsk, proc_signal);
	break;
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
    struct task_struct *tsk = NULL;
    int ret, len, len_name;

    if (!table->data || !table->maxlen)
	return -ENOTDIR;

    if (!oldval || !oldlenp || get_user(len, oldlenp))
	return -EFAULT;

    if (!name || !nlen || get_user(len_name, name))
	return -EFAULT;

    if (len != table->maxlen)
	return -EFAULT;

    if (nlen != 2)
	return -EFAULT;

    read_lock (&tasklist_lock);
    tsk = find_task_by_pid (name [1]);
    /* FIXME!! This should be done after the last use */
    read_unlock(&tasklist_lock);

    if (tsk == NULL)
	return -ESRCH;

    ret = libgtop_sysctl_proc (table, nlen, name, tsk);
    if (ret) return ret;

    if(copy_to_user(oldval, table->data, len))
	return -EFAULT;

    return 1;
}
