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
(1 << GLIBTOP_PROC_SEGMENT_TEXT_RSS) +
(1 << GLIBTOP_PROC_SEGMENT_SHLIB_RSS) +
(1 << GLIBTOP_PROC_SEGMENT_DATA_RSS) +
(1 << GLIBTOP_PROC_SEGMENT_STACK_RSS) +
(1 << GLIBTOP_PROC_SEGMENT_DIRTY_SIZE);

static const unsigned long _glibtop_sysdeps_proc_segment_state =
(1 << GLIBTOP_PROC_SEGMENT_START_CODE) +
(1 << GLIBTOP_PROC_SEGMENT_END_CODE) +
(1 << GLIBTOP_PROC_SEGMENT_START_STACK);

/* Init function. */

void
glibtop_init_proc_segment_s (glibtop *server)
{
    server->sysdeps.proc_segment = _glibtop_sysdeps_proc_segment |
	_glibtop_sysdeps_proc_segment_state;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_segment_s (glibtop *server, glibtop_proc_segment *buf,
			    pid_t pid)
{
    libgtop_proc_mem_t proc_mem;
    libgtop_proc_state_t proc_state;

    memset (buf, 0, sizeof (glibtop_proc_segment));

    if (glibtop_get_proc_data_proc_mem_s (server, &proc_mem, pid))
	return;

    buf->text_rss = proc_mem.trs;
    buf->shlib_rss = proc_mem.lrs;
    buf->data_rss = proc_mem.drs;
    buf->stack_rss = proc_mem.segment.stack;
    buf->dirty_size = proc_mem.dt;

    buf->flags = _glibtop_sysdeps_proc_segment;

    if (glibtop_get_proc_data_proc_state_s (server, &proc_state, pid))
	return;

    buf->start_code = proc_state.start_code;
    buf->end_code = proc_state.end_code;
    buf->start_stack = proc_state.start_stack;

    buf->flags |= _glibtop_sysdeps_proc_segment_state;
}
