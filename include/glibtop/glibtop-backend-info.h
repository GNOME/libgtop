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

#ifndef __GLIBTOP_BACKEND_INFO_H__
#define __GLIBTOP_BACKEND_INFO_H__

#include <glibtop.h>
#include <glibtop/global.h>

#include <glibtop/glibtop-server.h>

G_BEGIN_DECLS

typedef struct _glibtop_backend_info	glibtop_backend_info;
typedef struct _glibtop_backend_entry	glibtop_backend_entry;
typedef struct _glibtop_backend_module	glibtop_backend_module;

typedef struct _glibtop_call_vector	glibtop_call_vector;

typedef int (*glibtop_backend_open_func_t)  (glibtop_server *, glibtop_backend *,
					     u_int64_t, const char **);
typedef int (*glibtop_backend_close_func_t) (glibtop_server *, glibtop_backend *,
					     void *);

struct _glibtop_backend_info
{
    const char *name;
    glibtop_backend_open_func_t open;
    glibtop_backend_close_func_t close;
    glibtop_call_vector *call_vector;
};

#ifdef _IN_LIBGTOP

#include <gmodule.h>

/* private structure */
struct _glibtop_backend_module
{
    int refcount;
    GModule *module;
    GSList *extra_modules;
};

struct _glibtop_backend_entry
{
    char *name;
    char *libtool_name;
    char *shlib_name;

    GSList *extra_libs;

    /* This is const since you must not attempt to
     * write into gmodule loaded memory. */
    const glibtop_backend_info *info;

    /* private pointer */
    glibtop_backend_module *_priv;
};

#endif /* _IN_LIBGTOP */

long
glibtop_register_backend (glibtop_backend_entry *entry);

void
glibtop_unregister_backend (long id);

glibtop_backend_entry *
glibtop_backend_by_id (long id);

glibtop_backend_entry *
glibtop_backend_by_name (const char *backend_name);

void
glibtop_init_backends (void);

G_END_DECLS

#endif
