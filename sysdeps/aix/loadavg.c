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
#include <nlist.h>
#include <glibtop.h>
#include <glibtop/loadavg.h>
#include <glibtop/error.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_loadavg =
(1 << GLIBTOP_LOADAVG_LOADAVG);

/* Init function. */

void
_glibtop_init_loadavg_p (glibtop *server)
{
	int result;

	result = _glibtop_get_kmem_offset(server, "avenrun");
	if (result == -1)
	{
		server->sysdeps.loadavg = 0;
		return;
	}

	server->machine->loadavg_offset = result;

	server->sysdeps.loadavg = _glibtop_sysdeps_loadavg;
}

/* Provides load averange. */

void
glibtop_get_loadavg_p (glibtop *server, glibtop_loadavg *buf)
{
	int result;
	int loadavg[3];

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_LOADAVG), 0);

	memset (buf, 0, sizeof (glibtop_loadavg));

	result = _glibtop_get_kmem_info(server, server->machine->loadavg_offset,
					loadavg, sizeof(loadavg));
        if (result <= 0)
        {
                glibtop_error_io_r (server, "Cannot read loadavg");
                return;
        }

	buf->loadavg[0] = loadavg[0] / 65536.0;
	buf->loadavg[1] = loadavg[1] / 65536.0;
	buf->loadavg[2] = loadavg[2] / 65536.0;

	buf->flags = _glibtop_sysdeps_loadavg;
}
