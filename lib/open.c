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
	server->name = program_name;

	/* It is important to set _GLIBTOP_INIT_STATE_OPEN here when we
	 * do recursive calls to glibtop_init_r (). */

	server->flags |= _GLIBTOP_INIT_STATE_OPEN;

	if (server->method == GLIBTOP_METHOD_INET) {
		int connect_type;

		fprintf (stderr, "Connecting to '%s' port %d.\n",
			 server->server_host, server->server_port);
		
		connect_type = glibtop_make_connection
			(server->server_host, server->server_port,
			 &server->socket);
		
		server->features = -1;

		return;
	}

	/* If the server has been started, ask it for its features. */

	if (server->flags & _GLIBTOP_INIT_STATE_SERVER) {
		glibtop_sysdeps sysdeps;
		
		glibtop_call_l (server, GLIBTOP_CMND_SYSDEPS, 0, NULL,
				sizeof (glibtop_sysdeps), &sysdeps);
		
		server->features = sysdeps.features;
	}
}
