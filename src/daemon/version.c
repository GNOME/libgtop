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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/version.h>

void
glibtop_send_version (glibtop *server, int fd)
{
	char buffer [BUFSIZ];
	size_t size;

	sprintf (buffer, LIBGTOP_VERSION_STRING,
		 LIBGTOP_VERSION, LIBGTOP_SERVER_VERSION,
		 sizeof (glibtop_command),
		 sizeof (glibtop_response),
		 sizeof (glibtop_union),
		 sizeof (glibtop_sysdeps));
	
	size = strlen (buffer) + 1;

	fprintf (stderr, "SERVER ID: |%s|\n", buffer);

	if (fd == 0) {
		if (write (1, &size, sizeof (size)) < 0)
			glibtop_warn_io_r (server, "write");
	} else {
		if (send (fd, &size, sizeof (size), 0) < 0)
			glibtop_warn_io_r (server, "send");
	}

	if (fd == 0) {
		if (write (1, buffer, size) < 0)
			glibtop_warn_io_r (server, "write");
	} else {
		if (send (fd, buffer, size, 0) < 0)
			glibtop_warn_io_r (server, "send");
	}
}
