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

#include <glibtop/read.h>

/* Reads some data from server. */

void
glibtop_read_l (glibtop *server, size_t size, void *buf)
{
	size_t	ssize;
	int ret;

	glibtop_init_r (&server, 0, 0);

#ifdef DEBUG
	fprintf (stderr, "LIBRARY: reading %d bytes, should be %d.\n", sizeof (size_t), size);
#endif

	if (server->socket) {
		ret = recv (server->socket, &ssize, sizeof (size_t), 0);
	} else {
		ret = read (server->input [0], &ssize, sizeof (size_t));
	}

	if (ret < 0)
		glibtop_error_io_r (server, _("read size"));
	
	if (size != ssize)
		glibtop_error_r (server, _("got %d bytes but requested %d (ret = %d)"), ssize, size, ret);

#ifdef DEBUG
	fprintf (stderr, "LIBRARY: really reading %d bytes.\n", size);
#endif

	if (server->socket) {
		ret = recv (server->socket, buf, size, 0);
	} else {
		ret = read (server->input [0], buf, size);
	}

	if (ret < 0)
		glibtop_error_io_r (server, _("read %d bytes"), size);
}
