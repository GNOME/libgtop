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
(1 << GLIBTOP_PROC_UID_UID) + (1 << GLIBTOP_PROC_UID_EUID) +
(1 << GLIBTOP_PROC_UID_GID) + (1 << GLIBTOP_PROC_UID_EGID) +
(1 << GLIBTOP_PROC_UID_SUID) + (1 << GLIBTOP_PROC_UID_SGID) +
(1 << GLIBTOP_PROC_UID_FSUID) + (1 << GLIBTOP_PROC_UID_FSGID) +
(1 << GLIBTOP_PROC_UID_PID) + (1 << GLIBTOP_PROC_UID_PPID) +
(1 << GLIBTOP_PROC_UID_PGRP) + (1 << GLIBTOP_PROC_UID_SESSION) +
(1 << GLIBTOP_PROC_UID_TTY) + (1 << GLIBTOP_PROC_UID_TPGID) +
(1 << GLIBTOP_PROC_UID_PRIORITY) + (1 << GLIBTOP_PROC_UID_NICE) +
(1 << GLIBTOP_PROC_UID_NGROUPS) + (1 << GLIBTOP_PROC_UID_GROUPS);

#ifndef min
#define min(a,b) ((a < b) ? a : b)
#endif

/* Init function. */

void
glibtop_init_proc_uid_s (glibtop *server)
{
    server->sysdeps.proc_uid = _glibtop_sysdeps_proc_uid;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_uid_s (glibtop *server, glibtop_proc_uid *buf,
			pid_t pid)
{
    libgtop_proc_state_t proc_state;
    long priority, nice;
    int i;

    memset (buf, 0, sizeof (glibtop_proc_uid));

    if (glibtop_get_proc_data_proc_state_s (server, &proc_state, pid))
	return;

    buf->uid = proc_state.uid;
    buf->euid = proc_state.euid;
    buf->gid = proc_state.gid;
    buf->egid = proc_state.egid;

    buf->suid = proc_state.suid;
    buf->sgid = proc_state.sgid;
    buf->fsuid = proc_state.fsuid;
    buf->fsgid = proc_state.fsgid;

    buf->pid = proc_state.pid;
    buf->ppid = proc_state.ppid;
    buf->pgrp = proc_state.pgrp;

    buf->session = proc_state.session;
    buf->tty = proc_state.tty;
    buf->tpgid = proc_state.tpgid;

    priority = proc_state.counter;
    priority = 20 - (priority * 10 + proc_state.def_priority / 2) /
	proc_state.def_priority;
    nice = proc_state.priority;
    nice = 20 - (nice * 20 + proc_state.def_priority / 2) /
	proc_state.def_priority;

    buf->priority = priority;
    buf->nice = nice;

    buf->ngroups = min (proc_state.ngroups, GLIBTOP_MAX_GROUPS);
    for (i = 0; i < buf->ngroups; i++)
	buf->groups [i] = proc_state.groups [i];

    buf->flags = _glibtop_sysdeps_proc_uid;
}
