/* $Id$ */

/* Copyright (C) 1998 Joshua Sled
   This file is part of LibGTop 1.0.

   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   LibGTop is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with LibGTop; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/prockernel.h>

#include <glibtop_suid.h>

#include <kvm.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/proc.h>
#if (!defined __OpenBSD__) && (!defined __bsdi__)
#include <sys/user.h>
#endif
#ifndef __bsdi__
#include <machine/pcb.h>
#endif
#ifdef __FreeBSD__
#include <machine/tss.h>
#endif

#include <unistd.h>
#include <fcntl.h>

#ifdef __FreeBSD__
#include <osreldate.h>
#endif

static const unsigned long _glibtop_sysdeps_proc_kernel_pstats =
(1L << GLIBTOP_PROC_KERNEL_MIN_FLT) +
(1L << GLIBTOP_PROC_KERNEL_MAJ_FLT) +
(1L << GLIBTOP_PROC_KERNEL_CMIN_FLT) +
(1L << GLIBTOP_PROC_KERNEL_CMAJ_FLT);

static const unsigned long _glibtop_sysdeps_proc_kernel_pcb =
(1L << GLIBTOP_PROC_KERNEL_KSTK_EIP) +
(1L << GLIBTOP_PROC_KERNEL_KSTK_ESP);

static const unsigned long _glibtop_sysdeps_proc_kernel_wchan =
(1L << GLIBTOP_PROC_KERNEL_NWCHAN) +
(1L << GLIBTOP_PROC_KERNEL_WCHAN);

/* Init function. */

void
glibtop_init_proc_kernel_p (glibtop *server)
{
	server->sysdeps.proc_kernel = _glibtop_sysdeps_proc_kernel_pstats |
		_glibtop_sysdeps_proc_kernel_pcb |
		_glibtop_sysdeps_proc_kernel_wchan;
}

void
glibtop_get_proc_kernel_p (glibtop *server,
			   glibtop_proc_kernel *buf,
			   pid_t pid)
{
	struct kinfo_proc *pinfo;
	struct user *u_addr = (struct user *)USRSTACK;
	struct pstats pstats;
	struct pcb pcb;
	int count;

	char filename [BUFSIZ];
	struct stat statb;

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_PROC_KERNEL), 0);
	
	memset (buf, 0, sizeof (glibtop_proc_kernel));

	if (server->sysdeps.proc_time == 0)
		return;

	/* It does not work for the swapper task. */
	if (pid == 0) return;
	
	/* Get the process information */
	pinfo = kvm_getprocs (server->machine.kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1))
		glibtop_error_io_r (server, "kvm_getprocs (%d)", pid);

	buf->nwchan = (unsigned long) pinfo [0].kp_proc.p_wchan &~ KERNBASE;
	buf->flags |= (1L << GLIBTOP_PROC_KERNEL_NWCHAN);

	if (pinfo [0].kp_proc.p_wchan && pinfo [0].kp_proc.p_wmesg) {
		strncpy (buf->wchan, pinfo [0].kp_eproc.e_wmesg,
			 sizeof (buf->wchan) - 1);
		buf->wchan [sizeof (buf->wchan) - 1] = 0;
		buf->flags |= (1L << GLIBTOP_PROC_KERNEL_WCHAN);
	} else {
		buf->wchan [0] = 0;
	}

	/* Taken from `saveuser ()' in `/usr/src/bin/ps/ps.c'. */

	/* [FIXME]: /usr/include/sys/user.h tells me that the user area
	 *          may or may not be at the same kernel address in all
	 *          processes, but I don't see any way to get that address.
	 *          Since `ps' simply uses its own address, I think it's
	 *          safe to do this here, too. */

	/* NOTE: You need to mount the /proc filesystem to make
	 *       `kvm_uread' work. */

	sprintf (filename, "/proc/%d/mem", (int) pid);
	if (stat (filename, &statb)) return;

	glibtop_suid_enter (server);

	if ((pinfo [0].kp_proc.p_flag & P_INMEM) &&
	    kvm_uread (server->machine.kd, &(pinfo [0]).kp_proc,
		       (unsigned long) &u_addr->u_stats,
		       (char *) &pstats, sizeof (pstats)) == sizeof (pstats))
		{
			/*
			 * The u-area might be swapped out, and we can't get
			 * at it because we have a crashdump and no swap.
			 * If it's here fill in these fields, otherwise, just
			 * leave them 0.
			 */

			buf->min_flt = (u_int64_t) pstats.p_ru.ru_minflt;
			buf->maj_flt = (u_int64_t) pstats.p_ru.ru_majflt;
			buf->cmin_flt = (u_int64_t) pstats.p_cru.ru_minflt;
			buf->cmaj_flt = (u_int64_t) pstats.p_cru.ru_majflt;
			
			buf->flags |= _glibtop_sysdeps_proc_kernel_pstats;
		}

	if ((pinfo [0].kp_proc.p_flag & P_INMEM) &&
	    kvm_uread (server->machine.kd, &(pinfo [0]).kp_proc,
		       (unsigned long) &u_addr->u_pcb,
		       (char *) &pcb, sizeof (pcb)) == sizeof (pcb))
		{
#ifdef __FreeBSD__
#if (__FreeBSD_version >= 300003)
			buf->kstk_esp = (u_int64_t) pcb.pcb_esp;
			buf->kstk_eip = (u_int64_t) pcb.pcb_eip;
#else
			buf->kstk_esp = (u_int64_t) pcb.pcb_ksp;
			buf->kstk_eip = (u_int64_t) pcb.pcb_pc;
#endif
#else
			buf->kstk_esp = (u_int64_t) pcb.pcb_tss.tss_esp0;
#ifdef __bsdi__
			buf->kstk_eip = (u_int64_t) pcb.pcb_tss.tss_eip;
#else
			buf->kstk_eip = (u_int64_t) pcb.pcb_tss.__tss_eip;
#endif

			buf->flags |= _glibtop_sysdeps_proc_kernel_pcb;
#endif
		}

	/* Taken from `wchan ()' in `/usr/src/bin/ps/print.c'. */

	glibtop_suid_leave (server);
}
