/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Joshua Sled <jsled@xcf.berkeley.edu, July 1998.

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
#include <glibtop/procstate.h>

#include <glibtop_suid.h>

#include <sys/user.h>

static const unsigned long _glibtop_sysdeps_proc_state =
(1 << GLIBTOP_PROC_STATE_CMD) + (1 << GLIBTOP_PROC_STATE_STATE) +
(1 << GLIBTOP_PROC_STATE_UID) + (1 << GLIBTOP_PROC_STATE_GID);

/* Init function. */

void
glibtop_init_proc_state_p (glibtop *server)
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

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_STATE), 0);
	
	memset (buf, 0, sizeof (glibtop_proc_state));

	glibtop_suid_enter (server);

	/* Get the process information */
	pinfo = kvm_getprocs (server->machine.kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1))
		glibtop_error_io_r (server, "kvm_getprocs (%d)", pid);

	glibtop_suid_leave (server);

	strncpy (buf->cmd, pinfo [0].kp_proc.p_comm, sizeof (buf->cmd)-1);
	buf->cmd [sizeof (buf->cmd)-1] = 0;

	buf->uid = pinfo [0].kp_eproc.e_pcred.p_svuid;
	buf->gid = pinfo [0].kp_eproc.e_pcred.p_svgid;

	switch (pinfo [0].kp_proc.p_stat) {
	case SIDL:
		buf->state = 'I';
		break;
	case SRUN:
		buf->state = 'R';
		break;
	case SSLEEP:
		buf->state = 'S';
		break;
	case SSTOP:
		buf->state = 'T';
		break;
	case SZOMB:
		buf->state = 'Z';
		break;
	default:
		buf->state = '?';
		break;
	}

	/* Set the flags for the data we're about to return*/
	buf->flags = _glibtop_sysdeps_proc_state;
}
