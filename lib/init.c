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
#include <glibtop/xmalloc.h>
#include <glibtop/parameter.h>

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 42800
#endif

static glibtop _glibtop_global_server;
glibtop *glibtop_global_server = &_glibtop_global_server;


void
_init_server (glibtop *server, const unsigned features)
{
	char *command, *temp;

	/* Try to get server command, but don't override if already
	 * set via glibtop_set_parameter () */

	if (server->server_command == NULL) {
		const char *temp = getenv ("LIBGTOP_SERVER") ?
			getenv ("LIBGTOP_SERVER") : GTOP_SERVER;

		server->server_command = glibtop_strdup_r (server, temp);
	}

	if (server->server_rsh == NULL) {
		const char *temp = getenv ("LIBGTOP_RSH") ?
			getenv ("LIBGTOP_RSH") : "/usr/bin/ssh";
		
		server->server_rsh = glibtop_strdup_r (server, temp);
	}

	/* Try to get server method, but don't override if already
	 * set via  glibtop_set_parameter () */

	if (server->method) return;

	/* If server->command doesn't start with a colon, then it is
	 * the full pathname of the server executable. */

	if (server->server_command [0] != ':') {
		if (features & glibtop_server_features) {
			
			/* We really need the server. */
			
			server->method = GLIBTOP_METHOD_PIPE;
				
		} else {
				
			/* Fine. No server is needed, so we call the
			 * sysdeps functions directly. */
			
			server->method = GLIBTOP_METHOD_DIRECT;
		}

		return;
	}


	/* If the first character of 'server_command' is a colon,
	 * the first field is the method to connect to the server. */
	
	/* Everything up to the next colon is the method. */
	
	command = glibtop_strdup_r (server, server->server_command+1);
	temp = strstr (command, ":");
	if (temp) *temp = 0;
	
	/* Dispatch method. */
			
	if (!strcmp (command, "direct")) {

				/* Use sysdeps dir instead of connecting to server
				 * even if using the server would be required on
				 * the current system. */

		server->method = GLIBTOP_METHOD_DIRECT;
			
	} else if (!strcmp (command, "inet")) {

		server->method = GLIBTOP_METHOD_INET;

				/* Connect to internet server. */

		if (temp == NULL) {
				/* If no value was set, we use 'localhost'. */
			if (server->server_host == NULL)
				server->server_host = glibtop_strdup_r
					(server, "localhost");
		} else {
			char *temp2 = strstr (temp+1, ":");
			if (temp2) *temp2 = 0;
					
				/* Override default. */
			if (server->server_host)
				glibtop_free_r (server, (char *) server->server_host);

			server->server_host = glibtop_strdup_r
				(server, temp+1);
					
			temp = temp2;
		}
			
		if (temp == NULL) {
				/* If no value was set, we use DEFAULT_PORT. */
			if (server->server_port == 0)
				server->server_port = DEFAULT_PORT;
		} else {
			char *temp2 = strstr (temp+1, ":");
			if (temp2) *temp2 = 0;
					
			if (sscanf (temp+1, "%ld", &server->server_port) != 1)
				server->server_port = DEFAULT_PORT;
					
			temp = temp2 ? temp2 + 1 : temp2;
		}
	} else {
		glibtop_error_r (server, "Unknown server method '%s'",
				 server->server_command+1);
	}
			
	glibtop_free_r (server, command);
}

glibtop *
glibtop_init_r (glibtop **server_ptr, const unsigned long features,
		const unsigned flags)
{
	glibtop *server;

	if (server_ptr == NULL)
		return NULL;

	if (*server_ptr == NULL)
		*server_ptr = glibtop_global_server;

	server = *server_ptr;

	/* Should we do the initialization? */

	if (flags & GLIBTOP_INIT_NO_INIT)
		return server;

	/* Do the initialization, but only if not already initialized. */

	if ((server->flags & _GLIBTOP_INIT_STATE_INIT) == 0) {
		_init_server (server, features);
		
		server->flags |= _GLIBTOP_INIT_STATE_INIT;
	}

	/* Should we open the server? */

	if (flags & GLIBTOP_INIT_NO_OPEN)
		return server;

	/* Open server, but only if not already opened. */
	
	if ((server->flags & _GLIBTOP_INIT_STATE_OPEN) == 0)
		glibtop_open_l (glibtop_global_server, "glibtop",
				features, flags);
	
	return server;
}
