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

#ifndef __GLIBTOP_BACKEND_H__
#define __GLIBTOP_BACKEND_H__

#include <glibtop.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define	GLIBTOP_TYPE_BACKEND		(glibtop_backend_get_type ())
#define GLIBTOP_BACKEND(backend)	(G_TYPE_CHECK_INSTANCE_CAST ((backend), GLIBTOP_TYPE_BACKEND, glibtop_backend))
#define GLIBTOP_BACKEND_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GLIBTOP_TYPE_BACKEND, glibtop_backend_class))
#define GLIBTOP_IS_BACKEND(backend)	(G_TYPE_CHECK_INSTANCE_TYPE ((backend), GLIBTOP_TYPE_BACKEND))
#define GLIBTOP_IS_BACKEND_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GLIBTOP_TYPE_BACKEND))
#define	GLIBTOP_BACKEND_GET_CLASS(backend)(G_TYPE_INSTANCE_GET_CLASS ((backend), GLIBTOP_TYPE_BACKEND, glibtop_backend_class))

#define GLIBTOP_BACKEND_TYPE(backend)	(G_TYPE_FROM_INSTANCE (backend))
#define GLIBTOP_BACKEND_TYPE_NAME(backend)(g_type_name (GLIBTOP_BACKEND_TYPE (backend)))

/* --- typedefs & structures --- */
typedef struct _glibtop_backend		glibtop_backend;
typedef struct _glibtop_backend_class	glibtop_backend_class;
typedef struct _glibtop_backend_private	glibtop_backend_private;

#include <glibtop/glibtop-backend-info.h>

struct	_glibtop_backend
{
    GObject object;

    /* <private> */
    glibtop_backend_private *_priv;
};

struct	_glibtop_backend_class
{
    GObjectClass parent_class;;
};

GType
glibtop_backend_get_type (void);

glibtop_backend *
glibtop_backend_get (const char *backend_name, guint64 features,
		     const char **backend_args, GError **error);

void
glibtop_backend_set_closure_data (glibtop_backend *backend, glibtop_closure *closure);

glibtop_closure *
glibtop_backend_get_closure_data (glibtop_backend *backend);

glibtop_call_vector *
glibtop_backend_get_call_vector (glibtop_backend *backend);

glibtop_server *
glibtop_backend_get_server (glibtop_backend *backend);

G_END_DECLS

#endif
