/*
   This file is part of LibGTop 2.0.

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
#include <unistd.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procuid.h>

#include <glibtop_suid.h>

#include <sys/param.h>
#include <sys/sysctl.h>

static const unsigned long _glibtop_sysdeps_proc_uid =
(1L << GLIBTOP_PROC_UID_UID) + (1L << GLIBTOP_PROC_UID_EUID) +
(1L << GLIBTOP_PROC_UID_GID) + (1L << GLIBTOP_PROC_UID_EGID) +
(1L << GLIBTOP_PROC_UID_PID) + (1L << GLIBTOP_PROC_UID_PPID) +
(1L << GLIBTOP_PROC_UID_PGRP) + (1L << GLIBTOP_PROC_UID_TTY) +
(1L << GLIBTOP_PROC_UID_TPGID);

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
	int mib[4];
	size_t length;
	struct kinfo_proc kinfo;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_UID), 0);
	
	memset (buf, 0, sizeof (glibtop_proc_uid));

	mib [0] = CTL_KERN;
	mib [1] = KERN_PROC;
	mib [2] = KERN_PROC_PID;
	mib [3] = pid;

	length = sizeof (struct kinfo_proc);
	if (sysctl (mib, 4, &kinfo, &length, NULL, 0) < 0) {
		glibtop_warn_io_r (server, "sysctl (procuid)");
		return;
	}

	buf->uid      = kinfo.kp_eproc.e_pcred.p_ruid;
	buf->euid     = kinfo.kp_eproc.e_pcred.p_svuid;
	buf->gid      = kinfo.kp_eproc.e_pcred.p_rgid;
	buf->egid     = kinfo.kp_eproc.e_pcred.p_svgid;
	buf->ppid     = kinfo.kp_eproc.e_ppid;
	buf->pgrp     = kinfo.kp_eproc.e_pgid;
	buf->tpgid    = kinfo.kp_eproc.e_tpgid;
	buf->nice     = kinfo.kp_proc.p_nice;
	buf->priority = kinfo.kp_proc.p_priority;

	buf->flags    = _glibtop_sysdeps_proc_uid;
}
