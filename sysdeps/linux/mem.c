/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   LibGTop is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
   License for more details.

   You should have received a copy of the GNU Library General Public
   License along with LibGTop; see the file COPYING.LIB.  If not, write
   to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <config.h>
#include <glibtop/error.h>
#include <glibtop/mem.h>

static const unsigned long _glibtop_sysdeps_mem =
(1 << GLIBTOP_MEM_TOTAL) + (1 << GLIBTOP_MEM_USED) +
(1 << GLIBTOP_MEM_FREE) + (1 << GLIBTOP_MEM_SHARED) +
(1 << GLIBTOP_MEM_BUFFER) + (1 << GLIBTOP_MEM_CACHED) +
(1 << GLIBTOP_MEM_USER);

/* Init function. */

void
glibtop_init_mem_s (glibtop *server)
{
	server->sysdeps.mem = _glibtop_sysdeps_mem;
}

/* Provides information about memory usage. */

#define FILENAME	"/proc/meminfo"

void
glibtop_get_mem_s (glibtop *server, glibtop_mem *buf)
{
	char buffer [BUFSIZ], *p;
	int fd, len;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_MEM, 0);

	memset (buf, 0, sizeof (glibtop_mem));

	fd = open (FILENAME, O_RDONLY);
	if (fd < 0)
		glibtop_error_io_r (server, "open (%s)", FILENAME);

	len = read (fd, buffer, BUFSIZ-1);
	if (len < 0)
		glibtop_error_io_r (server, "read (%s)", FILENAME);

	close (fd);

	buffer [len] = '\0';

	p = skip_line (buffer);
	p = skip_token (p);		/* "Mem:" */

	buf->total  = strtoul (p, &p, 0);
	buf->used   = strtoul (p, &p, 0);
	buf->free   = strtoul (p, &p, 0);
	buf->shared = strtoul (p, &p, 0);
	buf->buffer = strtoul (p, &p, 0);
	buf->cached = strtoul (p, &p, 0);

	buf->user = buf->total - buf->free - buf->shared - buf->buffer;

	buf->flags = _glibtop_sysdeps_mem;
}
