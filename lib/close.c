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
#include <glibtop/open.h>
#include <glibtop/close.h>

#include <glibtop/backend.h>

/* Closes server. */

static void
close_backend (gpointer value, gpointer user_data)
{
    glibtop_backend *backend = (glibtop_backend *) value;
    glibtop *server = (glibtop *) user_data;

    /* should not happen ... */
    if (!backend || !backend->_priv_module)
	return;

    if (backend->info && backend->info->close)
	backend->info->close (server, backend);

    /* Note that two or more servers may open the same backend. */
    backend->_priv_module->refcount--;
#if LIBGTOP_USE_GMODULE
    if (!backend->_priv_module->refcount) {
	g_module_close (backend->_priv_module->module);
	g_free (backend->_priv_module);
    }
#endif /* LIBGTOP_USE_GMODULE */

    g_free (backend);
}

void
glibtop_close_r (glibtop *server)
{
    g_slist_foreach (server->_priv->backend_list, close_backend, server);
    g_slist_free (server->_priv->backend_list);
    server->_priv->backend_list = NULL;
}
