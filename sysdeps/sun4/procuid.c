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
#include <glibtop/procuid.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_proc_uid =
(1 << GLIBTOP_PROC_UID_UID) + (1 << GLIBTOP_PROC_UID_EUID) +
(1 << GLIBTOP_PROC_UID_EGID) + (1 << GLIBTOP_PROC_UID_PID) +
(1 << GLIBTOP_PROC_UID_PPID) + (1 << GLIBTOP_PROC_UID_PGRP) +
(1 << GLIBTOP_PROC_UID_TPGID) + (1 << GLIBTOP_PROC_UID_PRIORITY) +
(1 << GLIBTOP_PROC_UID_NICE);

/* Provides detailed information about a process. */

void
glibtop_get_proc_uid_p (glibtop *server, glibtop_proc_uid *buf,
			pid_t pid)
{
	struct proc *pp;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_UID), 0);

	memset (buf, 0, sizeof (glibtop_proc_uid));

	/* Read process table from kernel. */

	_glibtop_read_proc_table (server);

	/* Find the pid in the process table. */

	pp = _glibtop_find_pid (server, pid);

	if (pp == NULL)	return;

	/* Fill in data fields. */

	buf->uid = pp->p_uid;
	buf->euid = pp->p_suid;
	buf->egid = pp->p_sgid;
	buf->pid = pp->p_pid;
	buf->ppid = pp->p_ppid;
	buf->pgrp = pp->p_pgrp;
	buf->tpgid = pp->p_pgrp;
	buf->priority = pp->p_pri;
	buf->nice = pp->p_nice;

	buf->flags = _glibtop_sysdeps_proc_uid;
}
