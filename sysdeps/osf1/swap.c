/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/swap.h>

static unsigned long _glibtop_sysdeps_swap =
(1 << GLIBTOP_SWAP_TOTAL) + (1 << GLIBTOP_SWAP_USED) +
(1 << GLIBTOP_SWAP_FREE);

/* Provides information about swap usage. */

void
glibtop_get_swap__s (glibtop *server, glibtop_swap *buf)
{
	struct tbl_swapinfo swapinfo;
	int i;

	glibtop_init ();

	memset (buf, 0, sizeof (glibtop_swap));

	i = 0;
	
	while (table (TBL_SWAPINFO, i, (char *) &swapinfo, 1,
		      sizeof (struct tbl_swapinfo)) > 0) {
		buf->total += swapinfo.size * getpagesize ();
		buf->free  += swapinfo.free * getpagesize ();
		i++;
	}

	buf->flags = _glibtop_sysdeps_swap;
	
	buf->used  = buf->total - buf->free;
}
