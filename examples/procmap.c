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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <locale.h>
#include <libintl.h>
#include <stdio.h>

#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/close.h>

#include <glibtop/parameter.h>

#include <glibtop/procmap.h>


#include <sys/resource.h>
#include <sys/mman.h>

#include <fcntl.h>

#ifndef PROFILE_COUNT
#define PROFILE_COUNT	1
#endif

static void
try_mmap(const char *path)
{
	struct stat buf;
	int fd;

	if ((fd = open(path, O_RDONLY)) < 0)
		goto out;

	if (fstat(fd, &buf) < 0)
		goto out;

	if (mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0) == MAP_FAILED)
		goto out;

	close(fd);

	return;

out:
	fprintf(stderr, "Failed to test mmap with '%s'\n", path);
}


int
main (int argc, char *argv [])
{
	glibtop_proc_map procmap;
	glibtop_map_entry *maps;
	unsigned method, count, port, i;
	char buffer [BUFSIZ];
	pid_t pid;

	count = PROFILE_COUNT;

	setlocale (LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, GTOPLOCALEDIR);
	textdomain (GETTEXT_PACKAGE);

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

	if (argc == 1) {
		pid = getpid();
	}
	else if ((argc != 2) || (sscanf (argv [1], "%d", (int *) &pid) != 1))
		g_error ("Usage: %s [pid]", argv [0]);

	if (pid == getpid()) {
		/* let's map something for a try */
		try_mmap("/etc/passwd");
		try_mmap("/etc/resolv.conf");
		try_mmap(argv[0]);
	}

	fprintf (stderr, "Getting memory maps for pid %d.\n\n", (int) pid);

	maps = glibtop_get_proc_map (&procmap, pid);

	for (i = 0; i < procmap.number; i++) {
		const char *filename = NULL;
		char * format;
		unsigned device, device_major, device_minor;
		char perm [5];

		if (maps [i].flags & (1L << GLIBTOP_MAP_ENTRY_FILENAME))
			filename = maps [i].filename;

		perm [0] = (maps [i].perm & GLIBTOP_MAP_PERM_READ) ? 'r' : '-';
		perm [1] = (maps [i].perm & GLIBTOP_MAP_PERM_WRITE) ? 'w' : '-';
		perm [2] = (maps [i].perm & GLIBTOP_MAP_PERM_EXECUTE) ? 'x' : '-';
		perm [3] = (maps [i].perm & GLIBTOP_MAP_PERM_SHARED) ? 's' : '-';
		perm [4] = (maps [i].perm & GLIBTOP_MAP_PERM_PRIVATE) ? 'p' : '-';

		device = (unsigned long) maps [i].device;
		device_minor = (device & 255);
		device_major = ((device >> 8) & 255);

		if (sizeof (void*) == 8)
			format = "%016lx-%016lx +%016lx (%8lu bytes mapped) - "
				 "%02x:%02x % 8lu - %.*s";
		else
			format = "%08lx-%08lx +%08lx (%8lu bytes mapped) - "
				 "%02x:%02x % 8lu - %.*s";

		fprintf (stderr, format,
			 (unsigned long) maps [i].start,
			 (unsigned long) maps [i].end,
			 (unsigned long) maps [i].offset,
			 (unsigned long) (maps [i].end - (maps [i].start + maps [i].offset)),
			 device_major, device_minor,
			 (unsigned long) maps [i].inode,
			 sizeof perm, perm);

		if (filename) {
			fprintf(stderr, " - %s", filename);
		}

		fputc('\n', stderr);

		g_print("smaps flags:%#llx size:%" G_GUINT64_FORMAT "KiB rss:%" G_GUINT64_FORMAT "KiB "
			"shared_clean:%" G_GUINT64_FORMAT "Kib shared_dirty:%" G_GUINT64_FORMAT "KiB "
			"private_clean:%" G_GUINT64_FORMAT "KiB private_dirty: %" G_GUINT64_FORMAT "KiB\n",
			(unsigned long long)maps[i].flags,
			maps[i].size >> 10,
			maps[i].rss >> 10,
			maps[i].shared_clean >> 10,
			maps[i].shared_dirty >> 10,
			maps[i].private_clean >> 10,
			maps[i].private_dirty >> 10);

		if (filename && (filename != maps [i].filename))
			g_free ((void*)filename);
	}

	g_free (maps);

	glibtop_close ();

	exit (0);
}
