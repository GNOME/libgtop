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

static const unsigned long _glibtop_sysdeps_cpu =
(1L << GLIBTOP_CPU_TOTAL) + (1L << GLIBTOP_CPU_USER) +
(1L << GLIBTOP_CPU_NICE) + (1L << GLIBTOP_CPU_SYS) +
(1L << GLIBTOP_CPU_IDLE) + (1L << GLIBTOP_CPU_FREQUENCY);

/* Init function. */

void
_glibtop_init_cpu_s (glibtop *server)
{
	server->sysdeps.cpu = _glibtop_sysdeps_cpu;
}

/* Provides information about cpu usage. */

void
glibtop_get_cpu_s (glibtop *server, glibtop_cpu *buf)
{
	struct tbl_sysinfo sysinfo;
	int ret;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_CPU, 0);

	memset (buf, 0, sizeof (glibtop_cpu));

	ret = table (TBL_SYSINFO, 0, (char *) &sysinfo, 1,
		     sizeof (struct tbl_sysinfo));

	if (ret != 1) return;

	buf->flags = _glibtop_sysdeps_cpu;

	buf->user = sysinfo.si_user;
	buf->nice = sysinfo.si_nice;
	buf->sys  = sysinfo.si_sys;
	buf->idle = sysinfo.si_idle;

	buf->total = sysinfo.si_user + sysinfo.si_nice +
		sysinfo.si_sys + sysinfo.si_idle;

	buf->frequency = sysinfo.si_hz;
}
