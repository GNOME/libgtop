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
glibtop_error_r (glibtop *server, char *format, ...)
{
	va_list	ap;
	
	va_start (ap, format);

	print_server_name (server);	
	vfprintf (stderr, format, ap);
	fprintf (stderr, "\n");
	
	va_end (ap);

#ifdef LIBGTOP_ENABLE_DEBUG
	abort ();
#else
	exit (1);
#endif
}

void
glibtop_error_io_r (glibtop *server, char *format, ...)
{
	va_list	ap;
	
	va_start (ap, format);
	
	print_server_name (server);
	vfprintf (stderr, format, ap);
	fprintf (stderr, ": %s\n", strerror (errno));
	
	va_end (ap);

#ifdef LIBGTOP_ENABLE_DEBUG
	abort ();
#else
	exit (1);
#endif
}

void
glibtop_warn_r (glibtop *server, char *format, ...)
{
	va_list	ap;
	
	va_start (ap, format);
	
	print_server_name (server);
	vfprintf (stderr, format, ap);
	fprintf (stderr, "\n");
	
	va_end (ap);

#ifdef LIBGTOP_FATAL_WARNINGS
	abort ();
#endif
}

void
glibtop_warn_io_r (glibtop *server, char *format, ...)
{
	va_list	ap;

	va_start (ap, format);
	
	print_server_name (server);
	vfprintf (stderr, format, ap);
	fprintf (stderr, ": %s\n", strerror (errno));
	
	va_end (ap);

#ifdef LIBGTOP_FATAL_WARNINGS
	abort ();
#endif
}
