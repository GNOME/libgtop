/* Copyright (C) 1998 Joshua Sled
   This file is part of LibGTop 1.0.

   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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
#include <stdlib.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/loadavg.h>

#include <sys/types.h>
#include <sys/sysctl.h>

static const unsigned long _glibtop_sysdeps_loadavg =
(1L << GLIBTOP_LOADAVG_LOADAVG);

static const unsigned long _glibtop_sysdeps_last_pid =
(1L << GLIBTOP_LOADAVG_LAST_PID);

/* Init function. */

void
_glibtop_init_loadavg_s (glibtop *server)
{
	server->sysdeps.loadavg = _glibtop_sysdeps_loadavg |
		_glibtop_sysdeps_last_pid;
}

/* Provides load averange. */

void
glibtop_get_loadavg_s (glibtop *server, glibtop_loadavg *buf)
{
	pid_t last_pid;
	size_t len;

	memset (buf, 0, sizeof (glibtop_loadavg));

	if (getloadavg (buf->loadavg, G_N_ELEMENTS(buf->loadavg)) == -1)
		return;

	buf->flags = _glibtop_sysdeps_loadavg;

	len = sizeof (last_pid);
	if (sysctlbyname ("kern.lastpid", &last_pid, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.lastpid)");
		return;
	}

	buf->last_pid = last_pid;

	buf->flags |= _glibtop_sysdeps_last_pid;
}
