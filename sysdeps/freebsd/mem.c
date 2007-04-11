/* Copyright (C) 1998 Joshua Sled
   This file is part of LibGTop 1.0.

   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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

#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/mem.h>

#include <sys/types.h>
#include <sys/sysctl.h>

static const unsigned long _glibtop_sysdeps_mem =
(1L << GLIBTOP_MEM_TOTAL) + (1L << GLIBTOP_MEM_USED) +
(1L << GLIBTOP_MEM_FREE) + (1L << GLIBTOP_MEM_SHARED) +
(1L << GLIBTOP_MEM_BUFFER) + (1L << GLIBTOP_MEM_CACHED) +
(1L << GLIBTOP_MEM_USER);

/* these are for getting the memory statistics */
static int pagesize;

/* Init function. */
void
_glibtop_init_mem_s (glibtop *server)
{
	pagesize = getpagesize ();

	server->sysdeps.mem = _glibtop_sysdeps_mem;
}

void
glibtop_get_mem_s (glibtop *server, glibtop_mem *buf)
{
	gulong memtotal;
	guint memused;
	gulong buffers;
	guint cached;
	size_t len;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_MEM, 0);

	memset (buf, 0, sizeof *buf);

	len = sizeof (memtotal);
	if (sysctlbyname ("hw.physmem", &memtotal, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (hw.physmem)");
		return;
	}

	len = sizeof (memused);
	if (sysctlbyname ("vm.stats.vm.v_wire_count", &memused, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (vm.stats.vm.v_wire_count)");
		return;
	}

	len = sizeof (cached);
	if (sysctlbyname ("vm.stats.vm.v_cache_count", &cached, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (vm.stats.vm.v_cache_count)");
		return;
	}

	buffers = 0;

	buf->total = memtotal;
	buf->used = (memused * pagesize);
	buf->free = (buf->total - buf->used);
	buf->shared = 0;
	buf->cached = (cached * pagesize);
	buf->buffer = buffers;

	buf->user = buf->total - buf->free - buf->cached - buf->buffer;
	buf->flags = _glibtop_sysdeps_mem;
}
