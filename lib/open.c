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

#include <glibtop/open.h>

/* Opens pipe to gtop server. Returns 0 on success and -1 on error. */

void
glibtop_open (glibtop *server, const char *program_name)
{
	char	version [BUFSIZ], buffer [BUFSIZ];

	memset (server, 0, sizeof (glibtop));

	server->name = program_name;

	if (pipe (server->input) || pipe (server->output))
		glibtop_error__r (server, _("cannot make a pipe: %s\n"), strerror (errno));

	server->pid  = fork ();

	if (server->pid < 0) {
		glibtop_error__r (server, _("%s: fork failed: %s\n"), strerror (errno));
	} else if (server->pid == 0) {
		close (0); close (1); /* close (2); */
		close (server->input [0]); close (server->output [1]);
		dup2 (server->input [1], 1); /* dup2 (server->input [1], 2); */
		dup2 (server->output [0], 0);
		execl (GTOP_SERVER, NULL);
		_exit (2);
	}

	close (server->input [1]);
	close (server->output [0]);

	sprintf (version, "%s server %s ready.\n", PACKAGE, VERSION);

	glibtop_read__l (server, strlen (version), buffer);

	if (memcmp (version, buffer, strlen (version)))
		glibtop_error__r (server, _("server version is not %s"), VERSION);
}
