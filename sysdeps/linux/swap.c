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

#include <config.h>
#include <glibtop/error.h>
#include <glibtop/swap.h>

#include <fcntl.h>

static unsigned long _glibtop_sysdeps_swap =
(1L << GLIBTOP_SWAP_TOTAL) + (1L << GLIBTOP_SWAP_USED) +
(1L << GLIBTOP_SWAP_FREE);

static unsigned long _glibtop_sysdeps_swap_paging =
(1L << GLIBTOP_SWAP_PAGEIN) + (1L << GLIBTOP_SWAP_PAGEOUT);

/* Init function. */

int
glibtop_init_swap_s (glibtop *server)
{
    server->sysdeps.swap = _glibtop_sysdeps_swap |
	_glibtop_sysdeps_swap_paging;

    return 0;
}

/* Provides information about swap usage. */

#define MEMINFO		"/proc/meminfo"
#define PROC_STAT	"/proc/stat"

int
glibtop_get_swap_s (glibtop *server, glibtop_swap *buf)
{
    char buffer [BUFSIZ], *p;
    int fd, len;

    glibtop_init_s (&server, GLIBTOP_SYSDEPS_SWAP, 0);

    memset (buf, 0, sizeof (glibtop_swap));

    fd = open (MEMINFO, O_RDONLY);
    if (fd < 0) {
	glibtop_warn_io_r (server, "open (%s)", MEMINFO);
	return -1;
    }

    len = read (fd, buffer, BUFSIZ-1);
    if (len < 0) {
	close (fd);
	glibtop_warn_io_r (server, "read (%s)", MEMINFO);
	return -1;
    }

    close (fd);

    buffer [len] = '\0';

    p = skip_line (buffer);
    p = skip_line (p);
    p = skip_token (p);		/* "Swap:" */

    buf->total  = strtoul (p, &p, 0);
    buf->used   = strtoul (p, &p, 0);
    buf->free   = strtoul (p, &p, 0);

    buf->flags = _glibtop_sysdeps_swap;

    fd = open (PROC_STAT, O_RDONLY);
    if (fd < 0) {
	glibtop_warn_io_r (server, "open (%s)", PROC_STAT);
	return -1;
    }

    len = read (fd, buffer, BUFSIZ-1);
    if (len < 0) {
	close (fd);
	glibtop_warn_io_r (server, "read (%s)", PROC_STAT);
	return -1;
    }

    close (fd);

    buffer [len] = '\0';

    p = strstr (buffer, "\nswap");
    if (p == NULL) return 0;

    p = skip_token (p);

    buf->pagein  = strtoul (p, &p, 0);
    buf->pageout = strtoul (p, &p, 0);

    buf->flags |= _glibtop_sysdeps_swap_paging;

    return 0;
}
