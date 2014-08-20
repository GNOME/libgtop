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
#include <glibtop/error.h>
#include <glibtop/mem.h>

#include <sys/vm.h>

#include <mach.h>
#include <mach/mach_types.h>
#include <mach/vm_statistics.h>

static const unsigned long _glibtop_sysdeps_mem =
(1L << GLIBTOP_MEM_TOTAL) + (1L << GLIBTOP_MEM_USED) +
(1L << GLIBTOP_MEM_FREE);

/* Init function. */

void
_glibtop_init_mem_s (glibtop *server)
{
	server->sysdeps.mem = _glibtop_sysdeps_mem;
}

/* Provides information about memory usage. */

void
glibtop_get_mem_s (glibtop *server, glibtop_mem *buf)
{
	vm_statistics_data_t vmstats;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_MEM, 0);

	memset (buf, 0, sizeof (glibtop_mem));

	buf->flags = _glibtop_sysdeps_mem;

	(void) vm_statistics(task_self(), &vmstats);

	buf->free = vmstats.free_count   * vmstats.pagesize;
	buf->used = vmstats.active_count * vmstats.pagesize;

	/* [FIXME]: Is this correct? */

	buf->total = (vmstats.active_count + vmstats.inactive_count +
		      vmstats.free_count + vmstats.wire_count) *
		vmstats.pagesize;
}
