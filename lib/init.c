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
#include <glibtop/sysdeps.h>
#include <glibtop/open.h>
#include <glibtop/parameter.h>
#include <glibtop/init_hooks.h>
#include <glibtop/machine.h>


#ifndef DEFAULT_PORT
#define DEFAULT_PORT 42800
#endif

static glibtop _glibtop_global_server;
glibtop *glibtop_global_server = &_glibtop_global_server;

static void
_init_server (glibtop *server, const unsigned features)
{
	char *command, *temp;

	/* Try to get server command, but don't override if already
	 * set via glibtop_set_parameter () */

	if (server->server_command == NULL) {
		const char *temp = getenv ("LIBGTOP_SERVER") ?
			getenv ("LIBGTOP_SERVER") : LIBGTOP_SERVER;

		server->server_command = g_strdup (temp);
	}

	if (server->server_rsh == NULL) {
		const char *temp = getenv ("LIBGTOP_RSH") ?
			getenv ("LIBGTOP_RSH") : "/usr/bin/ssh";

		server->server_rsh = g_strdup (temp);
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

	command = g_strdup (server->server_command+1);
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
				server->server_host = g_strdup ("localhost");
		} else {
			char *temp2 = strstr (temp+1, ":");
			if (temp2) *temp2 = 0;

			/* Override default. */
			if (server->server_host)
				g_free ((char *) server->server_host);

			server->server_host = g_strdup (temp+1);

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

	} else if (!strcmp (command, "unix")) {

		/* Connect to unix domain socket. */
		server->method = GLIBTOP_METHOD_UNIX;

	} else if (!strcmp (command, "pipe")) {

		/* Open pipe to server. */
		server->method = GLIBTOP_METHOD_PIPE;
		server->server_command = g_strdup(LIBGTOP_SERVER);
	} else {

		glibtop_error_r (server, "Unknown server method '%s'",
				 server->server_command+1);

	}

	g_free (command);
}

glibtop *
glibtop_init_r (glibtop **server_ptr, unsigned long features, unsigned flags)
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

		glibtop_machine_new (server);

		if (flags & GLIBTOP_FEATURES_EXCEPT)
			features = ~features & GLIBTOP_SYSDEPS_ALL;

		if (features == 0)
			features = GLIBTOP_SYSDEPS_ALL;

		if (flags & GLIBTOP_FEATURES_NO_SERVER) {
			server->method = GLIBTOP_METHOD_DIRECT;
			features = 0;
		}

		server->features = features;

		_init_server (server, features);

		server->flags |= _GLIBTOP_INIT_STATE_INIT;

		switch (server->method) {
		case GLIBTOP_METHOD_PIPE:
		case GLIBTOP_METHOD_UNIX:
			if (glibtop_server_features & features)
				break;

			server->method = GLIBTOP_METHOD_DIRECT;
			break;
		}
	}

	/* Should we open the server? */

	if (flags & GLIBTOP_INIT_NO_OPEN)
		return server;

	/* Open server, but only if not already opened. */

	if ((server->flags & _GLIBTOP_INIT_STATE_OPEN) == 0)
		glibtop_open_l (server, "glibtop",
				features, flags);

	return server;
}

glibtop *
glibtop_init_s (glibtop **server_ptr, unsigned long features, unsigned flags)
{
	glibtop *server;
	const _glibtop_init_func_t *init_fkt;

	glibtop_debug("init_s with features=%#0lx and flags=%#0x", features, flags);

	if (server_ptr == NULL)
		return NULL;

	if (*server_ptr == NULL)
		*server_ptr = glibtop_global_server;

	server = *server_ptr;

	/* Should we do the initialization? */

	if (flags & GLIBTOP_INIT_NO_INIT)
		return server;

	/* Do the initialization, but only if not already initialized. */

	if ((server->flags & _GLIBTOP_INIT_STATE_SYSDEPS) == 0) {
		glibtop_open_s (server, "glibtop", features, flags);

		for (init_fkt = _glibtop_init_hook_s; *init_fkt; init_fkt++)
			(*init_fkt) (server);

		server->flags |= _GLIBTOP_INIT_STATE_SYSDEPS;
	}

	return server;
}
