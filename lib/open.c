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
#include <glibtop/open.h>
#include <glibtop/sysdeps.h>
#include <glibtop/command.h>
#include <glibtop/xmalloc.h>

/* Opens pipe to gtop server. Returns 0 on success and -1 on error. */

void
glibtop_open_l (glibtop *server, const char *program_name,
		 const unsigned long features, const unsigned flags)
{
	char	version [BUFSIZ], buffer [BUFSIZ];
	char	*server_command, *server_rsh, *temp;
	char	*server_host, *server_user;
	glibtop_sysdeps sysdeps;

	memset (server, 0, sizeof (glibtop));

	server->name = program_name;

	/* Is the user allowed to override the server ? */

	if ((flags & GLIBTOP_OPEN_NO_OVERRIDE) == 0) {

		/* Try to get data from environment. */
		
		temp = getenv ("LIBGTOP_SERVER") ?
			getenv ("LIBGTOP_SERVER") : GTOP_SERVER;
		
		server_command = glibtop_malloc_r (server, strlen (temp) + 1);
		
		strcpy (server_command, temp);
		
		temp = getenv ("LIBGTOP_RSH") ?
			getenv ("LIBGTOP_RSH") : "rsh";
		
		server_rsh = glibtop_malloc_r (server, strlen (temp) + 1);
		
		strcpy (server_rsh, temp);
		
		/* Extract host and user information. */
		
		temp = strstr (server_command, ":");
		
		if (temp) {
			*temp = 0;
			server_host = server_command;
			server_command = temp+1;
			
			temp = strstr (server_host, "@");
			
			if (temp) {
				*temp = 0;
				server_user = server_host;
				server_host = temp+1;
			} else {
				server_user = NULL;
			}
		} else {
			server_host = NULL;
			server_user = NULL;
		}
		
		/* Store everything in `server'. */
		
		server->server_command = server_command;
		server->server_host = server_host;
		server->server_user = server_user;
		server->server_rsh = server_rsh;
	}
	
	/* Get server features. */
	
	if (server->server_host == NULL) {
		server->features = glibtop_server_features;
		
		if (server->features == 0)
			return;
	}

	/* Fork and exec server. */

	if (pipe (server->input) || pipe (server->output))
		glibtop_error_r (server, _("cannot make a pipe: %s\n"),
				  strerror (errno));

	server->pid  = fork ();

	if (server->pid < 0) {
		glibtop_error_r (server, _("%s: fork failed: %s\n"),
				  strerror (errno));
	} else if (server->pid == 0) {
		close (0); close (1); /* close (2); */
		close (server->input [0]); close (server->output [1]);
		dup2 (server->input [1], 1); /* dup2 (server->input [1], 2); */
		dup2 (server->output [0], 0);

		if (server_host) {
			if (server_user) {
				execl (server->server_rsh, "-l",
				       server->server_user, server->server_host,
				       server->server_command, NULL);
			} else {
				execl (server->server_rsh,
				       server->server_host, server_command, NULL);
			}
		} else {
			execl (server->server_command, NULL);
		}

		_exit (2);
	}

	fprintf (stderr, "PID: %d\n", server->pid);

	close (server->input [1]);
	close (server->output [0]);

	sprintf (version, "%s server %s ready.\n", PACKAGE, VERSION);

	glibtop_read_l (server, strlen (version), buffer);

	if (memcmp (version, buffer, strlen (version)))
		glibtop_error_r (server, _("server version is not %s"), VERSION);

	fprintf (stderr, "Calling GLITOP_CMND_SYSDEPS ...\n");

	glibtop_call_l (server, GLIBTOP_CMND_SYSDEPS, 0, NULL,
			 sizeof (glibtop_sysdeps), &sysdeps);

	server->features = sysdeps.features;

	fprintf (stderr, "Features: %lu\n", server->features);
}
