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
#include <glibtop/error.h>
#include <glibtop/xmalloc.h>
#include <glibtop/procargs.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proc_args =
(1L << GLIBTOP_ARRAY_NUMBER) + (1L << GLIBTOP_ARRAY_SIZE);

/* Init function. */

int
glibtop_init_proc_args_s (glibtop *server)
{
    server->sysdeps.proc_args = _glibtop_sysdeps_proc_args;

    return 0;
}

/* Provides detailed information about a process. */

char **
glibtop_get_proc_args_s (glibtop *server, glibtop_array *array, pid_t pid)
{
    char *ptr = NULL, *pos, **ptrlist;
    size_t count = 0, max_len, total;
    int i, ret;

    memset (array, 0, sizeof (glibtop_array));

    max_len = PAGE_SIZE;
    ptr = glibtop_malloc_r (server, max_len + 1);

    ret = glibtop_get_proc_data_proc_args_s (server, pid, ptr, max_len);
    if (ret < 0) {
	glibtop_free_r (server, ptr);
	return NULL;
    }

    total = ret;
    ptr [total] = '\0';

    for (i = 0; i <= total; i++) {
	if (ptr [i]) continue;
	count++;
    }

    ptrlist = glibtop_calloc_r (server, count+1, sizeof (char *));

    for (i = 0, pos = ptr; i < count; i++) {
	ptrlist [i] = glibtop_strdup_r (server, pos);
	pos += strlen (pos) + 1;
    }

    ptrlist [count] = NULL;

    glibtop_free_r (server, ptr);

    array->number = count;
    array->size = sizeof (char *);
    array->flags = _glibtop_sysdeps_proc_args;

    return ptrlist;
}
