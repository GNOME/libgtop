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
#include <glibtop/procstate.h>

#include <sys/stat.h>

static const unsigned long _glibtop_sysdeps_proc_state =
(1 << GLIBTOP_PROC_STATE_CMD) + (1 << GLIBTOP_PROC_STATE_STATE);

static const unsigned long _glibtop_sysdeps_proc_state_uid =
(1 << GLIBTOP_PROC_STATE_UID) + (1 << GLIBTOP_PROC_STATE_GID);

/* Init function. */

void
glibtop_init_proc_state_s (glibtop *server)
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
	
	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_STATE, 0);

	memset (buf, 0, sizeof (glibtop_proc_state));

	/* IMPORTANT NOTICE: For security reasons it is extremely important
	 *                   that the 'uid' and 'gid' fields have correct
	 *                   values; NEVER set their flags values if this
	 *                   is not the case !!! */

	sprintf (buffer, "/proc/%d/stat", pid);

	if (stat (buffer, &statb))
		return;

	/* For security reasons we use stat () since it is
	 * more failsafe than parsing the file. */
	
	buf->uid = statb.st_uid;
	buf->gid = statb.st_gid;

	buf->flags = _glibtop_sysdeps_proc_state_uid;

	/* Now we read the remaining fields. */

	if (proc_stat_to_buffer (buffer, pid))
		return;

	p = strrchr (buffer, ')'); *p = '\0';
	buf->state = p [2];

	p = skip_token (buffer); p++;	/* pid */
	if (*p++ != '(')
		glibtop_error_r (server, "Bad data in /proc/%d/stat", pid);

	strncpy (buf->cmd, p, sizeof (buf->cmd)-1);
	buf->cmd [sizeof (buf->cmd)-1] = 0;

	buf->flags |= _glibtop_sysdeps_proc_state;
}
