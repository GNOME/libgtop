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
#include "daemon.h"
#include <glibtop/error.h>

void
do_output (int s, glibtop_response *resp, off_t offset,
	   size_t data_size, const void *data)
{
#ifdef DEBUG
	fprintf (stderr, "Really writing %d bytes at offset %lu.\n",
		 sizeof (glibtop_response), offset);
#endif

	resp->offset = offset;
	resp->data_size = data_size;

	if (s == 0) {
		if (write (1, (const void *) resp, sizeof (glibtop_response)) < 0)
			glibtop_warn_io ("write");
	} else {
		if (send (s, (const void *) resp, sizeof (glibtop_response), 0) < 0)
			glibtop_warn_io ("send");
	}

	if (resp->data_size) {
#ifdef DEBUG
		fprintf (stderr, "Writing %d bytes of data.\n", resp->data_size);
#endif

		if (s == 0) {
			if (write (1, data, resp->data_size) < 0)
				glibtop_warn_io ("write");
		} else {
			if (send (s, data, resp->data_size, 0) , 0)
				glibtop_warn_io ("send");
		}
	}
}

int
do_read (int s, void *ptr, size_t total_size)
{
	int nread;
	char *tmp_ptr;
	size_t already_read = 0, remaining = total_size;

	while (already_read < total_size) {
		if (s)
			nread = recv (s, ptr, remaining, 0);
		else
			nread = read (0, ptr, remaining);

		if ((already_read == 0) && (nread == 0)) {
			glibtop_warn ("pid %d received eof.", getpid ());
			return 0;
		}

		if (nread <= 0) {
			glibtop_warn_io ("recv");
			return 0;
		}

		already_read += nread;
		remaining -= nread;
		/* (char *) ptr += nread; */
		tmp_ptr = ptr;
		tmp_ptr += nread;
		ptr = tmp_ptr;

#ifdef DEBUG
		fprintf (stderr, "READ (%d): %d - %d - %d\n",
			 nread, already_read, remaining, total_size);
#endif
	}

	return already_read;
}
