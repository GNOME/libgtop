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
#include <glibtop/uptime.h>

#include <glibtop_private.h>

static unsigned long _glibtop_sysdeps_uptime =
(1L << GLIBTOP_UPTIME_UPTIME) + (1L << GLIBTOP_UPTIME_IDLETIME) +
(1L << GLIBTOP_UPTIME_BOOT_TIME);

/* Init function. */

void
glibtop_init_uptime_s (glibtop *server)
{
    server->sysdeps.uptime = _glibtop_sysdeps_uptime;
}

/* Provides uptime and idle time. */

void
glibtop_get_uptime_s (glibtop *server, glibtop_uptime *buf)
{
    libgtop_stat_t stat;
    unsigned long total;

    memset (buf, 0, sizeof (glibtop_uptime));

    if (glibtop_get_proc_data_stat_s (server, &stat))
	return;

    total = stat.cpu.user + stat.cpu.nice + stat.cpu.sys + stat.cpu.idle;

    buf->uptime = (double) total / (double) stat.frequency;
    buf->idletime = (double) stat.cpu.idle / (double) stat.frequency;

    buf->boot_time = stat.boot_time;

    buf->flags = _glibtop_sysdeps_uptime;
}
