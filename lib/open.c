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

#include <glibtop/gnuserv.h>

/* Opens pipe to gtop server. Returns 0 on success and -1 on error. */

void
glibtop_open_l (glibtop *server, const char *program_name,
		const unsigned long features, const unsigned flags)
{
	char version [BUFSIZ], buffer [BUFSIZ];
	int connect_type;
	unsigned nbytes;

	server->name = program_name;

	/* It is important to set _GLIBTOP_INIT_STATE_OPEN here when we
	 * do recursive calls to glibtop_init_r (). */

	server->flags |= _GLIBTOP_INIT_STATE_OPEN;

	if (flags & GLIBTOP_FEATURES_EXCEPT)
		features = ~features & GLIBTOP_SYSDEPS_ALL;

	if (features == 0)
		features = GLIBTOP_SYSDEPS_ALL;

	if (flags & GLIBTOP_FEATURES_NO_SERVER) {
		server->method = GLIBTOP_METHOD_DIRECT;
		features = 0;
	}
	
	server->features = features;

#ifdef DEBUG
	fprintf (stderr, "SIZEOF: %u - %u - %u - %u - %u - %u\n",
		 sizeof (glibtop_command), sizeof (glibtop_response),
		 sizeof (glibtop_mountentry), sizeof (glibtop_union),
		 sizeof (glibtop_sysdeps), sizeof (glibtop_response_union));
#endif

	switch (server->method) {
	case GLIBTOP_METHOD_PIPE:
	case GLIBTOP_METHOD_UNIX:
		if (glibtop_server_features & features)
			break;

		fprintf (stderr, "Using the server is not required.\n");

		server->method = GLIBTOP_METHOD_DIRECT;
		break;
	}

	switch (server->method) {
	case GLIBTOP_METHOD_DIRECT:
		server->features = 0;
		break;
	case GLIBTOP_METHOD_INET:
		fprintf (stderr, "Connecting to '%s' port %ld.\n",
			 server->server_host, server->server_port);
		
		connect_type = glibtop_make_connection
			(server->server_host, server->server_port,
			 &server->socket);
		
		fprintf (stderr, "Connect Type is %d.\n", connect_type);
		
		server->flags |= _GLIBTOP_INIT_STATE_SERVER;
		
		server->features = -1;
		break;
	case GLIBTOP_METHOD_UNIX:
		fprintf (stderr, "Connecting to Unix Domain Socket.\n");

		connect_type = glibtop_make_connection
			("unix", 0, &server->socket);
		
		fprintf (stderr, "Connect Type is %d.\n", connect_type);

		server->flags |= _GLIBTOP_INIT_STATE_SERVER;

		server->features = -1;
		break;
	case GLIBTOP_METHOD_PIPE:
		fprintf (stderr, "Opening pipe to server (%s).\n",
			 LIBGTOP_SERVER);

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
			execl (LIBGTOP_SERVER, NULL);
			glibtop_error_io_r (server, "execl (%s)",
					    LIBGTOP_SERVER);
			_exit (2);
		}

		close (server->input [1]);
		close (server->output [0]);

		sprintf (version, "Libgtop server %s ready.\n",
			 LIBGTOP_VERSION);

		glibtop_read_l (server, sizeof (nbytes), &nbytes);

		if (nbytes != strlen (version))
			glibtop_error_r (server, "Requested %u bytes but got %u",
					 strlen (version), nbytes);
		
		glibtop_read_l (server, nbytes, buffer);
		
		if (memcmp (version, buffer, strlen (version)))
			glibtop_error_r (server, "server version is not %s",
					 LIBGTOP_VERSION);
		break;
	}

	/* If the server has been started, ask it for its features. */

	if (server->flags & _GLIBTOP_INIT_STATE_SERVER) {
		glibtop_sysdeps sysdeps;
		
		glibtop_call_l (server, GLIBTOP_CMND_SYSDEPS, 0, NULL,
				sizeof (glibtop_sysdeps), &sysdeps);
		
		server->features = sysdeps.features;

		fprintf (stderr, "Server features are %lu.\n",
			 server->features);
	}

	/* In any case, we call the open functions of our own sysdeps
	 * directory. */

	fprintf (stderr, "Calling sysdeps open function.\n");
	
	glibtop_open_s (server, program_name, features, flags);
}
