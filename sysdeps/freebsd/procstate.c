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
#include <glibtop/procstate.h>

#include <glibtop_suid.h>

#include <sys/user.h>

static const unsigned long _glibtop_sysdeps_proc_state =
(1L << GLIBTOP_PROC_STATE_CMD) + (1L << GLIBTOP_PROC_STATE_STATE) +
(1L << GLIBTOP_PROC_STATE_UID) + (1L << GLIBTOP_PROC_STATE_GID);

/* Init function. */

void
_glibtop_init_proc_state_p (glibtop *server)
{
	server->sysdeps.proc_state = _glibtop_sysdeps_proc_state;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_state_p (glibtop *server,
			  glibtop_proc_state *buf,
			  pid_t pid)
{
	struct kinfo_proc *pinfo;
	int count = 0;

	memset (buf, 0, sizeof (glibtop_proc_state));

	/* Get the process information */
	pinfo = kvm_getprocs (server->machine->kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1)) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		return;
	}

	buf->uid = pinfo[0].ki_ruid;
	buf->gid = pinfo[0].ki_rgid;

	g_strlcpy (buf->cmd, pinfo[0].ki_comm, sizeof (buf->cmd));

	switch (pinfo[0].ki_stat) {
		case SRUN:
			buf->state = GLIBTOP_PROCESS_RUNNING;
			break;
		case SSLEEP:
			buf->state = GLIBTOP_PROCESS_INTERRUPTIBLE;
			break;
		case SSTOP:
			buf->state = GLIBTOP_PROCESS_STOPPED;
			break;
		case SZOMB:
			buf->state = GLIBTOP_PROCESS_ZOMBIE;
			break;
		case SWAIT:
		case SLOCK:
			buf->state = GLIBTOP_PROCESS_UNINTERRUPTIBLE;
			break;
		case SIDL:
		default:
			buf->state = 0;
			break;
	}

	buf->flags = _glibtop_sysdeps_proc_state;
}
