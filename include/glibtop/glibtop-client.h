/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/* $Id$ */

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

#ifndef __GLIBTOP_CLIENT_H__
#define __GLIBTOP_CLIENT_H__

#include <glibtop.h>
#include <gobject/gobject.h>

#include <glibtop/backend.h>

BEGIN_LIBGTOP_DECLS

#define	GLIBTOP_TYPE_CLIENT		(glibtop_client_get_type ())
#define GLIBTOP_CLIENT(client)		(G_TYPE_CHECK_INSTANCE_CAST ((client), GLIBTOP_TYPE_CLIENT, glibtop_client))
#define GLIBTOP_CLIENT_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GLIBTOP_TYPE_CLIENT, glibtop_client_class))
#define GLIBTOP_IS_CLIENT(client)	(G_TYPE_CHECK_INSTANCE_TYPE ((client), GLIBTOP_TYPE_CLIENT))
#define GLIBTOP_IS_CLIENT_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GLIBTOP_TYPE_CLIENT))
#define	GLIBTOP_CLIENT_GET_CLASS(client)(G_TYPE_INSTANCE_GET_CLASS ((client), GLIBTOP_TYPE_CLIENT, glibtop_client_class))

#define GLIBTOP_CLIENT_TYPE(client)	(G_TYPE_FROM_INSTANCE (client))
#define GLIBTOP_CLIENT_TYPE_NAME(client)(g_type_name (GLIBTOP_CLIENT_TYPE (client)))

/* --- typedefs & structures --- */
typedef struct _glibtop_client		glibtop_client;
typedef struct _glibtop_client_class	glibtop_client_class;
typedef struct _glibtop_client_private	glibtop_client_private;

struct	_glibtop_client
{
    GObject object;

    /* <public> */
    GError *error;

    /* <private> */
    glibtop_client_private *_priv;
};

struct	_glibtop_client_class
{
    GObjectClass parent_class;;

    void (*error) (glibtop_client *client, GError *error);
    void (*warning) (glibtop_client *client, GError *error);
};

GType
glibtop_client_get_type (void);

glibtop_client *
glibtop_client_new (void);

void
glibtop_client_propagate_error (glibtop_client *client, GError *error);

void
glibtop_client_propagate_warning (glibtop_client *client, GError *error);

glibtop_backend *
glibtop_client_open_backend (glibtop_client *client, const char *backend_name,
			     u_int64_t features, const char **backend_args);

void
glibtop_client_add_backend (glibtop_client *client,
			    glibtop_backend *backend);

void
glibtop_client_remove_backend (glibtop_client *client,
			       glibtop_backend *backend);

END_LIBGTOP_DECLS

#endif
