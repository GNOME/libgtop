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
	char	version [BUFSIZ], buffer [BUFSIZ], *temp, *temp2;
	glibtop_sysdeps sysdeps;
	int	connect_type, ret;

	memset (server, 0, sizeof (glibtop));

	server->name = program_name;

	/* Is the user allowed to override the server ? */

	if (flags & GLIBTOP_OPEN_NO_OVERRIDE)
		return;
	
	/* Try to get data from environment. */
	
	temp = getenv ("LIBGTOP_SERVER") ?
		getenv ("LIBGTOP_SERVER") : GTOP_SERVER;

	server->server_command = glibtop_strdup_r (server, temp);
	
	temp = getenv ("LIBGTOP_RSH") ?
		getenv ("LIBGTOP_RSH") : "rsh";

	server->server_rsh = glibtop_strdup_r (server, temp);
	
	/* If the first character of 'server_command' is a colon,
	 * the first field is the method to connect to the server. */
	
	if (server->server_command [0] == ':') {
		
		/* Everything up to the next colon is the method. */
		
		temp = strstr (server->server_command+1, ":");
		if (temp) *temp = 0;

		/* Dispatch method. */
			
		if (!strcmp (server->server_command+1, "direct")) {

			/* Use sysdeps dir instead of connecting to server
			 * even if using the server would be required on
			 * the current system. */

			return;
			
		} else if (!strcmp (server->server_command+1, "inet")) {

			/* Connect to internet server. */

			if (temp == NULL) {
				server->server_host = glibtop_strdup_r
					(server, "localhost");
			} else {
				temp2 = strstr (temp+1, ":");
				if (temp2) *temp2 = 0;

				server->server_host = glibtop_strdup_r
					(server, temp+1);

				temp = temp2;
			}
			
			if (temp == NULL) {
				server->server_port = DEFAULT_PORT;
			} else {
				temp2 = strstr (temp+1, ":");
				if (temp2) *temp2 = 0;

				ret = sscanf (temp+1, "%d", &server->server_port);

				if (ret != 1)
					server->server_port = DEFAULT_PORT;

				temp = temp2 ? temp2 + 1 : temp2;
			}

			fprintf (stderr, "Connecting to '%s' port %d.\n",
				 server->server_host, server->server_port);

			connect_type = glibtop_make_connection
				(server->server_host, server->server_port,
				 &server->socket);

			server->features = -1;

			return;
		}
	}

	glibtop_call_l (server, GLIBTOP_CMND_SYSDEPS, 0, NULL,
			sizeof (glibtop_sysdeps), &sysdeps);
	
	server->features = sysdeps.features;
}
