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
#include <glibtop/proclist.h>

#define GLIBTOP_PROCLIST_FLAGS	3

static const unsigned long _glibtop_sysdeps_proclist = 0;

/* Init function. */

int
glibtop_init_proclist_s (glibtop_server *server)
{
    server->info->sysdeps.proclist = _glibtop_sysdeps_proclist;

    return 0;
}

/* Fetch list of currently running processes.
 *
 * IMPORTANT NOTE:
 *   On error, this function MUST return NULL and set buf->flags to zero !
 *   On success, it returnes a pointer to a list of buf->number elements
 *   each buf->size big. The total size is stored in buf->total. */

unsigned *
glibtop_get_proclist_s (glibtop_server *server, glibtop_proclist *buf,
			int64_t which, int64_t arg)
{
    glibtop_init_s (server, GLIBTOP_SYSDEPS_PROCLIST, 0);
	
    memset (buf, 0, sizeof (glibtop_proclist));

    return NULL;
}
