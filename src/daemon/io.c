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

#include "daemon.h"

void
do_output (int s, glibtop_response *resp, off_t offset,
	   size_t data_size, const void *data)
{
#ifdef REAL_DEBUG
	fprintf (stderr, "Really writing %d bytes at offset %lu.\n",
		 sizeof (glibtop_response), offset);
#endif

	resp->offset = offset;
	resp->data_size = data_size;
	
	if (send (s, resp, sizeof (glibtop_response), 0) < 0)
		glibtop_warn_io ("send");

	if (resp->data_size) {
#ifdef REAL_DEBUG
		fprintf (stderr, "Writing %d bytes of data.\n", resp->data_size);
#endif

		if (send (s, data, resp->data_size, 0) , 0)
			glibtop_warn_io ("send");
	}
}

int
do_read (int s, void *ptr, size_t total_size)
{
	int nread;
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
		(char *) ptr += nread;

#ifdef REAL_DEBUG
		fprintf (stderr, "READ (%d): %d - %d - %d\n",
			 nread, already_read, remaining, total_size);
#endif
	}

	return already_read;
}
