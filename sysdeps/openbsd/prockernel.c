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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/prockernel.h>

#include <glibtop_suid.h>

#include <kvm.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/proc.h>
#include <unistd.h>
#include <fcntl.h>

static const unsigned long _glibtop_sysdeps_proc_kernel_pstats =
(1L << GLIBTOP_PROC_KERNEL_K_FLAGS) +
(1L << GLIBTOP_PROC_KERNEL_MIN_FLT) +
(1L << GLIBTOP_PROC_KERNEL_MAJ_FLT);

static const unsigned long _glibtop_sysdeps_proc_kernel_wchan =
(1L << GLIBTOP_PROC_KERNEL_WCHAN) +
(1L << GLIBTOP_PROC_KERNEL_NWCHAN);

/* Init function. */

void
_glibtop_init_proc_kernel_p (glibtop *server)
{
	server->sysdeps.proc_kernel = _glibtop_sysdeps_proc_kernel_pstats |
		_glibtop_sysdeps_proc_kernel_wchan;
}

void
glibtop_get_proc_kernel_p (glibtop *server,
			   glibtop_proc_kernel *buf,
			   pid_t pid)
{
	struct kinfo_proc *pinfo;
	int count;

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_PROC_KERNEL), 0);

	memset (buf, 0, sizeof (glibtop_proc_kernel));

	if (server->sysdeps.proc_time == 0)
		return;

	/* It does not work for the swapper task. */
	if (pid == 0) return;

	glibtop_suid_enter (server);

	/* Get the process information */
	pinfo = kvm_getprocs (server->machine->kd, KERN_PROC_PID, pid,
			      sizeof(*pinfo), &count);
	if (pinfo == NULL) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		glibtop_suid_leave (server);
		return;
	}

	glibtop_suid_leave (server);

#define	PROC_WCHAN	p_wchan
#define	PROC_WMESG	p_wmesg

	buf->nwchan = (unsigned long) pinfo [0].PROC_WCHAN;

	buf->flags |= (1L << GLIBTOP_PROC_KERNEL_NWCHAN);

	if (pinfo [0].PROC_WCHAN && pinfo [0].PROC_WMESG[0] != 0) {
		g_strlcpy (buf->wchan, pinfo [0].PROC_WMESG,
			 sizeof buf->wchan);
		buf->flags |= (1L << GLIBTOP_PROC_KERNEL_WCHAN);
	} else {
		buf->wchan [0] = 0;
	}

	buf->k_flags = (unsigned long) pinfo [0].p_flag;
	buf->min_flt = (unsigned long) pinfo [0].p_uru_minflt;
	buf->maj_flt = (unsigned long) pinfo [0].p_uru_majflt;

	buf->flags |= _glibtop_sysdeps_proc_kernel_pstats;

	return;
}
