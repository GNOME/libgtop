/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/prockernel.h>

#include <glibtop_suid.h>

#include <kvm.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/proc.h>
#include <sys/user.h>
#include <machine/pcb.h>
#include <machine/tss.h>

#include <unistd.h>
#include <fcntl.h>

static const unsigned long _glibtop_sysdeps_proc_kernel =
(1 << GLIBTOP_PROC_KERNEL_K_FLAGS) +
(1 << GLIBTOP_PROC_KERNEL_KSTK_ESP) +
(1 << GLIBTOP_PROC_KERNEL_KSTK_EIP) +
(1 << GLIBTOP_PROC_KERNEL_WCHAN);

static const unsigned long _glibtop_sysdeps_proc_kernel_user =
(1 << GLIBTOP_PROC_KERNEL_MIN_FLT) +
(1 << GLIBTOP_PROC_KERNEL_MAJ_FLT) +
(1 << GLIBTOP_PROC_KERNEL_CMIN_FLT) +
(1 << GLIBTOP_PROC_KERNEL_CMAJ_FLT);

/* Init function. */

void
glibtop_init_proc_kernel_p (glibtop *server)
{
	server->sysdeps.proc_kernel = _glibtop_sysdeps_proc_kernel;
}

void
glibtop_get_proc_kernel_p (glibtop *server,
			   glibtop_proc_kernel *buf,
			   pid_t pid)
{
	struct kinfo_proc *pinfo;
	struct i386tss *pcb_tss;
	struct user *u_addr = (struct user *)USRSTACK;
	struct pstats pstats;
	int f, count;

	glibtop_init_p (server, GLIBTOP_SYSDEPS_PROC_KERNEL, 0);
	
	memset (buf, 0, sizeof (glibtop_proc_kernel));

	/* Get the process information */
	pinfo = kvm_getprocs (server->machine.kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1))
		glibtop_error_io_r (server, "kvm_getprocs (%d)", pid);

	/* Taken from `saveuser ()' in `/usr/src/bin/ps/ps.c'. */

	if ((pinfo [0].kp_proc.p_flag & P_INMEM) &&
	    kvm_uread (server->machine.kd, pinfo [0].kp_proc,
		       (unsigned long) &u_addr->u_stats,
		       (char *) &pstats, sizeof (pstats)) == sizeof (pstats)) {
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

		buf->flags |= _glibtop_sysdeps_proc_kernel_user;
	}

#if 0
	/* kstk_esp: pcb_tss.tss_esp */
	buf->kstk_esp = (u_int64_t) usr.u_pcb.pcb_ksp;
	/* kstk_eip: pcb_tss.tss_eip */
	buf->kstk_eip = (u_int64_t) usr.u_pcb.pcb_pc;
#endif
	
	/* wchan : kinfo_proc.proc.p_wchan */
	buf->wchan = (u_int64_t) pinfo [0].kp_proc.p_wchan;
}
