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
#include <glibtop/procsegment.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proc_segment =
(1L << GLIBTOP_PROC_SEGMENT_TEXT_RSS) +
(1L << GLIBTOP_PROC_SEGMENT_SHLIB_RSS) +
(1L << GLIBTOP_PROC_SEGMENT_DATA_RSS) +
(1L << GLIBTOP_PROC_SEGMENT_STACK_RSS) +
(1L << GLIBTOP_PROC_SEGMENT_DIRTY_SIZE);

static const unsigned long _glibtop_sysdeps_proc_segment_state =
(1L << GLIBTOP_PROC_SEGMENT_START_CODE) +
(1L << GLIBTOP_PROC_SEGMENT_END_CODE) +
(1L << GLIBTOP_PROC_SEGMENT_START_STACK);

#ifndef LOG1024
#define LOG1024		10
#endif

/* these are for getting the memory statistics */
static int pageshift;		/* log base 2 of the pagesize */

/* define pagetok in terms of pageshift */
#define pagetok(size) ((size) << pageshift)

/* Init function. */

int
glibtop_init_proc_segment_s (glibtop *server)
{
    register int pagesize;

    server->sysdeps.proc_segment = _glibtop_sysdeps_proc_segment |
	_glibtop_sysdeps_proc_segment_state;

    /* get the page size with "getpagesize" and calculate pageshift. */
    pagesize = getpagesize ();
    pageshift = 0;
    while (pagesize > 1) {
	pageshift++;
	pagesize >>= 1;
    }

    return 0;
}

/* Provides detailed information about a process. */

int
glibtop_get_proc_segment_s (glibtop *server, glibtop_proc_segment *buf,
			    pid_t pid)
{
    libgtop_proc_mem_t proc_mem;
    libgtop_proc_state_t proc_state;
    int retval;

    memset (buf, 0, sizeof (glibtop_proc_segment));

    retval = glibtop_get_proc_data_proc_mem_s (server, &proc_mem, pid);
    if (retval)
	return retval;

    buf->text_rss = proc_mem.trs;
    buf->shlib_rss = proc_mem.lrs;
    buf->data_rss = proc_mem.drs;
    buf->stack_rss = proc_mem.segment.stack;
    buf->dirty_size = proc_mem.dt;

    buf->text_rss   <<= pageshift;
    buf->shlib_rss  <<= pageshift;
    buf->data_rss   <<= pageshift;
    buf->stack_rss  <<= pageshift;
    buf->dirty_size <<= pageshift;

    buf->flags = _glibtop_sysdeps_proc_segment;

    retval = glibtop_get_proc_data_proc_state_s (server, &proc_state, pid);
    if (retval)
	return retval;

    buf->start_code = proc_state.start_code;
    buf->end_code = proc_state.end_code;
    buf->start_data = proc_state.start_data;
    buf->end_data = proc_state.end_data;
    buf->start_brk = proc_state.start_brk;
    buf->end_brk = proc_state.brk;

    buf->start_stack = proc_state.start_stack;
    buf->start_mmap = proc_state.start_mmap;

    buf->arg_start = proc_state.arg_start;
    buf->arg_end = proc_state.arg_end;
    buf->env_start = proc_state.env_start;
    buf->env_end = proc_state.env_end;

    buf->flags |= _glibtop_sysdeps_proc_segment_state;

    return 0;
}
