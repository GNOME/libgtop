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
#include <glibtop/swap.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_swap =
(1 << GLIBTOP_SWAP_TOTAL) + (1 << GLIBTOP_SWAP_USED) +
(1 << GLIBTOP_SWAP_FREE);

static const unsigned long _glibtop_sysdeps_swap_stat =
(1 << GLIBTOP_SWAP_PAGEIN) + (1 << GLIBTOP_SWAP_PAGEOUT);

/* Init function. */

void
glibtop_init_swap_s (glibtop *server)
{
    server->sysdeps.swap = _glibtop_sysdeps_swap |
	_glibtop_sysdeps_swap_stat;
}

/* Provides information about swap usage. */

void
glibtop_get_swap_s (glibtop *server, glibtop_swap *buf)
{
    libgtop_stat_t stat;
    libgtop_swap_t swap;

    memset (buf, 0, sizeof (glibtop_swap));

    if (glibtop_get_proc_data_swap_s (server, &swap))
	return;

    buf->total = swap.totalswap;
    buf->free = swap.freeswap;
    buf->used = swap.totalswap - swap.freeswap;

    buf->flags = _glibtop_sysdeps_swap;

    if (glibtop_get_proc_data_stat_s (server, &stat))
	return;

    buf->pagein = stat.pswpin;
    buf->pageout = stat.pswpout;

    buf->flags |= _glibtop_sysdeps_swap_stat;
}
