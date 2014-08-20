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
#include <glibtop/cpu.h>
#include <glibtop/uptime.h>

#include <glibtop_suid.h>

#include <time.h>

static const unsigned long _glibtop_sysdeps_uptime =
(1 << GLIBTOP_UPTIME_UPTIME) + (1 << GLIBTOP_UPTIME_IDLETIME);

/* Provides uptime and idle time. */

void
glibtop_get_uptime_p (glibtop *server, glibtop_uptime *buf)
{
	glibtop_cpu cpu;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_UPTIME), 0);

	/* Get currect cpu usage. */

	glibtop_get_cpu_p (server, &cpu);

	/* Make sure all required fields are present. */

	if (((cpu.flags & (1 << GLIBTOP_CPU_TOTAL)) == 0) ||
	    ((cpu.flags & (1 << GLIBTOP_CPU_IDLE)) == 0) ||
	    ((cpu.flags & (1 << GLIBTOP_CPU_FREQUENCY)) == 0) ||
	    (cpu.frequency == 0))
		return;

	/* Simply calculate uptime and idle time from
	 * cpu usage. */

	buf->uptime = (double) cpu.total / (double) cpu.frequency;
	buf->idletime = (double) cpu.idle / (double) cpu.frequency;
	buf->boot_time = (guint64) time(NULL) - (guint64) buf->uptime;

	buf->flags = _glibtop_sysdeps_uptime;
}
