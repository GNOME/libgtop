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

#include <config.h>
#include <glibtop/error.h>
#include <glibtop/swap.h>

#include <fcntl.h>

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

#define FILENAME	"/proc/meminfo"

void
glibtop_get_swap_s (glibtop *server, glibtop_swap *buf)
{
	char buffer [BUFSIZ+1], *ptr;
	int fd, len;
	FILE *f;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_SWAP, 0);

	memset (buf, 0, sizeof (glibtop_swap));

	buf->flags = _glibtop_sysdeps_swap;

	f = fopen ("/proc/meminfo", "r");
	if (!f) return;

	fscanf (f, "%*[^\n]\n%*[^\n]\nSwap: %Lu %Lu %Lu\n",
		&buf->total, &buf->used, &buf->free);

	fclose (f);

        fd = open ("/proc/stat", O_RDONLY);
        len = read (fd, buffer, BUFSIZ);
        close (fd);

	ptr = strstr (buffer, "\nswap");
	if (ptr == NULL) return;
	
	sscanf (ptr, "\nSwap: %Lu %Lu\n",
		&buf->pagein, &buf->pageout);
}
