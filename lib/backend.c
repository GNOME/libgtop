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

static GHashTable *_glibtop_backend_list = NULL;
static long _glibtop_backend_nr = 0;

long
glibtop_register_backend (glibtop_backend_info *info)
{
    long id;

    if (!_glibtop_backend_list)
	_glibtop_backend_list = g_hash_table_new (NULL, NULL);

    id = ++_glibtop_backend_nr;

    g_hash_table_insert (_glibtop_backend_list,
			 GINT_TO_POINTER (id),
			 info);

    return id;
}

void
glibtop_unregister_backend (long id)
{
    g_hash_table_remove (_glibtop_backend_list,
			 GINT_TO_POINTER (id));
}

glibtop_backend_info *
glibtop_backend_by_id (long id)
{
    return g_hash_table_lookup (_glibtop_backend_list,
				GINT_TO_POINTER (id));
}

typedef struct {
    const char *backend_name;
    glibtop_backend_info *info;
} _find_by_name_param_t;

static void
find_by_name (gpointer key, gpointer value, gpointer user_data)
{
    _find_by_name_param_t *param = (_find_by_name_param_t *) user_data;
    glibtop_backend_info *info = (glibtop_backend_info *) value;

    if (!info || !info->name || param->info)
	return;

    if (!strcmp (info->name, param->backend_name))
	param->info = info;
}

glibtop_backend_info *
glibtop_backend_by_name (const char *backend_name)
{
    _find_by_name_param_t param = { backend_name, NULL };

    g_hash_table_foreach (_glibtop_backend_list,
			  find_by_name, &param);

    return param.info;
}
