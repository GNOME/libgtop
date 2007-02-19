/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 2.0.

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

#include <config.h>
#include <glibtop.h>
#include <glibtop-server-private.h>

glibtop_server *
glibtop_server_new (void)
{
    glibtop_server *retval;

    retval = g_new0 (glibtop_server, 1);
    retval->info = g_new0 (glibtop_server_info, 1);
    retval->_priv = g_new0 (glibtop_server_private, 1);
    retval->refcount = 1;

    return retval;
}

void
glibtop_server_ref (glibtop_server *server)
{
    server->refcount++;
}

void
glibtop_server_unref (glibtop_server *server)
{
    server->refcount--;

    if (server->refcount <= 0) {
	g_free (server->_priv);
	g_free (server->info);
	g_free (server);
    }
}
