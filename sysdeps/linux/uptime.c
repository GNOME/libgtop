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

#include <config.h>
#include <glibtop/uptime.h>

static unsigned long _glibtop_sysdeps_uptime =
(1 << GLIBTOP_UPTIME_UPTIME) + (1 << GLIBTOP_UPTIME_IDLETIME);

/* Provides uptime and idle time. */

void
glibtop_get_uptime__r (glibtop *server, glibtop_uptime *buf)
{
	FILE *f;

	glibtop_init__r (&server);

	memset (buf, 0, sizeof (glibtop_uptime));

	buf->flags = _glibtop_sysdeps_uptime;

	f = fopen ("/proc/uptime", "r");
	if (!f) return;

	fscanf (f, "%lf %lf\n", &buf->uptime, &buf->idletime);

	fclose (f);
}
