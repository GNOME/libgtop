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

#include <glibtop/error.h>

#define DEFAULT_NAME	"LibGTop-Server"

/* Prints error message and exits. */

static void
print_server_name (glibtop *server)
{
	fprintf (stderr, "%s: ", server ?
		 (server->name ? server->name : DEFAULT_NAME)
		 : DEFAULT_NAME);
}

void
glibtop_error_vr (glibtop *server, char *format, va_list args)
{
	print_server_name (server);
	vfprintf (stderr, format, args);
	fprintf (stderr, "\n");

#ifdef LIBGTOP_ENABLE_DEBUG
	abort ();
#else
	exit (1);
#endif
}

void
glibtop_error_io_vr (glibtop *server, char *format, int error, va_list args)
{
	print_server_name (server);
	vfprintf (stderr, format, args);
	fprintf (stderr, ": %s\n", strerror (error));

#ifdef LIBGTOP_ENABLE_DEBUG
	abort ();
#else
	exit (1);
#endif
}

void
glibtop_warn_vr (glibtop *server, char *format, va_list args)
{
	print_server_name (server);
	vfprintf (stderr, format, args);
	fprintf (stderr, "\n");

#ifdef LIBGTOP_FATAL_WARNINGS
	abort ();
#endif
}

void
glibtop_warn_io_vr (glibtop *server, char *format, int error, va_list args)
{
	print_server_name (server);
	vfprintf (stderr, format, args);
	fprintf (stderr, ": %s\n", strerror (error));

#ifdef LIBGTOP_FATAL_WARNINGS
	abort ();
#endif
}
