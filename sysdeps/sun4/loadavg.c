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
#include <glibtop/loadavg.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_loadavg =
(1 << GLIBTOP_LOADAVG_LOADAVG);

/* Provides load averange. */

void
glibtop_get_loadavg_p (glibtop *server, glibtop_loadavg *buf)
{
	load_avg avenrun [3];
	int i;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_LOADAVG), 0);

	memset (buf, 0, sizeof (glibtop_loadavg));

	/* !!! THE FOLLOWING CODE RUNS SGID KMEM - CHANGE WITH CAUTION !!! */

	setregid (server->machine->gid, server->machine->egid);

	/* get the load average array */

	(void) _glibtop_getkval (server, _glibtop_nlist [X_AVENRUN].n_value,
				 (int *) avenrun, sizeof (avenrun),
				 _glibtop_nlist [X_AVENRUN].n_name);

	if (setregid (server->machine->egid, server->machine->gid))
		_exit (1);

	/* !!! END OF SGID KMEM PART !!! */

	for (i = 0; i < 3; i++) {
		/* Calculate loadavg values from avenrun. */
		buf->loadavg [i] = loaddouble (avenrun [i]);
	}

	/* Now we can set the flags. */

	buf->flags = _glibtop_sysdeps_loadavg;
}
