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
(1 << GLIBTOP_PROC_STATE_CMD) + (1 << GLIBTOP_PROC_STATE_STATE) +
(1 << GLIBTOP_PROC_STATE_UID) + (1 << GLIBTOP_PROC_STATE_GID);

/* Provides detailed information about a process. */

void
glibtop_get_proc_state_s (glibtop *server, glibtop_proc_state *buf, pid_t pid)
{
	char buffer [BUFSIZ], input [BUFSIZ], *tmp;
	struct stat statb;
	int fd, nread;
	
	glibtop_init_r (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_proc_state));

	if (pid == 0) {
		/* Client is only interested in the flags. */
		buf->flags = _glibtop_sysdeps_proc_state;
		return;
	}

	server->machine.last_pid = pid;
	server->machine.no_update = 0;
	server->machine.proc_status [0] = 0;
	server->machine.proc_statm [0] = 0;
	server->machine.proc_stat [0] = 0;

	sprintf (input, "/proc/%d/stat", pid);

	/* IMPORTANT NOTICE: For security reasons it is extremely important
	 *                   that the 'uid' and 'gid' fields have correct
	 *                   values; NEVER set their flags values if this
	 *                   is not the case !!! */

	if (stat (input, &statb)) return;

	/* For security reasons we use stat () that is more failsafe than sscanf (). */
	
	buf->uid = statb.st_uid;
	buf->gid = statb.st_gid;

	fd = open (input, O_RDONLY);
	if (fd == -1)
		glibtop_error_r (server, "open (%s): %s",
				 input, strerror (errno));

	nread = read (fd, buffer, BUFSIZ);
	if (nread == -1)
		glibtop_error_r (server, "read (%s): %s",
				 input, strerror (errno));
	
	buffer [nread] = 0;

	server->machine.no_update = 1;
	server->machine.last_pid = pid;
	strcpy (server->machine.proc_stat, buffer);
	
	/* This is from guile-utils/gtop/proc/readproc.c */
	
	/* split into "PID (cmd" and "<rest>" */
	tmp = strrchr (buffer, ')');
	*tmp = '\0';		/* replace trailing ')' with NUL */
	/* parse these two strings separately, skipping the leading "(". */
	memset (buf->cmd, 0, sizeof (buf->cmd));
	sscanf (buffer, "%d (%39c", &pid, buf->cmd);
	sscanf(tmp + 2, "%c", &buf->state); /* skip space after ')' too */

	close (fd);

	buf->flags = _glibtop_sysdeps_proc_state;
}
