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
#include <glibtop/cpu.h>

#include <glibtop_suid.h>

#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/vm_map.h>

static const unsigned long _glibtop_sysdeps_cpu =
(1L << GLIBTOP_CPU_TOTAL)  + (1L << GLIBTOP_CPU_USER) +
(1L << GLIBTOP_CPU_NICE)   + (1L << GLIBTOP_CPU_SYS) +
(1L << GLIBTOP_CPU_IDLE)   + (1L << GLIBTOP_CPU_FREQUENCY) +
(1L << GLIBTOP_XCPU_TOTAL) + (1L << GLIBTOP_XCPU_USER) +
(1L << GLIBTOP_XCPU_NICE)  + (1L << GLIBTOP_XCPU_SYS) +
(1L << GLIBTOP_XCPU_IDLE);

/* Init function. */

void
_glibtop_init_cpu_p (glibtop *server)
{
	server->sysdeps.cpu = _glibtop_sysdeps_cpu;
}

/* Provides information about cpu usage. */

void
glibtop_get_cpu_p (glibtop *server, glibtop_cpu *buf)
{
	processor_cpu_load_info_data_t *pinfo;
	mach_msg_type_number_t info_count;
	natural_t processor_count;
	int i;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_CPU), 0);

	memset (buf, 0, sizeof (glibtop_cpu));

	if (host_processor_info (mach_host_self (),
				 PROCESSOR_CPU_LOAD_INFO,
				 &processor_count,
				 (processor_info_array_t*)&pinfo,
				 &info_count)) {
		glibtop_warn_io_r (server, "host_processor_info (cpu)");
		return;
	}

	for (i = 0; i < server->ncpu; i++) {
		buf->xcpu_user [i] = pinfo[i].cpu_ticks [CPU_STATE_USER];
		buf->xcpu_sys  [i] = pinfo[i].cpu_ticks [CPU_STATE_SYSTEM];
		buf->xcpu_idle [i] = pinfo[i].cpu_ticks [CPU_STATE_IDLE];
		buf->xcpu_nice [i] = pinfo[i].cpu_ticks [CPU_STATE_NICE];
		buf->xcpu_total[i] = buf->xcpu_user [i] + buf->xcpu_sys  [i] +
				     buf->xcpu_idle [i] + buf->xcpu_nice [i];

		buf->user  += buf->xcpu_user [i];
		buf->sys   += buf->xcpu_sys  [i];
		buf->idle  += buf->xcpu_idle [i];
		buf->nice  += buf->xcpu_nice [i];
		buf->total += buf->xcpu_total[i];
	}
	vm_deallocate (mach_task_self (), (vm_address_t) pinfo, info_count);

	buf->frequency = 100;
	buf->flags = _glibtop_sysdeps_cpu;
}

