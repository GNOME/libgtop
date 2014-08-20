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
#include <glibtop/proctime.h>
#include <glibtop/uptime.h>

#include "glibtop_private.h"

static const unsigned long _glibtop_sysdeps_proc_time =
(1L << GLIBTOP_PROC_TIME_UTIME) + (1L << GLIBTOP_PROC_TIME_CUTIME) +
(1L << GLIBTOP_PROC_TIME_RTIME) +
(1L << GLIBTOP_PROC_TIME_STIME) + (1L << GLIBTOP_PROC_TIME_CSTIME) +
(1L << GLIBTOP_PROC_TIME_FREQUENCY) + (1L << GLIBTOP_PROC_TIME_TIMEOUT) +
(1L << GLIBTOP_PROC_TIME_IT_REAL_VALUE) + (1L << GLIBTOP_PROC_TIME_START_TIME);

static const unsigned long _glibtop_sysdeps_proc_time_smp =
(1L << GLIBTOP_PROC_TIME_XCPU_UTIME) + (1L << GLIBTOP_PROC_TIME_XCPU_STIME);

/* Init function. */

void
_glibtop_init_proc_time_s (glibtop *server)
{
	server->sysdeps.proc_time = _glibtop_sysdeps_proc_time;

	if (server->ncpu)
		server->sysdeps.proc_time |= _glibtop_sysdeps_proc_time_smp;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_time_s (glibtop *server, glibtop_proc_time *buf, pid_t pid)
{
	char buffer [BUFSIZ], *p;
	int i;

	memset (buf, 0, sizeof (glibtop_proc_time));

	if (proc_stat_to_buffer(buffer, sizeof buffer, pid))
		return;

	p = proc_stat_after_cmd (buffer);
	if (!p) return;

	p = skip_multiple_token (p, 11);

	/* clock_t  (1/100 s) */
	buf->utime  = strtoull (p, &p, 0);
	buf->stime  = strtoull (p, &p, 0);
	buf->rtime = buf->utime + buf->stime;

	buf->cutime = strtoull (p, &p, 0);
	buf->cstime = strtoull (p, &p, 0);

	p = skip_multiple_token (p, 3);

	buf->it_real_value = strtoull (p, &p, 0);

	/* seconds since epoch */
	{
	  buf->start_time = get_boot_time(server) + strtoull (p, &p, 0) / 100;
	}

	buf->frequency = 100;

	buf->flags = _glibtop_sysdeps_proc_time;

	if (!server->ncpu)
		return;

	if (proc_file_to_buffer(buffer, sizeof buffer, "/proc/%d/cpu", pid))
		return;

	p = skip_multiple_token (p, 3);

	for (i = 0; i <= server->ncpu; i++) {

		if (!check_cpu_line_warn(server, p + 1, i))
			break;

		p = skip_token(p);
		buf->xcpu_utime [i] = strtoull (p, &p, 0);
		buf->xcpu_stime [i] = strtoull (p, &p, 0);
	}

	buf->flags |= _glibtop_sysdeps_proc_time_smp;
}
