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

static const unsigned long _glibtop_sysdeps_loadavg =
(1L << GLIBTOP_LOADAVG_LOADAVG);

/* Init function. */

void
_glibtop_init_loadavg_s (glibtop *server)
{
	server->sysdeps.loadavg = _glibtop_sysdeps_loadavg;
}

/* Provides load averange. */

void
glibtop_get_loadavg_s (glibtop *server, glibtop_loadavg *buf)
{
	struct tbl_loadavg loadavg;
	int ret;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_LOADAVG, 0);

	memset (buf, 0, sizeof (glibtop_loadavg));

	ret = table (TBL_LOADAVG, 0, (char *) &loadavg, 1,
		     sizeof (struct tbl_loadavg));

	if (ret != 1) return;

	buf->flags = _glibtop_sysdeps_loadavg;

	if (loadavg.tl_lscale == 0) {
		buf->loadavg [0] = loadavg.tl_avenrun.d [0];
		buf->loadavg [1] = loadavg.tl_avenrun.d [1];
		buf->loadavg [2] = loadavg.tl_avenrun.d [2];
	} else {
		buf->loadavg [0] =
			(double) loadavg.tl_avenrun.l [0] /
			(double) loadavg.tl_mach_factor [0];
		buf->loadavg [1] =
			(double) loadavg.tl_avenrun.l [1]
			/ (double) loadavg.tl_mach_factor [1];
		buf->loadavg [2] =
			(double) loadavg.tl_avenrun.l [2] /
			(double) loadavg.tl_mach_factor [2];
	}
}
