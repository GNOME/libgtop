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
#include <glibtop/error.h>
#include <glibtop/procstate.h>

#include "kernel.h"

static const unsigned long _glibtop_sysdeps_proc_state =
(1 << GLIBTOP_PROC_STATE_CMD) + (1 << GLIBTOP_PROC_STATE_STATE) +
(1 << GLIBTOP_PROC_STATE_UID) + (1 << GLIBTOP_PROC_STATE_GID);

/* Init function. */

void
glibtop_init_proc_state_s (glibtop *server)
{
	server->sysdeps.proc_state = _glibtop_sysdeps_proc_state;
}

/* Provides detailed information about a process. */

#define NR_STATES	7

void
glibtop_get_proc_state_s (glibtop *server, glibtop_proc_state *buf,
			  pid_t pid)
{
	union table tbl;
	static const char states [NR_STATES] =
	{ 'R', 'S', 'D', 'Z', 'T', 'W', '.' };
	unsigned state;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_STATE, 0);

	memset (buf, 0, sizeof (glibtop_proc_state));

	if (table (TABLE_PROC_STATE, &tbl, &pid))
		glibtop_error_io_r (server, "table(TABLE_PROC_STATE)");

	buf->flags = _glibtop_sysdeps_proc_state;

	memcpy (buf->cmd, tbl.proc_state.comm, sizeof (tbl.proc_state.comm));

	buf->uid = tbl.proc_state.uid;
	buf->gid = tbl.proc_state.state;

	state = (unsigned) tbl.proc_state.state;

	if (state >= NR_STATES)
		state = NR_STATES;

	buf->state = states [state];
}
