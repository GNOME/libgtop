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

	glibtop_init_r (&server, 0, 0);
	
	if (read (server->input [0], &ssize, sizeof (size_t)) < 0)
		glibtop_error_r (server, _("read size: %s"), strerror (errno));
	
	if (size != ssize)
		glibtop_error_r (server, _("got %d bytes but requested %d"), ssize, size);
	
	if (read (server->input [0], buf, size) < 0)
		glibtop_error_r (server, _("read %d bytes: %s"), size, strerror (errno));
}
