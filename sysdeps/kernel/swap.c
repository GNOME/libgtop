/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   LibGTop is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
   License for more details.

   You should have received a copy of the GNU Library General Public
   License along with LibGTop; see the file COPYING.LIB.  If not, write
   to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <config.h>
#include <glibtop/error.h>
#include <glibtop/swap.h>

#include "kernel.h"

static unsigned long _glibtop_sysdeps_swap =
(1 << GLIBTOP_SWAP_TOTAL) + (1 << GLIBTOP_SWAP_USED) +
(1 << GLIBTOP_SWAP_FREE) + (1 << GLIBTOP_SWAP_PAGEIN) +
(1 << GLIBTOP_SWAP_PAGEOUT);

/* Init function. */

void
glibtop_init_swap_s (glibtop *server)
{
	server->sysdeps.swap = _glibtop_sysdeps_swap;
}

/* Provides information about swap usage. */

void
glibtop_get_swap_s (glibtop *server, glibtop_swap *buf)
{
	union table tbl;
	
	glibtop_init_s (&server, GLIBTOP_SYSDEPS_SWAP, 0);

	memset (buf, 0, sizeof (glibtop_swap));

	if (table (TABLE_SWAP, &tbl, NULL))
		glibtop_error_io_r (server, "table(TABLE_SWAP)");

	buf->flags = _glibtop_sysdeps_swap;

	buf->total = tbl.swap.total;
	buf->used  = tbl.swap.used;
	buf->free  = tbl.swap.free;

	buf->pagein  = tbl.swap.pagein;
	buf->pageout = tbl.swap.pageout;
}
