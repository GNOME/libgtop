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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/backend.h>

#include <glibtop-backend-private.h>

void
glibtop_read_i (glibtop *server, glibtop_backend *backend,
		size_t size, void *buf)
{
    int ret = 0;

    glibtop_init_r (&server, 0, 0);

#ifdef DEBUG
    fprintf (stderr, "LIBRARY: really reading %d bytes from %d.\n",
	     size, backend->_priv->input [0]);
#endif

    ret = read (backend->_priv->input [0], buf, size);

#ifdef DEBUG
    fprintf (stderr, "LIBRARY: read %d bytes.\n", ret);
#endif

    if (ret < 0)
	glibtop_error_io_r (server, "read %d bytes", size);
}
