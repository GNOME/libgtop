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
#include <glibtop/procstate.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proc_state =
(1 << GLIBTOP_PROC_STATE_UID) + (1 << GLIBTOP_PROC_STATE_GID) +
(1 << GLIBTOP_PROC_STATE_CMD);

/* Init function. */

void
glibtop_init_proc_state_s (glibtop *server)
{
	server->sysdeps.proc_state = _glibtop_sysdeps_proc_state;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_state_s (glibtop *server, glibtop_proc_state *buf, pid_t pid)
{
	struct psinfo psinfo;

	memset (buf, 0, sizeof (glibtop_proc_state));

	if (glibtop_get_proc_data_psinfo_s (server, &psinfo, pid))
		return;

	buf->uid = psinfo.pr_uid;
	buf->gid = psinfo.pr_gid;

	strncpy (buf->cmd, psinfo.pr_fname, 39);

	buf->flags = _glibtop_sysdeps_proc_state;
}
