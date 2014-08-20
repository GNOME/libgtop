/* Copyright (C) 1998-99 Benoît Dejean
   This file is part of LibGTop 2.0.

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
#include <string.h>
#include <kstat.h>

#include <glibtop/netlist.h>



static const unsigned long _glibtop_sysdeps_netlist = (1 << GLIBTOP_NETLIST_NUMBER);

/* Init function. */

void
_glibtop_init_netlist_s (glibtop *server)
{
	server->sysdeps.netlist = _glibtop_sysdeps_netlist;
}


char**
glibtop_get_netlist_s (glibtop *server, glibtop_netlist *buf)
{
	GPtrArray *devices;
	kstat_t *ksp;
	kstat_ctl_t *kc;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_NETLIST, 0);

	memset (buf, 0, sizeof (glibtop_netlist));

	devices = g_ptr_array_new();

	kc = kstat_open();
	if (kc == NULL) {
		glibtop_warn_io_r(server, "kstat_open()");
		return NULL;
	}
	
	for (ksp = kc->kc_chain; ksp; ksp = ksp->ks_next)
	{
		if (strcmp(ksp->ks_class, "net") != 0)
			continue;

		g_ptr_array_add(devices, g_strdup(ksp->ks_name));
		buf->number++;
	}

	kstat_close(kc);

	buf->flags = _glibtop_sysdeps_netlist;

	g_ptr_array_add(devices, NULL);

	return (char**) g_ptr_array_free(devices, FALSE);
}
