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

#include <config.h>
#include <glibtop.h>
#include <glibtop/open.h>

static glibtop	_glibtop_global_server;
glibtop		*glibtop_global_server = NULL;

glibtop *
glibtop_init_r (glibtop **server,
		const unsigned long features,
		const unsigned flags)
{
	if (*server != NULL)
		return *server;

	if (glibtop_global_server == NULL) {
		glibtop_global_server = &_glibtop_global_server;
		glibtop_open (glibtop_global_server, "glibtop",
			      features, flags);
	}

	return *server = glibtop_global_server;
}
