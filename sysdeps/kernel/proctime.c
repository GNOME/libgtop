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

#include <glibtop.h>
#include <glibtop/proctime.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proc_time =
(1L << GLIBTOP_PROC_TIME_UTIME) + (1L << GLIBTOP_PROC_TIME_STIME) +
(1L << GLIBTOP_PROC_TIME_CUTIME) + (1L << GLIBTOP_PROC_TIME_CSTIME) +
(1L << GLIBTOP_PROC_TIME_START_TIME) + (1L << GLIBTOP_PROC_TIME_FREQUENCY);

static const unsigned long _glibtop_sysdeps_proc_time_smp =
(1L << GLIBTOP_PROC_TIME_XCPU_UTIME) + (1L << GLIBTOP_PROC_TIME_XCPU_STIME);

/* Init function. */

void
glibtop_init_proc_time_s (glibtop *server)
{
    server->sysdeps.proc_time = _glibtop_sysdeps_proc_time;

    if (server->ncpu)
	server->sysdeps.proc_time |= _glibtop_sysdeps_proc_time_smp;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_time_s (glibtop *server, glibtop_proc_time *buf,
			 pid_t pid)
{
    libgtop_proc_state_t proc_state;
    int i;

    memset (buf, 0, sizeof (glibtop_proc_time));

    if (glibtop_get_proc_data_proc_state_s (server, &proc_state, pid))
	return;

    buf->start_time = proc_state.start_time;
    buf->utime = proc_state.utime;
    buf->stime = proc_state.stime;
    buf->cutime = proc_state.cutime;
    buf->cstime = proc_state.cstime;

    buf->frequency = 100;

    buf->flags = _glibtop_sysdeps_proc_time;

    for (i = 0; i < server->ncpu; i++) {
	buf->xcpu_utime [i] = proc_state.per_cpu_utime [i];
	buf->xcpu_stime [i] = proc_state.per_cpu_stime [i];
    }

    if (server->ncpu)
	buf->flags |= _glibtop_sysdeps_proc_time_smp;
}
