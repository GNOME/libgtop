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
#include <glibtop/global.h>
#include <glibtop/xmalloc.h>

#include <glibtop/open.h>
#include <glibtop/backend.h>
#include <glibtop-backend-private.h>

static int
_open_sysdeps (glibtop *, glibtop_backend *, u_int64_t, const char **);

static int
_close_sysdeps (glibtop *, glibtop_backend *);

extern glibtop_call_vector glibtop_backend_sysdeps_call_vector;

glibtop_backend_info glibtop_backend_sysdeps = {
    "glibtop-backend-sysdeps", _open_sysdeps, _close_sysdeps,
    &glibtop_backend_sysdeps_call_vector
};

static int
_open_sysdeps (glibtop *server, glibtop_backend *backend,
	       u_int64_t features, const char **backend_args)
{
    glibtop_init_func_t *init_fkt;

    backend->_priv = glibtop_calloc_r
	(server, 1, sizeof (glibtop_backend_private));

    /* Do the initialization, but only if not already initialized. */

    if ((server->flags & _GLIBTOP_INIT_STATE_SYSDEPS) == 0) {
	glibtop_open_s (server, "glibtop", features, 0);

	for (init_fkt = _glibtop_init_hook_s; *init_fkt; init_fkt++)
	    (*init_fkt) (server);

	server->sysdeps.pointer_size = sizeof (void*)*8;
		
	server->flags |= _GLIBTOP_INIT_STATE_SYSDEPS;
    }

    return 0;
}

static int
_close_sysdeps (glibtop *server, glibtop_backend *backend)
{
    return -1;
}
