/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, March 1999.

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

#include <glibtop.h>
#include <glibtop/cpu.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_cpu =
(1L << GLIBTOP_CPU_TOTAL) + (1L << GLIBTOP_CPU_USER) +
(1L << GLIBTOP_CPU_NICE) + (1L << GLIBTOP_CPU_SYS) +
(1L << GLIBTOP_CPU_IDLE) + (1L << GLIBTOP_CPU_FREQUENCY);

static const unsigned long _glibtop_sysdeps_cpu_smp =
(1L << GLIBTOP_CPU_XCPU_TOTAL) + (1L << GLIBTOP_CPU_XCPU_USER) +
(1L << GLIBTOP_CPU_XCPU_NICE) + (1L << GLIBTOP_CPU_XCPU_SYS) +
(1L << GLIBTOP_CPU_XCPU_IDLE);

/* Init function. */

int
glibtop_init_cpu_s (glibtop *server)
{
    server->sysdeps.cpu = _glibtop_sysdeps_cpu;

    if (server->ncpu)
	server->sysdeps.cpu |= _glibtop_sysdeps_cpu_smp;

    return 0;
}

/* Provides information about cpu usage. */

int
glibtop_get_cpu_s (glibtop *server, glibtop_cpu *buf)
{
    libgtop_stat_t stat;
    int retval, i;

    memset (buf, 0, sizeof (glibtop_cpu));

    retval = glibtop_get_proc_data_stat_s (server, &stat);
    if (retval)
	return retval;

    buf->user = stat.cpu.user;
    buf->nice = stat.cpu.nice;
    buf->sys = stat.cpu.sys;
    buf->idle = stat.cpu.idle;

    buf->total = buf->user + buf->nice + buf->sys + buf->idle;
    buf->frequency = stat.frequency;

    buf->flags = _glibtop_sysdeps_cpu;

    if (stat.ncpu) {
	for (i = 0; i < GLIBTOP_NCPU; i++) {
	    buf->xcpu_user [i] = stat.xcpu [i].user;
	    buf->xcpu_nice [i] = stat.xcpu [i].nice;
	    buf->xcpu_sys [i] = stat.xcpu [i].sys;
	    buf->xcpu_idle [i] = stat.xcpu [i].idle;
	    
	    buf->xcpu_total [i] = buf->xcpu_user [i] + buf->xcpu_nice [i] +
		buf->xcpu_sys [i] + buf->xcpu_idle [i];
	}

	buf->flags |= _glibtop_sysdeps_cpu_smp;
    }

    return 0;
}
