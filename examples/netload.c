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

#include <glibtop/netload.h>

#ifndef PROFILE_COUNT
#define PROFILE_COUNT	1
#endif

int
main (int argc, char *argv [])
{
	glibtop_netload netload;
	unsigned method, count, port, i;
	char buffer [BUFSIZ];

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

	if (argc != 2)
		glibtop_error ("Usage: %s interface", argv [0]);
	
	glibtop_get_netload (&netload, argv [1]);

	printf ("Network Load (0x%08lx): "
		"%lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu\n",
		(unsigned long) netload.flags,
		(unsigned long) netload.mtu,
		(unsigned long) netload.packets_in,
		(unsigned long) netload.packets_out,
		(unsigned long) netload.bytes_in,
		(unsigned long) netload.bytes_out,
		(unsigned long) netload.errors_in,
		(unsigned long) netload.errors_out,
		(unsigned long) netload.collisions);

	glibtop_close ();

	exit (0);
}
