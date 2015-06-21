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

#include <config.h>

#include <glibtop/error.h>
#include <glibtop/read_data.h>
#include <glib/gi18n-lib.h>

/* Reads some data from server. */

void *
glibtop_read_data_l (glibtop *server)
{
	size_t size;
	void *ptr;
	int ret;

	glibtop_init_r (&server, 0, 0);

	glibtop_debug ("LIBRARY: reading %lu data bytes.",
		 (unsigned long) sizeof (size_t));

	if (server->socket) {
		ret = recv (server->socket, &size, sizeof (size_t), 0);
	} else {
		ret = read (server->input [0], &size, sizeof (size_t));
	}

	if (ret < 0)
		glibtop_error_io_r (server, _("read data size"));

	glibtop_debug ("LIBRARY: really reading %lu data bytes (ret = %d).",
		 (unsigned long) size, ret);

	if (!size) return NULL;

	ptr = g_malloc (size);

	if (server->socket) {
		ret = recv (server->socket, ptr, size, 0);
	} else {
		ret = read (server->input [0], ptr, size);
	}

	if (ret < 0)
		glibtop_error_io_r (server,
				    ngettext ("read %lu byte of data",
					      "read %lu bytes of data",
					      (unsigned long) size),
				    (unsigned long) size);

	return ptr;
}
