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
#include <errno.h>
#include <glibtop/write.h>
#include <glibtop/error.h>
#include <glib/gi18n-lib.h>

/* Writes some data to server. */

void
glibtop_write_l (glibtop *server, size_t size, void *buf)
{
	int ret;
	int fd;

	glibtop_init_r (&server, 0, 0);

	if (size == 0) return;

	glibtop_debug("LIBRARY: really writing %d bytes.", (int)size);

	fd = server->socket ? server->socket : server->output[1];

	while ((ret = write(fd, buf, size)) < 0 && errno == EINTR)
		;

	if (ret < 0)
		glibtop_error_io_r (server,
				    ngettext("wrote %d byte",
					     "wrote %d bytes", size),
				    (int) size);
}
