/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, March 1999.

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

#ifdef HAVE_GETLOADAVG
#include <sys/loadavg.h>
#else
#include <assert.h>
#include <sys/param.h>
#include <kstat.h>
#endif

static const unsigned long _glibtop_sysdeps_loadavg =
(1L << GLIBTOP_LOADAVG_LOADAVG);

/* Init function. */

void
_glibtop_init_loadavg_s (glibtop *server)
{
	server->sysdeps.loadavg = _glibtop_sysdeps_loadavg;
}

/* Provides load average. */

void
glibtop_get_loadavg_s (glibtop *server, glibtop_loadavg *buf)
{
#ifndef HAVE_GETLOADAVG
	kstat_ctl_t *kc;
	kstat_t *ksp;
	size_t i;
	static const char avestrings[][14] = { "avenrun_1min",
					       "avenrun_5min",
					       "avenrun_15min" };
#endif
	memset (buf, 0, sizeof (glibtop_loadavg));

#ifdef HAVE_GETLOADAVG
	if (getloadavg (buf->loadavg, 3) != 3)
		return;

#else
	if(!(kc = server->machine->kc))
	    return;

	switch(kstat_chain_update(kc))
	{
	    case -1: assert(0); /* Debugging, shouldn't happen */
	    case 0:  break;
	    default: glibtop_get_kstats(server);
	}

	if(!(ksp = server->machine->system))
	    return;

	if(kstat_read(kc, ksp, NULL) < 0)
	    return;

	for(i = 0; i < 3; ++i) /* Do we have a countof macro? */
	{
		kstat_named_t *kn;

		kn = (kstat_named_t *)kstat_data_lookup(ksp, avestrings[i]);
		if(kn)
		    buf->loadavg[i] = (double)kn->value.ul / FSCALE;
	}
#endif /* HAVE_GETLOADAVG */

	buf->flags = _glibtop_sysdeps_loadavg;
}
