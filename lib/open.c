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
#include <glibtop/open.h>
#include <glibtop/read.h>
#include <glibtop/error.h>
#include <glibtop/version.h>
#include <glibtop/sysdeps.h>
#include <glibtop/command.h>

#include <glibtop/gnuserv.h>

/* Opens pipe to gtop server. Returns 0 on success and -1 on error. */

void
glibtop_open_l (glibtop *server, const char *program_name,
		const unsigned long features, const unsigned flags)
{
	int connect_type;

	server->name = program_name;

	/* It is important to set _GLIBTOP_INIT_STATE_OPEN here when we
	 * do recursive calls to glibtop_init_r (). */

	server->flags |= _GLIBTOP_INIT_STATE_OPEN;

	server->error_method = GLIBTOP_ERROR_METHOD_DEFAULT;

	glibtop_debug ("SIZEOF: %zu - %zu - %zu - %zu - %zu - %zu",
		 sizeof (glibtop_command), sizeof (glibtop_response),
		 sizeof (glibtop_mountentry), sizeof (glibtop_union),
		 sizeof (glibtop_sysdeps), sizeof (glibtop_response_union));

	switch (server->method) {
	case GLIBTOP_METHOD_DIRECT:
		server->features = 0;
		break;
	case GLIBTOP_METHOD_INET:
		glibtop_debug ("Connecting to '%s' port %ld.",
			 server->server_host, server->server_port);

		connect_type = glibtop_make_connection
			(server->server_host, server->server_port,
			 &server->socket);

		glibtop_debug ("Connect Type is %d.", connect_type);

		server->flags |= _GLIBTOP_INIT_STATE_SERVER;

		server->features = -1;
		break;
	case GLIBTOP_METHOD_UNIX:
		glibtop_debug ("Connecting to Unix Domain Socket.");

		connect_type = glibtop_make_connection
			("unix", 0, &server->socket);

		glibtop_debug ("Connect Type is %d.", connect_type);

		server->flags |= _GLIBTOP_INIT_STATE_SERVER;

		server->features = -1;
		break;
	case GLIBTOP_METHOD_PIPE:
		glibtop_debug ("Opening pipe to server (%s).",
			 server->server_command);

		if (pipe (server->input) || pipe (server->output))
			glibtop_error_io_r (server, "cannot make a pipe");

		server->pid  = fork ();

		if (server->pid < 0) {
			glibtop_error_io_r (server, "fork failed");
		} else if (server->pid == 0) {
			close (0); close (1);
			close (server->input [0]); close (server->output [1]);
			dup2 (server->input [1], 1);
			dup2 (server->output [0], 0);
			execl (server->server_command, "libgtop-server", NULL);
			glibtop_error_io_r (server, "execl (%s)",
					    server->server_command);
			_exit (2);
		}

		close (server->input [1]);
		close (server->output [0]);

		server->flags |= _GLIBTOP_INIT_STATE_SERVER;

		server->features = -1;
		break;
	}

	/* If the server has been started, ask it for its features. */

	if (server->flags & _GLIBTOP_INIT_STATE_SERVER) {
		char version [BUFSIZ], buffer [BUFSIZ];
		glibtop_sysdeps sysdeps;
		size_t size, nbytes;

		/* First check whether the server version is correct. */

		sprintf (version, LIBGTOP_VERSION_STRING,
			 LIBGTOP_VERSION, LIBGTOP_SERVER_VERSION,
			 (guint) sizeof (glibtop_command),
			 (guint) sizeof (glibtop_response),
			 (guint) sizeof (glibtop_union),
			 (guint) sizeof (glibtop_sysdeps));

		size = strlen (version) + 1;

		glibtop_read_l (server, sizeof (nbytes), &nbytes);

		if (nbytes != size)
			glibtop_error_r (server,
					 "Requested %u bytes but got %u.",
					 (unsigned)size, (unsigned)nbytes);

		glibtop_read_l (server, nbytes, buffer);

		if (memcmp (version, buffer, size))
			glibtop_error_r (server, "server version is not %s",
					 LIBGTOP_VERSION);

		/* Now ask it for its features. */

		glibtop_call_l (server, GLIBTOP_CMND_SYSDEPS, 0, NULL,
				sizeof (glibtop_sysdeps), &sysdeps);

		server->features = sysdeps.features;

		memcpy (&server->sysdeps, &sysdeps, sizeof (glibtop_sysdeps));

		glibtop_debug ("Server features are %#0lx.",
			 server->features);
	}

	/* In any case, we call the open functions of our own sysdeps
	 * directory. */

	glibtop_debug ("Calling sysdeps open function.");

	glibtop_init_s (&server, features, flags);
}
