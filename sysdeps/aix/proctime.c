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
#include <unistd.h>

#include <glibtop.h>
#include <glibtop/proctime.h>
#include <glibtop/error.h>

#include "utils.h"

static const unsigned long _glibtop_sysdeps_proc_time =
(1 << GLIBTOP_PROC_TIME_START_TIME) + (1 << GLIBTOP_PROC_TIME_RTIME) +
(1 << GLIBTOP_PROC_TIME_UTIME) + (1 << GLIBTOP_PROC_TIME_STIME) +
(1 << GLIBTOP_PROC_TIME_CUTIME) + (1 << GLIBTOP_PROC_TIME_CSTIME) +
(1 << GLIBTOP_PROC_TIME_FREQUENCY);

/* Init function. */

void
_glibtop_init_proc_time_s (glibtop *server)
{
	server->sysdeps.proc_time = _glibtop_sysdeps_proc_time;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_time_s (glibtop *server, glibtop_proc_time *buf,
			 pid_t pid)
{
	struct procsinfo *pinfo;

	glibtop_init_s (&server, (1L << GLIBTOP_SYSDEPS_PROC_TIME), 0);

	memset (buf, 0, sizeof (glibtop_proc_time));

	pinfo = _glibtop_get_procinfo(server, pid);
	if (pinfo == NULL) return;

	buf->start_time = pinfo->pi_start;
	buf->utime = pinfo->pi_ru.ru_utime.tv_sec + (double)pinfo->pi_ru.ru_utime.tv_usec/1000000.0;
	buf->stime = pinfo->pi_ru.ru_stime.tv_sec + (double)pinfo->pi_ru.ru_stime.tv_usec/1000000.0;
	buf->rtime = buf->utime + buf->stime;
	buf->cutime = pinfo->pi_cru.ru_utime.tv_sec + (double)pinfo->pi_cru.ru_utime.tv_usec/1000000.0;
	buf->cstime = pinfo->pi_cru.ru_stime.tv_sec + (double)pinfo->pi_cru.ru_stime.tv_usec/1000000.0;
	buf->frequency = sysconf(_SC_CLK_TCK);

	buf->flags = _glibtop_sysdeps_proc_time;
}
