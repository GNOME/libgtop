/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/* $Id$ */

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
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <backend-server.h>

static int
_open_server (glibtop_server *, glibtop_backend *, u_int64_t, const char **);

static int
_close_server (glibtop_server *, glibtop_backend *, void *);

extern glibtop_call_vector glibtop_backend_server_call_vector;

#ifdef LIBGTOP_USE_GMODULE
glibtop_backend_info LibGTopBackendInfo = {
    "glibtop-backend-server", _open_server, _close_server,
    &glibtop_backend_server_call_vector
};
#else
glibtop_backend_info LibGTopBackendInfo_Server = {
    "glibtop-backend-server", _open_server, _close_server,
    &glibtop_backend_server_call_vector
};
#endif

static int
_open_server (glibtop_server *server, glibtop_backend *backend,
	      u_int64_t features, const char **backend_args)
{
    backend_server_private *priv;

    priv = g_malloc (sizeof (backend_server_private));

    glibtop_backend_set_closure_data (backend, priv);

#ifdef DEBUG
    fprintf (stderr, "open_server - %p, %p, %p\n", server, backend, priv);

    fprintf (stderr, "Opening pipe to server (%s).\n", LIBGTOP_SERVER);
#endif

    if (pipe (priv->input) || pipe (priv->output)) {
	glibtop_warn_io_r (server, "cannot make a pipe");
	return -1;
    }

    priv->pid  = fork ();

    if (priv->pid < 0) {
	glibtop_warn_io_r (server, "fork failed");
	return -1;
    } else if (priv->pid == 0) {
	close (0); close (1);
	close (priv->input [0]);
	close (priv->output [1]);
	dup2 (priv->input [1], 1);
	dup2 (priv->output [0], 0);
	execl (LIBGTOP_SERVER, "libgtop-server", NULL);
	glibtop_error_io_r (server, "execl (%s)",
			    LIBGTOP_SERVER);
	_exit (2);
    }

    close (priv->input [1]);
    close (priv->output [0]);

    glibtop_server_ref (server);
    priv->server = server;

    return 0;
}

static int
_close_server (glibtop_server *server, glibtop_backend *backend,
	       void *closure)
{
    backend_server_private *priv = closure;

    kill (priv->pid, SIGKILL);
    close (priv->input [0]);
    close (priv->output [1]);

    glibtop_server_unref (priv->server);

    priv->pid = 0;

    g_free (priv);

    return 0;
}

