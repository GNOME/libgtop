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

#include <locale.h>

#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/close.h>
#include <glibtop/xmalloc.h>

#include <glibtop/parameter.h>

#include <glibtop/procmap.h>

#include <sys/resource.h>
#include <sys/mman.h>

#ifndef PROFILE_COUNT
#define PROFILE_COUNT	1
#endif

int
main (int argc, char *argv [])
{
	glibtop_proc_map procmap;
	glibtop_map_entry *maps;
	unsigned method, count, port, i, *ptr;
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

	if ((argc != 2) || (sscanf (argv [1], "%d", &pid) != 1))
		glibtop_error ("Usage: %s pid", argv [0]);

	fprintf (stderr, "Getting memory maps for pid %d.\n\n", pid);

	maps = glibtop_get_proc_map (&procmap, pid);

	for (i = 0; i < procmap.number; i++) {
		fprintf (stderr, "%08x - %08x - %08lu - %08lu - '%s'\n",
			 (unsigned long) maps [i].start,
			 (unsigned long) maps [i].end,
			 (unsigned long) maps [i].device,
			 (unsigned long) maps [i].inode,
			 devname (maps [i].device, S_IFBLK));
	}

	glibtop_free (maps);

	glibtop_close ();

	exit (0);
}
