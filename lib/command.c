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
#include <glibtop/write.h>
#include <glibtop/read_data.h>

#include <glibtop/command.h>
#include <glibtop/xmalloc.h>

void *
glibtop_call_l (glibtop *server, unsigned command, size_t send_size, void *send_buf,
		size_t recv_size, void *recv_buf)
{
	glibtop_command cmnd;
	glibtop_response response;

	glibtop_init_r (&server, 0, 0);

	memset (&cmnd, 0, sizeof (glibtop_command));

	memcpy (&cmnd.server, server, sizeof (glibtop));
	
	cmnd.command = command;

	/* If send_size is less than _GLIBTOP_PARAM_SIZE (normally 16 Bytes), we
	 * send it together with command, so we only need one system call instead
	 * of two. */

#ifdef DEBUG
	//	fprintf (stderr, "COMMAND: send_size = %d; command = %d; sizeof (cmnd) = %d\n",
	// send_size, command, sizeof (glibtop_command));
#endif

	if (send_size <= _GLIBTOP_PARAM_SIZE) {
		memcpy (cmnd.parameter, send_buf, send_size);
		cmnd.size = send_size;
	} else {
		cmnd.data_size = send_size;
	}
	
	glibtop_write_l (server, sizeof (glibtop_command), &cmnd);
	//	glibtop_write_l (server, cmnd.data_size, send_buf);

	glibtop_read_l (server, sizeof (glibtop_response), &response);

	fprintf (stderr, "RESPONSE: %d - %d\n", response.offset, response.data_size);

	if (recv_buf)
		memcpy (recv_buf, ((char *) &response) + response.offset, recv_size);

	if (response.data_size) {
		void *ptr = glibtop_malloc_r (server, response.data_size);

		glibtop_read_l (server, response.data_size, ptr);

		return ptr;
	}

	return NULL;
}
