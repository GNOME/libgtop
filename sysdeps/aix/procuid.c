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
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <config.h>
#include <unistd.h>
#include <sys/vminfo.h>
#include <sys/resource.h>

#include <glibtop.h>
#include <glibtop/procuid.h>
#include <glibtop/error.h>

#include "utils.h"

static const unsigned long _glibtop_sysdeps_proc_uid =
(1 << GLIBTOP_PROC_UID_UID) + (1 << GLIBTOP_PROC_UID_EUID) +
(1 << GLIBTOP_PROC_UID_GID) + (1 << GLIBTOP_PROC_UID_EGID) +
(1 << GLIBTOP_PROC_UID_PID) + (1 << GLIBTOP_PROC_UID_PPID) +
(1 << GLIBTOP_PROC_UID_PGRP) + (1 << GLIBTOP_PROC_UID_SESSION) +
(1 << GLIBTOP_PROC_UID_TTY) + (1 << GLIBTOP_PROC_UID_PRIORITY) +
(1 << GLIBTOP_PROC_UID_NICE);


/* Init function. */

void
_glibtop_init_proc_uid_s (glibtop *server)
{
	server->sysdeps.proc_uid = _glibtop_sysdeps_proc_uid;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_uid_s (glibtop *server, glibtop_proc_uid *buf,
			pid_t pid)
{
	struct procsinfo *pinfo;
	struct thrdsinfo thinfo;
	tid_t thid;
	int result;

	glibtop_init_s (&server, (1L << GLIBTOP_SYSDEPS_PROC_UID), 0);

	memset (buf, 0, sizeof (glibtop_proc_uid));

	pinfo = _glibtop_get_procinfo(server, pid);
	if (pinfo == NULL) return;

	buf->uid = pinfo->pi_cred.cr_ruid;
	buf->euid =  pinfo->pi_cred.cr_uid;
	buf->gid =  pinfo->pi_cred.cr_rgid;
	buf->egid =  pinfo->pi_cred.cr_gid;

	buf->pid = pinfo->pi_pid;
	buf->ppid = pinfo->pi_ppid;

	buf->pgrp = pinfo->pi_pgrp;
	buf->session = pinfo->pi_sid;

	buf->tty = pinfo->pi_ttyd;

	buf->nice = pinfo->pi_nice;

	buf->flags = _glibtop_sysdeps_proc_uid;

	thid = 0;
	result = getthrds(pid, &thinfo, sizeof(thinfo), &thid, 1);
	if (result != 1)
	{
		buf->flags &= ~(1 << GLIBTOP_PROC_UID_PRIORITY);

		glibtop_error_io_r(server, "Cannot read thrdsinfo");
	}

	/* priority of first thread */

	buf->priority = thinfo.ti_pri;
}
