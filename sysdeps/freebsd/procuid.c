/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

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
#include <glibtop/procuid.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_proc_uid =
(1L << GLIBTOP_PROC_UID_UID) + (1L << GLIBTOP_PROC_UID_EUID) +
(1L << GLIBTOP_PROC_UID_EGID) + (1L << GLIBTOP_PROC_UID_PID) +
(1L << GLIBTOP_PROC_UID_PPID) + (1L << GLIBTOP_PROC_UID_PGRP) +
(1L << GLIBTOP_PROC_UID_TPGID) + (1L << GLIBTOP_PROC_UID_PRIORITY) +
(1L << GLIBTOP_PROC_UID_NICE) + (1L << GLIBTOP_PROC_UID_GID);

/* Init function. */

void
_glibtop_init_proc_uid_p (glibtop *server)
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

	memset (buf, 0, sizeof (glibtop_proc_uid));

	/* It does not work for the swapper task. */
	if (pid == 0) return;

	/* Get the process information */
	pinfo = kvm_getprocs (server->machine->kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1)) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		return;
	}

#define	PROC_RUID	ki_ruid
#define	PROC_EUID	ki_uid
#define	PROC_RGID	ki_rgid
#define	PROC_SVGID	ki_svgid
#define	PROC_PPID	ki_ppid
#define	PROC_PGID	ki_pgid
#define	PROC_TPGID	ki_tpgid
#define	PROC_NICE	ki_nice
#define	PROC_PRIORITY	ki_pri.pri_user

	buf->uid  = pinfo [0].PROC_RUID;
	buf->euid = pinfo [0].PROC_EUID;
	buf->gid  = pinfo [0].PROC_RGID;
	buf->egid = pinfo [0].PROC_SVGID;

	buf->ppid  = pinfo [0].PROC_PPID;
	buf->pgrp  = pinfo [0].PROC_PGID;
	buf->tpgid = pinfo [0].PROC_TPGID;

	buf->nice     = pinfo [0].PROC_NICE;
	buf->priority = pinfo [0].PROC_PRIORITY;

	buf->flags = _glibtop_sysdeps_proc_uid;
}
