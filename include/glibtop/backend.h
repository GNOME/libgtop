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

#ifndef __GLIBTOP_BACKEND_H__
#define __GLIBTOP_BACKEND_H__

#include <glibtop.h>
#include <glibtop/global.h>

#include <glibtop/call-vector.h>

#include <glib.h>

BEGIN_LIBGTOP_DECLS

typedef struct _glibtop_backend_info	glibtop_backend_info;
typedef struct _glibtop_backend_private	glibtop_backend_private;

typedef struct _glibtop_backend		glibtop_backend;

typedef int (*glibtop_backend_open_func_t) (glibtop *, glibtop_backend *,
					    u_int64_t, const char **);
typedef int (*glibtop_backend_close_func_t) (glibtop *, glibtop_backend *);

struct _glibtop_backend_info
{
    const char *name;
    glibtop_backend_open_func_t open;
    glibtop_backend_close_func_t close;
    glibtop_call_vector *call_vector;
};

struct _glibtop_backend
{
    glibtop_backend_info *info;
    glibtop_backend_private *_priv;
};

long
glibtop_register_backend (glibtop_backend_info *info);

void
glibtop_unregister_backend (long id);

glibtop_backend_info *
glibtop_backend_by_id (long id);

glibtop_backend_info *
glibtop_backend_by_name (const char *backend_name);

void
glibtop_init_backends (void);

int
glibtop_open_backend_l (glibtop *server, const char *backend_name,
			u_int64_t features, const char **backend_args);

END_LIBGTOP_DECLS

#endif
