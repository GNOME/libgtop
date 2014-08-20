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
#include <glibtop/loadavg.h>

#include "glibtop_private.h"

static const unsigned long _glibtop_sysdeps_loadavg =
(1L << GLIBTOP_LOADAVG_LOADAVG);

static const unsigned long _glibtop_sysdeps_loadavg_tasks =
(1L << GLIBTOP_LOADAVG_NR_RUNNING) +
(1L << GLIBTOP_LOADAVG_NR_TASKS) +
(1L << GLIBTOP_LOADAVG_LAST_PID);

/* Init function. */

void
_glibtop_init_loadavg_s (glibtop *server)
{
	server->sysdeps.loadavg = _glibtop_sysdeps_loadavg;
}

/* Provides load load averange. */

#define FILENAME	"/proc/loadavg"

void
glibtop_get_loadavg_s (glibtop *server, glibtop_loadavg *buf)
{
	char buffer [BUFSIZ], *p, *old;

	memset (buf, 0, sizeof (glibtop_loadavg));

	file_to_buffer(server, buffer, sizeof buffer, FILENAME);

	buf->loadavg [0] = g_ascii_strtod (buffer, &p);
	buf->loadavg [1] = g_ascii_strtod (p, &p);
	buf->loadavg [2] = g_ascii_strtod (p, &p);

	buf->flags = _glibtop_sysdeps_loadavg;

	p = next_token(p);

	/* Older Linux versions don't have the nr_running/nr_tasks fields. */

	old = p;
	while (*p) {
		if (*p == '/')
			break;
		if (!isdigit (*p))
			return;
		p++;
	}

	buf->nr_running  = strtoull (old, &p, 0); p++;
	buf->nr_tasks    = strtoull (p, &p, 0);
	buf->last_pid    = strtoull (p, &p, 0);

	buf->flags |= _glibtop_sysdeps_loadavg_tasks;
}
