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

#include <glibtop.h>
#include <glibtop/procmem.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proc_mem =
(1L << GLIBTOP_PROC_MEM_VSIZE) + (1L << GLIBTOP_PROC_MEM_SIZE) +
(1L << GLIBTOP_PROC_MEM_RESIDENT) + (1L << GLIBTOP_PROC_MEM_SHARE) +
(1L << GLIBTOP_PROC_MEM_RSS) + (1L << GLIBTOP_PROC_MEM_RSS_RLIM);

#ifndef LOG1024
#define LOG1024		10
#endif

/* these are for getting the memory statistics */
static int pageshift;		/* log base 2 of the pagesize */

/* define pagetok in terms of pageshift */
#define pagetok(size) ((size) << pageshift)

/* Init function. */

int
glibtop_init_proc_mem_s (glibtop *server)
{
    register int pagesize;

    server->sysdeps.proc_mem = _glibtop_sysdeps_proc_mem;

    /* get the page size with "getpagesize" and calculate pageshift. */
    pagesize = getpagesize ();
    pageshift = 0;
    while (pagesize > 1) {
	pageshift++;
	pagesize >>= 1;
    }
}

/* Provides detailed information about a process. */

int
glibtop_get_proc_mem_s (glibtop *server, glibtop_proc_mem *buf,
			pid_t pid)
{
    libgtop_proc_mem_t proc_mem;

    memset (buf, 0, sizeof (glibtop_proc_mem));

    if (glibtop_get_proc_data_proc_mem_s (server, &proc_mem, pid))
	return;

    buf->vsize = proc_mem.segment.vsize;

    buf->size = proc_mem.size;
    buf->resident = proc_mem.resident;
    buf->share = proc_mem.share;

    buf->rss = proc_mem.rss;
    buf->rss_rlim = proc_mem.rlim;
    
    buf->vsize <<= pageshift;

    buf->size <<= pageshift;
    buf->resident <<= pageshift;
    buf->share <<= pageshift;

    buf->flags = _glibtop_sysdeps_proc_mem;
}
