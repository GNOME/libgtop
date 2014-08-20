/*
   This file is part of LibGTop 2.0.

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
#include <unistd.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/mem.h>

#include <glibtop_suid.h>

#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/host_info.h>

static const unsigned long _glibtop_sysdeps_mem =
(1L << GLIBTOP_MEM_TOTAL) + (1L << GLIBTOP_MEM_USED) +
(1L << GLIBTOP_MEM_FREE);

/* Init function. */

void
_glibtop_init_mem_p (glibtop *server)
{
	server->sysdeps.mem = _glibtop_sysdeps_mem;
}

/* Provides information about memory usage. */

void
glibtop_get_mem_p (glibtop *server, glibtop_mem *buf)
{
	vm_statistics_data_t vm_info;
	mach_msg_type_number_t info_count;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_MEM), 0);

	memset (buf, 0, sizeof (glibtop_mem));

	info_count = HOST_VM_INFO_COUNT;
	if (host_statistics (mach_host_self (), HOST_VM_INFO,
			     (host_info_t)&vm_info, &info_count)) {
		glibtop_warn_io_r (server, "host_statistics (vm_statistics)");
		return;
	}

	buf->total = (vm_info.active_count + vm_info.inactive_count +
		      vm_info.free_count + vm_info.wire_count) * vm_page_size;
	buf->free = vm_info.free_count   * vm_page_size;
	buf->used = buf->total - buf->free;
	buf->flags = _glibtop_sysdeps_mem;
}

