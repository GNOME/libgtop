/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, October 1998.

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
#include <glibtop/netinfo.h>

static const unsigned long _glibtop_sysdeps_netinfo = 0;

/* Init function. */

int
glibtop_init_netinfo_s (glibtop *server)
{
    server->sysdeps.netinfo = _glibtop_sysdeps_netinfo;

    return 0;
}

/* Provides network statistics. */

glibtop_ifaddr *
glibtop_get_netinfo_s (glibtop *server, glibtop_array *array,
		       glibtop_netinfo *buf, const char *interface,
		       u_int64_t transport)
{
    memset (buf, 0, sizeof (glibtop_netinfo));

    return NULL;
}
