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

#include <backend-common.h>

static int
_open_common (glibtop_server *, glibtop_backend *, guint64, const char **);

static int
_close_common (glibtop_server *, glibtop_backend *, void *closure);

extern glibtop_call_vector glibtop_backend_common_call_vector;

#ifdef LIBGTOP_USE_GMODULE
glibtop_backend_info LibGTopBackendInfo = {
    "glibtop-backend-common", _open_common, _close_common,
    &glibtop_backend_common_call_vector
};
#else
glibtop_backend_info LibGTopBackendInfo_Common = {
    "glibtop-backend-common", _open_common, _close_common,
    &glibtop_backend_common_call_vector
};
#endif

static int
_open_common (glibtop_server *server, glibtop_backend *backend,
	      guint64 features, const char **backend_args)
{
    return 0;
}

static int
_close_common (glibtop_server *server, glibtop_backend *backend,
	       void *closure)
{
    return 0;
}

