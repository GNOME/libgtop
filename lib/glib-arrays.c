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
#include <glibtop/union.h>

#include <glibtop/glib-arrays.h>

GArray *
glibtop_get_proclist_as_array_l (glibtop_client *client, gint64 which,
				 gint64 arg)
{
    glibtop_array array;
    GArray *retval;
    unsigned *ptr;
    int i;

    ptr = glibtop_get_proclist_l (client, &array, which, arg);
    if (!ptr) return NULL;

    retval = g_array_new (FALSE, TRUE, sizeof (guint));
    retval = g_array_set_size (retval, array.number);

    for (i = 0; i < array.number; i++)
	g_array_index (retval, guint, i) = ptr [i];

    g_free (ptr);

    return retval;
}

GPtrArray *
glibtop_get_proc_args_as_array_l (glibtop_client *client, pid_t pid)
{
    glibtop_array array;
    GPtrArray *retval;
    char **ptr;
    int i;

    ptr = glibtop_get_proc_args_l (client, &array, pid);
    if (!ptr) return NULL;

    retval = g_ptr_array_new ();
    g_ptr_array_set_size (retval, array.number);

    for (i = 0; i < array.number; i++) {
	retval->pdata [i] = g_strdup (ptr [i]);
	g_free (ptr [i]);
    }

    g_free (ptr);

    return retval;
}

GPtrArray *
glibtop_get_proc_map_as_array_l (glibtop_client *client, pid_t pid)
{
    glibtop_array array;
    GPtrArray *retval;
    glibtop_map_entry *ptr;
    int i;

    ptr = glibtop_get_proc_map_l (client, &array, pid);
    if (!ptr) return NULL;

    retval = g_ptr_array_new ();
    g_ptr_array_set_size (retval, array.number);

    for (i = 0; i < array.number; i++)
	retval->pdata [i] = g_memdup (ptr+i, sizeof (glibtop_map_entry));

    g_free (ptr);

    return retval;
}

GPtrArray *
glibtop_get_mountlist_as_array_l (glibtop_client *client, int all_fs)
{
    glibtop_array array;
    GPtrArray *retval;
    glibtop_mountentry *ptr;
    int i;

    ptr = glibtop_get_mountlist_l (client, &array, all_fs);
    if (!ptr) return NULL;

    retval = g_ptr_array_new ();
    g_ptr_array_set_size (retval, array.number);

    for (i = 0; i < array.number; i++)
	retval->pdata [i] = g_memdup (ptr+i, sizeof (glibtop_mountentry));

    g_free (ptr);

    return retval;
}
