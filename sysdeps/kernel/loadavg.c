/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, March 1999.

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
#include <glibtop/loadavg.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_loadavg =
(1L << GLIBTOP_LOADAVG_LOADAVG);

/* Init function. */

int
glibtop_init_loadavg_s (glibtop *server)
{
    server->sysdeps.loadavg = _glibtop_sysdeps_loadavg;

    return 0;
}

/* Provides load averange. */

int
glibtop_get_loadavg_s (glibtop *server, glibtop_loadavg *buf)
{
    libgtop_stat_t stat;
    int retval;

    memset (buf, 0, sizeof (glibtop_loadavg));

    retval = glibtop_get_proc_data_stat_s (server, &stat);
    if (retval)
	return retval;

    buf->loadavg [0] = stat.loadavg [0];
    buf->loadavg [1] = stat.loadavg [1];
    buf->loadavg [2] = stat.loadavg [2];

    buf->flags = _glibtop_sysdeps_loadavg;

    return 0;
}
