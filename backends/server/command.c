/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/* $Id$ */

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

#include <backend-server.h>

#include "command.h"

void *
glibtop_call_i (glibtop_server *server, backend_server_private *priv,
		unsigned command, size_t send_size, const void *send_buf,
		size_t data_size, const void *data_buf,
		size_t recv_size, void *recv_ptr,
		int *retval_ptr)
{
    glibtop_command cmnd = {0};
    glibtop_response resp = {0};

    cmnd.command = command;

    /* If send_size is less than _GLIBTOP_PARAM_SIZE (normally 16 Bytes), we
     * send it together with command, so we only need one system call instead
     * of two. */

    if (send_size <= _GLIBTOP_PARAM_SIZE) {
	memcpy (cmnd.parameter, send_buf, send_size);
	cmnd.param_size = send_size;
    } else {
	cmnd.send_size = send_size;
    }

    cmnd.data_size = data_size;

    glibtop_write_i (priv, sizeof (glibtop_command), &cmnd);

    if (data_size) {
#ifdef SLAVE_DEBUG
	fprintf (stderr, "SENDING %d bytes of DATA.\n", data_size);
#endif
	glibtop_write_i (priv, data_size, data_buf);
    }

    glibtop_read_i (priv, sizeof (glibtop_response), &resp);

#ifdef SLAVE_DEBUG
    fprintf (stderr, "RESPONSE: %d - %d - %ld - %ld - %p - %ld\n",
	     resp.retval, resp.glibtop_errno,
	     (long) resp.recv_size, (long) resp.data_size,
	     recv_ptr, (long) recv_size);
#endif

    if (retval_ptr)
	*retval_ptr = resp.glibtop_errno;

    if (resp.recv_size != recv_size) {
	glibtop_warn_r (server, "Expected %ld bytes, but got %ld.",
			(long) recv_size, (long) resp.recv_size);
	return NULL;
    }

    if (recv_ptr)
	glibtop_read_i (priv, recv_size, recv_ptr);

    if (resp.data_size) {
	void *ptr = g_malloc (resp.data_size);

	glibtop_read_i (priv, resp.data_size, ptr);

	return ptr;
    }

    return NULL;
}
