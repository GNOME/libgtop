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
#include <glibtop/xmalloc.h>
#include <glibtop/sysdeps.h>
#include <glibtop/parameter.h>

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 42800
#endif

static glibtop _glibtop_global_server;
glibtop *glibtop_global_server = &_glibtop_global_server;

void
glibtop_server_ref (glibtop *server)
{
    if (server == NULL)
	return;

    server->refcount++;
}

void
glibtop_server_unref (glibtop *server)
{
    if (server == NULL)
	return;

    if (!server->refcount) {
	glibtop_warn_r (server, "Attempted to unref server "
			"which refcount == 0");
	return;
    }

    server->refcount--;
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
	if (flags & GLIBTOP_FEATURES_EXCEPT)
	    features = ~features & GLIBTOP_SYSDEPS_ALL;
		
	if (features == 0)
	    features = GLIBTOP_SYSDEPS_ALL;
		
	if (flags & GLIBTOP_FEATURES_NO_SERVER) {
	    features = 0;
	}
		
	server->features = features;
		
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

glibtop *
glibtop_init_s (glibtop **server_ptr, unsigned long features, unsigned flags)
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

    if ((server->flags & _GLIBTOP_INIT_STATE_SYSDEPS) == 0) {
	server->sysdeps.pointer_size = sizeof (void*)*8;
		
	server->flags |= _GLIBTOP_INIT_STATE_SYSDEPS;
    }

    return server;
}

