/* $Id$ */

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
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <stdlib.h>
#include <sys/systemcfg.h>
#include <sys/sysinfo.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/cpu.h>

#include <utils.h>

static const unsigned long _glibtop_sysdeps_cpu =
(1L << GLIBTOP_CPU_TOTAL) + (1L << GLIBTOP_CPU_USER) +
(1L << GLIBTOP_CPU_SYS) + (1L << GLIBTOP_CPU_IDLE) +
(1L << GLIBTOP_CPU_NICE) + (1 << GLIBTOP_CPU_FREQUENCY) +
(1L << GLIBTOP_XCPU_TOTAL) + (1L << GLIBTOP_XCPU_USER) +
(1L << GLIBTOP_XCPU_SYS) + (1L << GLIBTOP_XCPU_IDLE) +
(1L << GLIBTOP_XCPU_NICE);

/* Init function. */

void
glibtop_init_cpu_p (glibtop *server)
{
	off_t result;

	server->ncpu = _system_configuration.ncpus;
	if (server->ncpu == 1)
	{
		server->ncpu = 0; /* means single-processor, see glibtop.h */
	}

	result = _glibtop_get_kmem_offset(server, "cpuinfo");
	if (result == -1)
	{
		server->sysdeps.cpu = 0;

		return;
	}
	server->machine.cpuinfo_offset = result;

	server->machine.cpuinfo = (struct cpuinfo*)calloc(_system_configuration.ncpus, sizeof(struct cpuinfo));

	server->sysdeps.cpu = _glibtop_sysdeps_cpu;
}

/* Provides information about cpu usage. */

void
glibtop_get_cpu_p (glibtop *server, glibtop_cpu *buf)
{
	int result;
	int cpu;

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_CPU), 0);

	memset (buf, 0, sizeof (glibtop_cpu));

	result = _glibtop_get_kmem_info(server, server->machine.cpuinfo_offset,
					server->machine.cpuinfo,
					_system_configuration.ncpus
						* sizeof(struct cpuinfo));

	if (result <= 0)
	{
		glibtop_error_io_r (server, "Cannot read cpuinfo");
		return;
	}

	buf->idle = 0;
	buf->user = 0;
	buf->sys = 0;
	buf->nice = 0;
	for (cpu = 0; cpu < _system_configuration.ncpus; cpu++)
	{
		if (cpu < GLIBTOP_NCPU)
		{
			buf->xcpu_idle[cpu] =
				server->machine.cpuinfo[cpu].cpu[CPU_IDLE];
			buf->xcpu_user[cpu] =
				server->machine.cpuinfo[cpu].cpu[CPU_USER];
			buf->xcpu_sys[cpu] =
				server->machine.cpuinfo[cpu].cpu[CPU_KERNEL];
			buf->xcpu_nice[cpu] =
				server->machine.cpuinfo[cpu].cpu[CPU_WAIT];

			buf->xcpu_total[cpu] = buf->xcpu_idle[cpu] +
					       buf->xcpu_user[cpu] +
					       buf->xcpu_sys[cpu] +
					       buf->xcpu_nice[cpu];
		}

		buf->idle += server->machine.cpuinfo[cpu].cpu[CPU_IDLE];
		buf->user += server->machine.cpuinfo[cpu].cpu[CPU_USER];
		buf->sys  += server->machine.cpuinfo[cpu].cpu[CPU_KERNEL];
		buf->nice += server->machine.cpuinfo[cpu].cpu[CPU_WAIT];

		buf->total = buf->idle + buf->user + buf->sys + buf->nice ;
	}

	buf->frequency = sysconf(_SC_CLK_TCK);
	buf->flags = _glibtop_sysdeps_cpu;
}
