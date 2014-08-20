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
#include <procinfo.h>
#include <sys/thread.h>

#include <glibtop.h>
#include <glibtop/procstate.h>
#include <glibtop/error.h>

#include "utils.h"

static const unsigned long _glibtop_sysdeps_proc_state =
(1 << GLIBTOP_PROC_STATE_CMD) + (1 << GLIBTOP_PROC_STATE_STATE) +
(1 << GLIBTOP_PROC_STATE_UID) + (1 << GLIBTOP_PROC_STATE_GID);

/* Init function. */

void
_glibtop_init_proc_state_s (glibtop *server)
{
	server->sysdeps.proc_state = _glibtop_sysdeps_proc_state;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_state_s (glibtop *server, glibtop_proc_state *buf,
			  pid_t pid)
{
	struct procsinfo *pinfo;
	struct thrdsinfo thinfo;
	tid_t thid;
	int result;

	glibtop_init_s (&server, (1L << GLIBTOP_SYSDEPS_PROC_STATE), 0);

	memset (buf, 0, sizeof (glibtop_proc_state));

	pinfo = _glibtop_get_procinfo(server, pid);
	if (pinfo == NULL) return;

	if (pinfo->pi_pid == 0)
	{
		strcpy(buf->cmd, "swapper");
	}

	if (pinfo->pi_comm[0] == 0)
	{
		strcpy(buf->cmd, "<unknown>");
	}
	else
	{
		g_strlcpy(buf->cmd, pinfo->pi_comm, sizeof buf->cmd);
	}

	buf->uid = pinfo->pi_uid;
	buf->gid = pinfo->pi_cred.cr_gid;

	if (pinfo->pi_state == SZOMB)
	{
		buf->state = GLIBTOP_PROCESS_ZOMBIE;
		strcpy(buf->cmd, "<defunct>");
	}
	else
	{
		/* get state of first thread */

		thid = 0;
		result = getthrds(pid, &thinfo, sizeof(thinfo), &thid, 1);
		if (result != 1)
		{
			glibtop_error_io_r(server, "Cannot read threadinfo");
		}

		switch (thinfo.ti_state)
		{
			case TSIDL:
				/* FIXME */
				/* buf->state = GLIBTOP_PROCESS_UNINTERRUPTIBLE; */
				break;
			case TSRUN:
				buf->state = GLIBTOP_PROCESS_RUNNING;
				break;
			case TSSLEEP:
				buf->state = GLIBTOP_PROCESS_INTERRUPTIBLE;
				break;
			case TSZOMB:
				buf->state = GLIBTOP_PROCESS_ZOMBIE;
				strcpy(buf->cmd, "<defunct>");
				break;
			case TSSTOP:
				buf->state = GLIBTOP_PROCESS_STOPPED;
				break;
			case TSSWAP:
				buf->state = GLIBTOP_PROCESS_SWAPPING;
				break;
			default:
				buf->state = 0;
				break;
		}
	}

	buf->flags = _glibtop_sysdeps_proc_state;
}
