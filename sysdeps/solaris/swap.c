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
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/swap.h>

#include <unistd.h>
#include <sys/swap.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_swap =
(1L << GLIBTOP_SWAP_TOTAL) + (1L << GLIBTOP_SWAP_USED) +
(1L << GLIBTOP_SWAP_FREE);

/* Init function. */

void
_glibtop_init_swap_s (glibtop *server)
{
    server->sysdeps.swap = _glibtop_sysdeps_swap;
}

/* Provides information about swap usage. */

void
glibtop_get_swap_s (glibtop *server, glibtop_swap *buf)
{
    swaptbl_t *s = NULL;
    int i, n1, n2;

    /* we don't care about ste_path, and we're lazy */
    char shared_path[BUFSIZ]; /* hope this is large enough */

    const int pagesize = getpagesize();

    memset(buf, 0, sizeof *buf);

    switch( (n1 = swapctl(SC_GETNSWP, NULL)) )
    {
    case -1:
	glibtop_warn_r(server, "swapctl: GETNSWP");
	return;

    case 0:
	/* no swapfile */
	goto out_no_swap;

    default:
	break;
    }

    /* RTFM */
    s = g_malloc(sizeof(swaptbl_t)
		 + n1 * sizeof(swapent_t));

    s->swt_n = n1;

    /* initialize string pointers */
    for (i = 0; i < n1; i++)
    {
	s->swt_ent[i].ste_path = shared_path;
    }

    if ((n2 = swapctl(SC_LIST, s)) < 0)
    {
	glibtop_warn_r(server, "swapctl: SC_LIST");
	goto out_free;
    }


    /* #swapfile may have changed between the 2 swapctl() calls
     * we don't care, we just use the smallest #swapfile */
    for (i = 0; i < MIN(n1, n2); i++)
    {
	buf->total += s->swt_ent[i].ste_pages;
	buf->free  += s->swt_ent[i].ste_free;
    }

    buf->total *= pagesize;
    buf->free  *= pagesize;
    buf->used   = buf->total - buf->free;

 out_no_swap:
    buf->flags  = _glibtop_sysdeps_swap;

 out_free:
    g_free(s);
}
