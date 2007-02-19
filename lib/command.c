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
#include <glibtop/read.h>
#include <glibtop/write.h>
#include <glibtop/read_data.h>

#include <glibtop/command.h>

void *
glibtop_call_l (glibtop *server, unsigned command, size_t send_size,
		const void *send_buf, size_t recv_size, void *recv_buf)
{
	glibtop_command cmnd = {0};
	glibtop_response response = {0};

	glibtop_init_r (&server, 0, 0);

	cmnd.command = command;

	/* If send_size is less than _GLIBTOP_PARAM_SIZE (normally 16 Bytes), we
	 * send it together with command, so we only need one system call instead
	 * of two. */

	if (send_size <= _GLIBTOP_PARAM_SIZE) {
		memcpy (cmnd.parameter, send_buf, send_size);
		cmnd.size = send_size;
	} else {
		cmnd.data_size = send_size;
	}

	glibtop_write_l (server, sizeof (glibtop_command), &cmnd);

	glibtop_read_l (server, sizeof (glibtop_response), &response);

#ifdef DEBUG
	fprintf (stderr, "RESPONSE: %lu - %d\n",
		 response.offset, response.data_size);
#endif

	if (recv_buf)
		memcpy (recv_buf, ((char *) &response) + response.offset,
			recv_size);

	if (response.data_size) {
		void *ptr = g_malloc (response.data_size);

		glibtop_read_l (server, response.data_size, ptr);

		return ptr;
	}

	return NULL;
}
