/* $Id$ */

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

#include <glibtop.h>
#include <glibtop/procuid.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proc_uid =
(1 << GLIBTOP_PROC_UID_EUID) + (1 << GLIBTOP_PROC_UID_UID) +
(1 << GLIBTOP_PROC_UID_EGID) + (1 << GLIBTOP_PROC_UID_GID) +
(1 << GLIBTOP_PROC_UID_PID) + (1 << GLIBTOP_PROC_UID_PPID) +
(1 << GLIBTOP_PROC_UID_PGRP) + (1 << GLIBTOP_PROC_UID_SESSION) +
(1 << GLIBTOP_PROC_UID_TTY);

/* Init function. */

void
glibtop_init_proc_uid_s (glibtop *server)
{
	server->sysdeps.proc_uid = _glibtop_sysdeps_proc_uid;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_uid_s (glibtop *server, glibtop_proc_uid *buf, pid_t pid)
{
	struct psinfo psinfo;

	memset (buf, 0, sizeof (glibtop_proc_uid));

	if (glibtop_get_proc_data_psinfo_s (server, &psinfo, pid))
		return;

	buf->euid = psinfo.pr_euid;
	buf->uid = psinfo.pr_uid;
	buf->egid = psinfo.pr_egid;
	buf->gid = psinfo.pr_gid;

	buf->pid = psinfo.pr_pid;
	buf->ppid = psinfo.pr_ppid;
	buf->pgrp = psinfo.pr_pgid;

	buf->session = psinfo.pr_sid;
	buf->tty = psinfo.pr_ttydev;

	buf->flags = _glibtop_sysdeps_proc_uid;
}
