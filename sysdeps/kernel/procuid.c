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

#include "kernel.h"

static const unsigned long _glibtop_sysdeps_proc_uid =
(1 << GLIBTOP_PROC_UID_UID) + (1 << GLIBTOP_PROC_UID_EUID) +
(1 << GLIBTOP_PROC_UID_GID) + (1 << GLIBTOP_PROC_UID_EGID) +
(1 << GLIBTOP_PROC_UID_PID) + (1 << GLIBTOP_PROC_UID_PPID) +
(1 << GLIBTOP_PROC_UID_PGRP) + (1 << GLIBTOP_PROC_UID_SESSION) +
(1 << GLIBTOP_PROC_UID_TTY) + (1 << GLIBTOP_PROC_UID_TPGID) +
(1 << GLIBTOP_PROC_UID_PRIORITY) + (1 << GLIBTOP_PROC_UID_NICE);

/* Init function. */

void
glibtop_init_proc_uid_s (glibtop *server)
{
	server->sysdeps.proc_uid = _glibtop_sysdeps_proc_uid;
}

#define LINUX_VERSION(x,y,z)	(0x10000*(x) + 0x100*(y) + z)

/* Provides detailed information about a process. */

void
glibtop_get_proc_uid_s (glibtop *server, glibtop_proc_uid *buf,
			pid_t pid)
{
	union table tbl;
	long def_priority, priority, nice;
	
	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_UID, 0);

	memset (buf, 0, sizeof (glibtop_proc_uid));

	if (table (TABLE_PROC_UID, &tbl, &pid))
	  glibtop_error_io_r (server, "table(TABLE_PROC_UID)");

	buf->flags = _glibtop_sysdeps_proc_uid;

	buf->uid = tbl.proc_uid.uid;
	buf->euid = tbl.proc_uid.euid;
	buf->gid = tbl.proc_uid.gid;
	buf->egid = tbl.proc_uid.egid;

	buf->pid = tbl.proc_uid.pid;
	buf->ppid = tbl.proc_uid.ppid;
	buf->pgrp = tbl.proc_uid.pgrp;
	buf->session = tbl.proc_uid.session;
	buf->tty = tbl.proc_uid.tty;
	buf->tpgid = tbl.proc_uid.tpgid;

	/* scale priority and nice values from timeslices to -20..20 */
	/* to make it look like a "normal" unix priority/nice value  */

	def_priority = tbl.proc_uid.def_priority;

	priority = tbl.proc_uid.counter;
	priority = 20 - (priority * 10 + def_priority / 2) / def_priority;
	nice = tbl.proc_uid.priority;
	nice = 20 - (nice * 20 + def_priority / 2) / def_priority;

	buf->priority = (int) priority;
	buf->nice = (int) nice;

	if (buf->tty == 0)
		/* the old notty val, update elsewhere bef. moving to 0 */
		buf->tty = -1;
	
	if (server->os_version_code < LINUX_VERSION(1,3,39)) {
		/* map old meanings to new */
		buf->priority = 2*15 - buf->priority;
		buf->nice = 15 - buf->nice;
	}
	if (server->os_version_code < LINUX_VERSION(1,1,30) && buf->tty != -1)
		/* when tty wasn't full devno */
		buf->tty = 4*0x100 + buf->tty;
	
	buf->flags = _glibtop_sysdeps_proc_uid;
}
