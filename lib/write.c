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

#include <glibtop/write.h>

/* Writes some data to server. */

void
glibtop_write__l (glibtop *server, size_t size, void *buf)
{
	glibtop_init__r (&server);

	if (write (server->output [1], &size, sizeof (size_t)) < 0)
		glibtop_error__r (server, _("write size: %s"), strerror (errno));

	if (!size) return;
	
	if (write (server->output [1], buf, size) < 0)
		glibtop_error__r (server, _("write %d bytes: %s"), size, strerror (errno));
}
