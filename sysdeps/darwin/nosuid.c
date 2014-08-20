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
#include <glibtop/cpu.h>
#include <glibtop/open.h>

#include <mach/mach_host.h>
#include <mach/mach_init.h>
#include <mach/mach_interface.h>

#include <glibtop/error.h>

void
glibtop_open_s (glibtop *server, const char *program_name,
		const unsigned long features, const unsigned flags)
{
	processor_cpu_load_info_data_t *pinfo;
	mach_msg_type_number_t info_count;   
	natural_t processor_count;

	if (host_processor_info (mach_host_self (),
		PROCESSOR_CPU_LOAD_INFO,
		&processor_count,
		(processor_info_array_t*)&pinfo,
		&info_count)) {
		glibtop_error_io_r (server, "host_processor_info");
	}
	server->ncpu = (processor_count <= GLIBTOP_NCPU) ?
		processor_count : GLIBTOP_NCPU;
	vm_deallocate (mach_task_self (), (vm_address_t) pinfo, info_count);
}

void
glibtop_close_s (glibtop *server)
{ }
