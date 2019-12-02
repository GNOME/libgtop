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

#include <glibtop/mountlist.h>
#include <glibtop/fsusage.h>

#ifndef PROFILE_COUNT
#define PROFILE_COUNT	1000
#endif

int
main (int argc, char *argv [])
{
	glibtop_fsusage fsusage;
	glibtop_mountlist mount_list;
	glibtop_mountentry *mount_entries;
	unsigned c, index, method, count, port;
	char buffer [BUFSIZ];

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

#ifdef HAVE_SBRK
	printf ("sbrk (0) = %p\n\n", sbrk (0));
#endif

	for (c = 0; c < PROFILE_COUNT; c++) {
		mount_entries = glibtop_get_mountlist (&mount_list, 1);

		g_free (mount_entries);
	}

#ifdef HAVE_SBRK
	printf ("sbrk (0) = %p\n\n", sbrk (0));
#endif

	mount_entries = glibtop_get_mountlist (&mount_list, 1);

	if (mount_entries == NULL)
		_exit (1);

	for (index = 0; index < mount_list.number; index++)
		printf ("Mount_Entry: %-30s %-10s %-20s\n",
			mount_entries [index].mountdir,
			mount_entries [index].type,
			mount_entries [index].devname);

	printf ("\n\n%-16s %9s %9s %9s %9s %9s %9s\n",
		"Mount", "Blocks", "Free", "Avail", "Files", "Free", "BlockSz");

	for (index = 0; index < mount_list.number; index++) {
		glibtop_get_fsusage (&fsusage,
				     mount_entries [index].mountdir);

		printf ("%-16s %9" G_GUINT64_FORMAT " %9" G_GUINT64_FORMAT " %9" G_GUINT64_FORMAT " %9" G_GUINT64_FORMAT " %9" G_GUINT64_FORMAT " %9d\n",
			mount_entries [index].mountdir,
			fsusage.blocks, fsusage.bfree,
			fsusage.bavail, fsusage.files,
			fsusage.ffree, fsusage.block_size);
	}

	g_free (mount_entries);

#ifdef HAVE_SBRK
	printf ("\nsbrk (0) = %p\n\n", sbrk (0));
#endif

	glibtop_close ();

	exit (0);
}
