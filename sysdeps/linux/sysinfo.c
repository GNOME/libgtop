/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <config.h>
#include <glibtop/cpu.h>
#include <glibtop/sysinfo.h>

static const unsigned long _glibtop_sysdeps_sysinfo =
(1 << GLIBTOP_SYSINFO_CPUINFO);

static glibtop_sysinfo sysinfo;

static void
init_sysinfo (glibtop *server)
{
	char buffer [BUFSIZ];
	static int init = 0;
	glibtop_entry *cpuinfo = NULL;
	FILE *f;

	if (init) return;

	init = TRUE;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_CPU, 0);

	memset (&sysinfo, 0, sizeof (glibtop_sysinfo));

	g_return_if_fail (f = fopen ("/proc/cpuinfo", "r"));

	while (fgets (buffer, BUFSIZ, f)) {
		char *p, *start, *key, *value;

		if (cpuinfo == NULL) {
			cpuinfo = &sysinfo.cpuinfo [sysinfo.ncpu++];

			cpuinfo->labels = g_ptr_array_new ();

			cpuinfo->values = g_hash_table_new (NULL, NULL);
			
			if (sysinfo.ncpu > GLIBTOP_NCPU)
				sysinfo.ncpu = GLIBTOP_NCPU;
		}

		p = strchr (buffer, ':');
		if (!p) continue;
		
		*p = '\0'; start = p; p++;
		while (isspace (*p)) p++;

		while ((start > buffer) && isspace (*start))
			*start-- = '\0';

		key = g_strdup (buffer);
		value = g_strdup (p);

		g_ptr_array_add (cpuinfo->labels, key);

		g_hash_table_insert (cpuinfo->values, key, value);
	}

	fclose (f);

	sysinfo.flags = _glibtop_sysdeps_sysinfo;
}

glibtop_sysinfo *
glibtop_get_sysinfo_s (glibtop *server)
{
	init_sysinfo (server);
	return &sysinfo;
}
