/* $Id$ */

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
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <locale.h>

#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/close.h>
#include <glibtop/xmalloc.h>

#include <glibtop/parameter.h>

#include <glibtop/procmap.h>

#ifdef GLIBTOP_INODEDB
#include <glibtop/inodedb.h>
#endif

#include <sys/resource.h>
#include <sys/mman.h>

#ifndef PROFILE_COUNT
#define PROFILE_COUNT	1
#endif

int
main (int argc, char *argv [])
{
#ifdef GLIBTOP_INODEDB
	glibtop_inodedb *inodedb;
#endif
	glibtop_proc_map procmap;
	glibtop_map_entry *maps;
	unsigned method, count, port, i;
	char buffer [BUFSIZ];
	pid_t pid;

	count = PROFILE_COUNT;

	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE, GTOPLOCALEDIR);
	textdomain (PACKAGE);
	
	glibtop_init_r (&glibtop_global_server, 0, GLIBTOP_INIT_NO_OPEN);

	glibtop_get_parameter (GLIBTOP_PARAM_METHOD, &method, sizeof (method));

	printf ("Method = %d\n", method);

	count = glibtop_get_parameter (GLIBTOP_PARAM_COMMAND, buffer, BUFSIZ);
	buffer [count] = 0;

	printf ("Command = '%s'\n", buffer);

	count = glibtop_get_parameter (GLIBTOP_PARAM_HOST, buffer, BUFSIZ);
	buffer [count] = 0;

	glibtop_get_parameter (GLIBTOP_PARAM_PORT, &port, sizeof (port));

	printf ("Host = '%s' - %u\n\n", buffer, port);

	glibtop_init_r (&glibtop_global_server, 0, 0);

	if ((argc != 2) || (sscanf (argv [1], "%d", (int *) &pid) != 1))
		glibtop_error ("Usage: %s pid", argv [0]);

#ifdef GLIBTOP_INODEDB
	inodedb = glibtop_inodedb_open (0, 0);
#endif

	fprintf (stderr, "Getting memory maps for pid %d.\n\n", (int) pid);

	maps = glibtop_get_proc_map (&procmap, pid);

	for (i = 0; i < procmap.number; i++) {
		const char *filename = NULL;
		unsigned device, device_major, device_minor;
		char perm [5];
		
		if (maps [i].flags & (1 << GLIBTOP_MAP_ENTRY_FILENAME))
			filename = maps [i].filename;

#ifdef GLIBTOP_INODEDB
		if (inodedb && !filename)
			filename = glibtop_inodedb_lookup
				(inodedb, maps [i].device, maps [i].inode);
#endif

		perm [0] = (maps [i].perm & GLIBTOP_MAP_PERM_READ) ? 'r' : '-';
		perm [1] = (maps [i].perm & GLIBTOP_MAP_PERM_WRITE) ? 'w' : '-';
		perm [2] = (maps [i].perm & GLIBTOP_MAP_PERM_EXECUTE) ? 'x' : '-';
		perm [3] = (maps [i].perm & GLIBTOP_MAP_PERM_SHARED) ? 's' : '-';
		perm [4] = (maps [i].perm & GLIBTOP_MAP_PERM_PRIVATE) ? 'p' : '-';

		device = (unsigned long) maps [i].device;
		device_minor = (device & 255);
		device_major = ((device >> 8) & 255);

		if (filename) {
			char *format;

			if (sizeof (void*) == 8)
				format = "%016lx-%016lx %016lx - "
					 "%02x:%02x %08lu - %4s - %s\n";
			else
				format = "%08lx-%08lx %08lx - "
					 "%02x:%02x %08lu - %4s - %s\n";

			fprintf (stderr, format,
				 (unsigned long) maps [i].start,
				 (unsigned long) maps [i].end,
				 (unsigned long) maps [i].offset,
				 device_major, device_minor,
				 (unsigned long) maps [i].inode,
				 perm, filename);
		} else {
			char * format;

			if (sizeof (void*) == 8)
				format = "%016lx-%016lx %016lx - "
					 "%02x:%02x %08lu - %4s\n";
			else
				format = "%08lx-%08lx %08lx - "
					 "%02x:%02x %08lu - %4s\n";

			fprintf (stderr, format,
				 (unsigned long) maps [i].start,
				 (unsigned long) maps [i].end,
				 (unsigned long) maps [i].offset,
				 device_major, device_minor,
				 (unsigned long) maps [i].inode,
				 perm);
		}

		if (filename && (filename != maps [i].filename))
			glibtop_free (filename);
	}

	glibtop_free (maps);

	glibtop_close ();

	exit (0);
}
