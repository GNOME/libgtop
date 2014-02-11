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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <config.h>
#include <glibtop.h>
#include <glibtop/proclist.h>

#define GLIBTOP_PROCLIST_FLAGS	3

static const unsigned long _glibtop_sysdeps_proclist = 0;

/* Init function. */

void
_glibtop_init_proclist_s (glibtop *server)
{
	server->sysdeps.proclist = _glibtop_sysdeps_proclist;
}

/* Fetch list of currently running processes.
 *
 * IMPORTANT NOTE:
 *   On error, this function MUST return NULL and set buf->flags to zero !
 *   On success, it returnes a pointer to a list of buf->number elements
 *   each buf->size big. The total size is stored in buf->total. */

pid_t *
glibtop_get_proclist_s (glibtop *server, glibtop_proclist *buf,
			gint64 which, gint64 arg)
{
	memset (buf, 0, sizeof (glibtop_proclist));
	return NULL;
}
