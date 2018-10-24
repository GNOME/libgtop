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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

static gulong mem_get_by_bytes (glibtop *server, const char *name) {
	gulong result = 0;
	size_t len = sizeof (result);

	if (sysctlbyname (name, &result, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (%s)", name);
	}
	return result;
}

static gulong try_mem_get_by_bytes (glibtop *server, const char *name) {
	gulong result = 0;
	size_t len = sizeof (result);

	if (sysctlbyname (name, &result, &len, NULL, 0)) {
		return 0;
	}
	return result;
}

static gulong mem_get_by_pages (glibtop *server, const char *name) {
	guint result = 0;
	size_t len = sizeof (result);

	if (sysctlbyname (name, &result, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (%s)", name);
	}
	return (gulong) result * pagesize;
}

void
glibtop_get_mem_s (glibtop *server, glibtop_mem *buf)
{
	gulong memtotal;
	gulong memactive;
	gulong meminactive;
	gulong memlaundry;
	gulong memwired;
	gulong memcached;
	gulong membuffer;
	gulong memfree;
	gulong zfs_arc_size;

	memset (buf, 0, sizeof *buf);

	memtotal = mem_get_by_bytes (server, "hw.physmem");
	memactive = mem_get_by_pages (server, "vm.stats.vm.v_active_count");
	meminactive = mem_get_by_pages (server, "vm.stats.vm.v_inactive_count");
	memlaundry = mem_get_by_pages (server, "vm.stats.vm.v_laundry_count");
	memwired = mem_get_by_pages (server, "vm.stats.vm.v_wire_count");
	memcached = mem_get_by_pages (server, "vm.stats.vm.v_cache_count");
	membuffer = mem_get_by_bytes (server, "vfs.bufspace");
	memfree = mem_get_by_pages (server, "vm.stats.vm.v_free_count");

	zfs_arc_size = try_mem_get_by_bytes (server, "kstat.zfs.misc.arcstats.size");

	buf->total = memtotal;
	buf->used = memtotal - memfree;
	buf->free = memfree;
	buf->shared = 0;
	buf->buffer = membuffer;
	buf->cached = memcached + zfs_arc_size;
	buf->locked = 0;

	buf->user = memactive + memlaundry + memwired - zfs_arc_size;

	buf->flags = _glibtop_sysdeps_mem;
}
