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

#include <glibtop.h>
#include <glibtop/global.h>
#include <glibtop/xmalloc.h>

#include <glibtop/backend.h>

int
glibtop_open_backend_l (glibtop *server, const char *backend_name,
			u_int64_t features, const char **backend_args)
{
    glibtop_backend_info *info;
    glibtop_backend *backend;

    info = glibtop_backend_by_name (backend_name);
    if (!info) return -GLIBTOP_ERROR_NO_SUCH_BACKEND;

    backend = glibtop_calloc_r (server, 1, sizeof (glibtop_backend));
    backend->info = info;

    if (info->open) {
	int retval;

	retval = info->open (server, backend, features, backend_args);
	if (retval) {
	    glibtop_free_r (server, backend->_priv);
	    glibtop_free_r (server, backend);
	    return retval;
	}
    }

    if (!server->_priv)
	server->_priv = glibtop_calloc_r
	    (server, 1, sizeof (glibtop_server_private));

    server->_priv->backend_list = g_slist_append
	(server->_priv->backend_list, backend);

    return 0;
}
