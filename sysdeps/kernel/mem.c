/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, March 1999.

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
#include <glibtop/mem.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_mem =
(1L << GLIBTOP_MEM_TOTAL) + (1L << GLIBTOP_MEM_USED) +
(1L << GLIBTOP_MEM_FREE) + (1L << GLIBTOP_MEM_SHARED) +
(1L << GLIBTOP_MEM_BUFFER) + (1L << GLIBTOP_MEM_CACHED);

/* Init function. */

void
glibtop_init_mem_s (glibtop *server)
{
    server->sysdeps.mem = _glibtop_sysdeps_mem;
}

/* Provides information about memory usage. */

void
glibtop_get_mem_s (glibtop *server, glibtop_mem *buf)
{
    libgtop_mem_t mem;

    memset (buf, 0, sizeof (glibtop_mem));

    if (glibtop_get_proc_data_mem_s (server, &mem))
	return;

    buf->total = mem.totalram;
    buf->used = mem.totalram - mem.freeram;
    buf->free = mem.freeram;
    buf->shared = mem.sharedram;
    buf->buffer = mem.bufferram;
    buf->cached = mem.cachedram;

    buf->flags = _glibtop_sysdeps_mem;
}
