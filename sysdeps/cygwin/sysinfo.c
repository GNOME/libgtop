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
#include <glibtop/cpu.h>
#include <glibtop/sysinfo.h>

#include "glibtop_private.h"

#define FILENAME "/proc/cpuinfo"

static const unsigned long _glibtop_sysdeps_sysinfo =
(1L << GLIBTOP_SYSINFO_CPUINFO);

static glibtop_sysinfo sysinfo = { .flags = 0 };

static void
init_sysinfo (glibtop *server)
{
	char buffer [16384];
	gchar ** processors;

	if(G_LIKELY(sysinfo.flags)) return;

	file_to_buffer(server, buffer, sizeof buffer, FILENAME);

	/* cpuinfo records are seperated by a blank line */
	processors = g_strsplit(buffer, "\n\n", 0);

	for(sysinfo.ncpu = 0;
	    sysinfo.ncpu < GLIBTOP_NCPU && processors[sysinfo.ncpu] && *processors[sysinfo.ncpu];
	    sysinfo.ncpu++) {

		gchar **parts, **p;

		glibtop_entry * const cpuinfo = &sysinfo.cpuinfo[sysinfo.ncpu];

		cpuinfo->labels = g_ptr_array_new ();

		cpuinfo->values = g_hash_table_new_full(g_str_hash, g_str_equal,
							g_free, g_free);

		cpuinfo->descriptions = g_hash_table_new_full(g_str_hash, g_str_equal,
							g_free, g_free);

		/* "<key>    : <value>" */
		parts = g_strsplit_set(processors[sysinfo.ncpu], ":\n", 0);

		for(p = parts; *p && *(p+1); p += 2) {

			/* stole the allocated memory */
			gchar * const key   = g_strstrip(   *p   );
			gchar * const value = g_strstrip( *(p+1) );

			g_ptr_array_add(cpuinfo->labels, key);
			g_hash_table_insert(cpuinfo->values, key, value);
		}


		/* the last key has no value and has not been added */
		if(*p) g_free(*p);

		/* just g_free instead of g_strvfree because we stole
		   the memory*/

		g_free(parts);

	}

	g_strfreev(processors);

	sysinfo.flags = _glibtop_sysdeps_sysinfo;
}

const glibtop_sysinfo *
glibtop_get_sysinfo_s (glibtop *server)
{
	init_sysinfo (server);
	return &sysinfo;
}
