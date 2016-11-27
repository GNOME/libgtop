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
#include <glibtop/error.h>
#include <glibtop/procstate.h>

#include "glibtop_private.h"

#include <sys/stat.h>

static const unsigned long _glibtop_sysdeps_proc_state =
(1L << GLIBTOP_PROC_STATE_CMD) + (1L << GLIBTOP_PROC_STATE_STATE);

static const unsigned long _glibtop_sysdeps_proc_state_uid =
(1L << GLIBTOP_PROC_STATE_UID) + (1L << GLIBTOP_PROC_STATE_GID);

/* Init function. */

void
_glibtop_init_proc_state_s (glibtop *server)
{
	server->sysdeps.proc_state = _glibtop_sysdeps_proc_state |
		_glibtop_sysdeps_proc_state_uid;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_state_s (glibtop *server, glibtop_proc_state *buf, pid_t pid)
{
	char buffer [BUFSIZ], *p;
	struct stat statb;

	memset (buf, 0, sizeof (glibtop_proc_state));

	/* IMPORTANT NOTICE: For security reasons it is extremely important
	 *                   that the 'uid' and 'gid' fields have correct
	 *                   values; NEVER set their flags values if this
	 *                   is not the case !!! */

	sprintf (buffer, "/proc/%d", pid);

	if (stat (buffer, &statb))
		return;

	/* For security reasons we use stat () since it is
	 * more failsafe than parsing the file. */

	buf->uid = statb.st_uid;
	buf->gid = statb.st_gid;

	buf->flags = _glibtop_sysdeps_proc_state_uid;

	/* Now we read the remaining fields. */

	if (proc_stat_to_buffer(buffer, sizeof buffer, pid))
		return;

	p = proc_stat_after_cmd(buffer);
	p = next_token(p);

	switch(*p)
	  {
	  case 'R':
	    buf->state = GLIBTOP_PROCESS_RUNNING;
	    break;

	  case 'Z':
	    buf->state = GLIBTOP_PROCESS_ZOMBIE;
	    break;

	  case 'S':
	    buf->state = GLIBTOP_PROCESS_INTERRUPTIBLE;
	    break;

	  case 'T':
	    buf->state = GLIBTOP_PROCESS_STOPPED;
	    break;

	  case 'D':
	    buf->state = GLIBTOP_PROCESS_UNINTERRUPTIBLE;
	    break;

	  case 'W':
	    buf->state = GLIBTOP_PROCESS_SWAPPING;
	    break;

	  case 'X':
	    buf->state = GLIBTOP_PROCESS_DEAD;
	    break;
	  }

	p = skip_multiple_token (p, 36);
	buf->last_processor = atoi (p);

	p = skip_token (buffer); /* pid */
	if (G_UNLIKELY(*p++ != '('))
		glibtop_error_r (server, "Bad data in /proc/%d/stat", pid);

	g_strlcpy (buf->cmd, p, sizeof buf->cmd);

	buf->flags |= _glibtop_sysdeps_proc_state;
}
