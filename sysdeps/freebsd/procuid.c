/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

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
#include <glibtop/procuid.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_proc_uid =
(1 << GLIBTOP_PROC_UID_UID) + (1 << GLIBTOP_PROC_UID_EUID) +
(1 << GLIBTOP_PROC_UID_EGID) + (1 << GLIBTOP_PROC_UID_PID) +
(1 << GLIBTOP_PROC_UID_PPID) + (1 << GLIBTOP_PROC_UID_PGRP) +
(1 << GLIBTOP_PROC_UID_TPGID) + (1 << GLIBTOP_PROC_UID_PRIORITY) +
(1 << GLIBTOP_PROC_UID_NICE);

/* Init function. */

void
glibtop_init_proc_uid_p (glibtop *server)
{
	server->sysdeps.proc_uid = _glibtop_sysdeps_proc_uid;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_uid_p (glibtop *server, glibtop_proc_uid *buf,
			pid_t pid)
{
	struct kinfo_proc *pinfo;
	int count = 0;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_UID), 0);
	
	memset (buf, 0, sizeof (glibtop_proc_uid));

	/* It does not work for the swapper task. */
	if (pid == 0) return;
	
	/* Get the process information */
	pinfo = kvm_getprocs (server->machine.kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1)) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		return;
	}

	buf->uid  = pinfo [0].kp_eproc.e_pcred.p_ruid;
	buf->euid = pinfo [0].kp_eproc.e_pcred.p_svuid;
	buf->gid  = pinfo [0].kp_eproc.e_pcred.p_rgid;
	buf->egid = pinfo [0].kp_eproc.e_pcred.p_svgid;

	buf->ppid  = pinfo [0].kp_eproc.e_ppid;
	buf->pgrp  = pinfo [0].kp_eproc.e_pgid;
	buf->tpgid = pinfo [0].kp_eproc.e_tpgid;

	buf->nice     = pinfo [0].kp_proc.p_nice;
	buf->priority = pinfo [0].kp_proc.p_priority;

	/* Set the flags for the data we're about to return*/
	buf->flags = _glibtop_sysdeps_proc_uid;

}
