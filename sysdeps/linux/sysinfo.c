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
extend_sysinfo (glibtop *server)
{
	/* expand the already retrieved information by augmenting the output of lscpu into it */
	const char* key = "model name";
	g_autofree char *output = NULL;
	g_autofree char *lscpu = NULL;
	gint exit_status;
	g_autoptr (GError) error = NULL;
	g_auto (GStrv) lines = NULL;
	g_auto (GStrv) fields = NULL;
	guint i;
	const char *command = "lscpu --parse=cpu,MODELNAME";

	if (g_spawn_command_line_sync (command, &output, NULL, &exit_status, &error)) {
		if (!g_spawn_check_wait_status (exit_status, &error)) {
			glibtop_warn_r (server, "Could not execute \"%s\" (%i)", command, exit_status);
			return;
		}
	} else {
		glibtop_warn_r (server, "Failed to spawn \"lscpu\"");
	}

	/* use output form here on */
	if (output == NULL)
		return;

	lines = g_strsplit (output, "\n", 0);
	char *model;
	model = NULL;

	for (i = 0; lines[i]; i++) {
		if (*lines[i] == '#')
			continue;

		fields = g_strsplit (lines[i], ",", 3);
		if (g_strv_length (fields) < 2) {
			continue;
		}

		/* parse cpu number and model name */
		guint cpu_num = g_ascii_strtoull (fields[0], NULL, 0);

		if (cpu_num >= sysinfo.ncpu) {
			continue;
		}
		
		/* check if model name field is already present */
		glibtop_entry * const cpuinfo = &sysinfo.cpuinfo[cpu_num];
		model = g_hash_table_lookup (cpuinfo->values, key);

		if (model == NULL) {
			g_ptr_array_add (cpuinfo->labels, key);
			g_hash_table_insert (cpuinfo->values, g_strdup (key), g_strdup (fields[1]));
		}
	}
}

static void
init_sysinfo (glibtop *server)
{
	char* buffer;
	gchar ** processors;

	if(G_LIKELY(sysinfo.flags)) return;

	if (!g_file_get_contents(FILENAME, &buffer, NULL, NULL)) {
		glibtop_error_io_r(server, "g_file_get_contents(%s)", FILENAME);
	}

	/* cpuinfo records are seperated by a blank line */
	processors = g_strsplit(buffer, "\n\n", 0);

	g_free(buffer);

	sysinfo.ncpu = 0;
	for (char** this_proc = &processors[0]; *this_proc && **this_proc; this_proc++) {

		if (sysinfo.ncpu >= GLIBTOP_NCPU) {
			glibtop_warn_r(server, "Cannot deal with more than %d CPUs", GLIBTOP_NCPU);
			break;
		}

		gchar **parts, **p;
		if (g_strrstr (*this_proc, "processor" ) == NULL) {
		  /* skip unknown paragraph */
		  continue;
		}

		glibtop_entry * const cpuinfo = &sysinfo.cpuinfo[sysinfo.ncpu];

		cpuinfo->labels = g_ptr_array_new ();

		cpuinfo->values = g_hash_table_new_full(g_str_hash, g_str_equal,
							g_free, g_free);

		cpuinfo->descriptions = g_hash_table_new_full(g_str_hash, g_str_equal,
							g_free, g_free);

		/* "<key>    : <value>" */
		parts = g_strsplit_set(*this_proc, ":\n", 0);

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

		sysinfo.ncpu++;
	}

	g_strfreev(processors);
	extend_sysinfo (server);

	sysinfo.flags = _glibtop_sysdeps_sysinfo;
}

const glibtop_sysinfo *
glibtop_get_sysinfo_s (glibtop *server)
{
	init_sysinfo (server);
	return &sysinfo;
}
