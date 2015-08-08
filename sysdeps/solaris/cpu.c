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
#include <glibtop/cpu.h>

#include <assert.h>
#include <sys/processor.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_cpu_freq =
(1L << GLIBTOP_CPU_FREQUENCY);

static const unsigned long _glibtop_sysdeps_cpu_all =
(1L << GLIBTOP_CPU_TOTAL) + (1L << GLIBTOP_CPU_USER) +
(1L << GLIBTOP_CPU_SYS) + (1L << GLIBTOP_CPU_IDLE) +
(1L << GLIBTOP_XCPU_TOTAL) + (1L << GLIBTOP_XCPU_USER) +
(1L << GLIBTOP_XCPU_SYS) + (1L << GLIBTOP_XCPU_IDLE) +
(1L << GLIBTOP_XCPU_FLAGS) +
(1L << GLIBTOP_CPU_FREQUENCY);

/* Init function. */

void
_glibtop_init_cpu_s (glibtop *server)
{
    server->sysdeps.cpu = _glibtop_sysdeps_cpu_all;
}

/* Provides information about cpu usage. */

void
glibtop_get_cpu_s (glibtop *server, glibtop_cpu *buf)
{
    kstat_ctl_t * const kc = server->machine->kc;
    cpu_stat_t cpu_stat;
    processorid_t cpu;
    int ncpu, found;

    memset (buf, 0, sizeof (glibtop_cpu));

    buf->frequency = server->machine->ticks;
    buf->flags = _glibtop_sysdeps_cpu_freq;

    if(!kc)
	return;

    switch(kstat_chain_update(kc))
    {
	case -1: assert(0); /* Debugging purposes, shouldn't happen */
	case 0:  break;
	default: glibtop_get_kstats(server);
    }

    ncpu = MIN(GLIBTOP_NCPU, server->ncpu);

    for (cpu = 0, found = 0; cpu < GLIBTOP_NCPU && found != ncpu; cpu++)
    {
	kstat_t * const ksp = server->machine->cpu_stat_kstat [cpu];
	if (!ksp) continue;;

	++found;

	if(p_online(cpu, P_STATUS) == P_ONLINE)
	    buf->xcpu_flags |= (1L << cpu);
	else
	    continue;

	if (kstat_read (kc, ksp, &cpu_stat) == -1) {
	    glibtop_warn_io_r (server, "kstat_read (cpu_stat%d)", cpu);
	    continue;
	}

	buf->xcpu_idle [cpu] = cpu_stat.cpu_sysinfo.cpu [CPU_IDLE];
	buf->xcpu_user [cpu] = cpu_stat.cpu_sysinfo.cpu [CPU_USER];
	buf->xcpu_sys [cpu] = cpu_stat.cpu_sysinfo.cpu [CPU_KERNEL];
	buf->xcpu_total [cpu] = buf->xcpu_idle [cpu] + buf->xcpu_user [cpu] +
	    buf->xcpu_sys [cpu];

	buf->idle += cpu_stat.cpu_sysinfo.cpu [CPU_IDLE];
	buf->user += cpu_stat.cpu_sysinfo.cpu [CPU_USER];
	buf->sys  += cpu_stat.cpu_sysinfo.cpu [CPU_KERNEL];
    }

    if(!found)
	return;

    buf->total = buf->idle + buf->user + buf->sys;
    buf->flags = _glibtop_sysdeps_cpu_all;
}
