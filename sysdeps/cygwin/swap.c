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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <config.h>
#include <glibtop/error.h>
#include <glibtop/swap.h>

#include "glibtop_private.h"

#include <fcntl.h>

static const unsigned long _glibtop_sysdeps_swap =
(1L << GLIBTOP_SWAP_TOTAL) + (1L << GLIBTOP_SWAP_USED) +
(1L << GLIBTOP_SWAP_FREE);

static const unsigned long _glibtop_sysdeps_swap_paging =
(1L << GLIBTOP_SWAP_PAGEIN) + (1L << GLIBTOP_SWAP_PAGEOUT);

/* Init function. */

void
_glibtop_init_swap_s (glibtop *server)
{
	server->sysdeps.swap = _glibtop_sysdeps_swap |
		_glibtop_sysdeps_swap_paging;
}

/* Provides information about swap usage. */

#define MEMINFO		"/proc/meminfo"
#define PROC_STAT	"/proc/stat"

void
glibtop_get_swap_s (glibtop *server, glibtop_swap *buf)
{
	char buffer [BUFSIZ], *p;

	memset (buf, 0, sizeof (glibtop_swap));

	file_to_buffer(server, buffer, sizeof buffer, MEMINFO);

	/* Kernel 2.6 with multiple lines */

	buf->total = get_scaled(buffer, "SwapTotal:");
	buf->free = get_scaled(buffer, "SwapFree:");
	buf->used = buf->total - buf->free;

	buf->flags = _glibtop_sysdeps_swap;

	file_to_buffer (server, buffer, sizeof buffer, PROC_STAT);

	p = strstr (buffer, "\nswap");

	if(p)
	{
		p = skip_token (p);

		buf->pagein  = strtoull (p, &p, 0);
		buf->pageout = strtoull (p, &p, 0);

		buf->flags |= _glibtop_sysdeps_swap_paging;
	}
}
