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
#include <glibtop/error.h>
#include <glibtop/cpu.h>

#include "glibtop_private.h"

static const unsigned long _glibtop_sysdeps_cpu =
(1L << GLIBTOP_CPU_TOTAL) + (1L << GLIBTOP_CPU_USER) +
(1L << GLIBTOP_CPU_NICE) + (1L << GLIBTOP_CPU_SYS) +
(1L << GLIBTOP_CPU_IDLE) + (1L << GLIBTOP_CPU_FREQUENCY);

static const unsigned long _glibtop_sysdeps_cpu_smp =
(1L << GLIBTOP_XCPU_TOTAL) + (1L << GLIBTOP_XCPU_USER) +
(1L << GLIBTOP_XCPU_NICE) + (1L << GLIBTOP_XCPU_SYS) +
(1L << GLIBTOP_XCPU_IDLE);

/* Init function. */

void
_glibtop_init_cpu_s (glibtop *server)
{
	server->sysdeps.cpu = _glibtop_sysdeps_cpu;

	if (server->ncpu)
		server->sysdeps.cpu |= _glibtop_sysdeps_cpu_smp;
}

/* Provides information about cpu usage. */

#define FILENAME	"/proc/stat"

void
glibtop_get_cpu_s (glibtop *server, glibtop_cpu *buf)
{
	char buffer [BUFSIZ], *p;
	int i;

	memset (buf, 0, sizeof (glibtop_cpu));

	file_to_buffer(server, buffer, sizeof buffer, FILENAME);

	/*
	 * GLOBAL
	 */

	p = skip_token (buffer);	/* "cpu" */

	buf->user = strtoull (p, &p, 0);
	buf->nice = strtoull (p, &p, 0);
	buf->sys  = strtoull (p, &p, 0);
	buf->idle = strtoull (p, &p, 0);
	buf->total = buf->user + buf->nice + buf->sys + buf->idle;

	buf->frequency = 100;
	buf->flags = _glibtop_sysdeps_cpu;

	/*
	 * PER CPU
	 */

	for (i = 0; i <= server->ncpu; i++) {

		p = skip_line(p); /* move to ^ */

		if (!check_cpu_line_warn(server, p, i))
			break;

		p = skip_token(p); /* "cpuN" */

		buf->xcpu_user [i] = strtoull (p, &p, 0);
		buf->xcpu_nice [i] = strtoull (p, &p, 0);
		buf->xcpu_sys  [i] = strtoull (p, &p, 0);
		buf->xcpu_idle [i] = strtoull (p, &p, 0);
		buf->xcpu_total[i] = buf->xcpu_user [i] \
			+ buf->xcpu_nice [i] \
			+ buf->xcpu_sys  [i] \
			+ buf->xcpu_idle [i];
	}

	if(server->ncpu) /* ok, that's a real SMP */
		buf->flags |= _glibtop_sysdeps_cpu_smp;
}
