/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <config.h>
#include <glibtop/error.h>
#include <glibtop/mem.h>

#include "kernel.h"

static const unsigned long _glibtop_sysdeps_mem =
(1 << GLIBTOP_MEM_TOTAL) + (1 << GLIBTOP_MEM_USED) +
(1 << GLIBTOP_MEM_FREE) + (1 << GLIBTOP_MEM_SHARED) +
(1 << GLIBTOP_MEM_BUFFER) + (1 << GLIBTOP_MEM_CACHED) +
(1 << GLIBTOP_MEM_USER);

/* Provides information about memory usage. */

void
glibtop_get_mem_s (glibtop *server, glibtop_mem *buf)
{
	union table tbl;
	
	glibtop_init_s (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_mem));

	if (table (TABLE_MEM, &tbl, NULL))
		glibtop_error_io_r (server, "table(TABLE_MEM)");

	buf->flags = _glibtop_sysdeps_mem;

	buf->total  = tbl.mem.total;
	buf->used   = tbl.mem.used;
	buf->free   = tbl.mem.free;
	buf->shared = tbl.mem.shared;
	buf->buffer = tbl.mem.buffer;
	buf->cached = tbl.mem.cached;

	buf->user = buf->total - buf->free - buf->shared - buf->buffer;
}
