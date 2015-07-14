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
#include <glibtop/error.h>
#include <glibtop/mem.h>

#include "glibtop_private.h"

static const unsigned long _glibtop_sysdeps_mem =
(1L << GLIBTOP_MEM_TOTAL) + (1L << GLIBTOP_MEM_USED) +
(1L << GLIBTOP_MEM_FREE) + (1L << GLIBTOP_MEM_SHARED) +
(1L << GLIBTOP_MEM_BUFFER) + (1L << GLIBTOP_MEM_CACHED) +
(1L << GLIBTOP_MEM_USER);

/* Init function. */

void
_glibtop_init_mem_s (glibtop *server)
{
	server->sysdeps.mem = _glibtop_sysdeps_mem;
}

/* Provides information about memory usage. */

#define FILENAME	"/proc/meminfo"

void
glibtop_get_mem_s (glibtop *server, glibtop_mem *buf)
{
	char buffer [BUFSIZ];

	memset(buf, 0, sizeof *buf);

	file_to_buffer(server, buffer, sizeof buffer, FILENAME);

        /* try to match procps's `free` */
	buf->total  = get_scaled(buffer, "MemTotal:");
	buf->free   = get_scaled(buffer, "MemFree:");
	buf->used   = buf->total - buf->free;
	buf->shared = get_scaled(buffer, "Shmem:");
	buf->buffer = get_scaled(buffer, "Buffers:");
	buf->cached = get_scaled(buffer, "Cached:") + get_scaled(buffer, "Slab:");

	if (server->os_version_code >= LINUX_VERSION_CODE(3, 14, 0)) {
		buf->user = buf->total - get_scaled(buffer, "MemAvailable:");
	}
	else {
		buf->user = buf->total - buf->free - buf->cached - buf->buffer;
	}

	buf->flags = _glibtop_sysdeps_mem;
}
