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
#include <glibtop/netlist.h>
#include <glibtop/error.h>

#include <stdio.h>


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
	char line[1024];
	FILE *f;
	GPtrArray *devices;

	memset (buf, 0, sizeof (glibtop_netlist));

	f = fopen("/proc/net/dev", "r");

	if(!f)
	{
		glibtop_warn_io_r (server, "cannot open \"/proc/net/dev\"");
		return NULL;
	}

	devices = g_ptr_array_new();

	while(fgets(line, sizeof line, f))
	{
		char *sep = strchr(line, ':');

		if(!sep) continue;

		*sep = '\0'; /* truncate : we only need the name */

		g_strstrip(line);
		g_ptr_array_add(devices, g_strdup(line));
		buf->number++;
	}

	fclose(f);

	buf->flags = _glibtop_sysdeps_netlist;

	g_ptr_array_add(devices, NULL);

	return (char**) g_ptr_array_free(devices, FALSE);
}

