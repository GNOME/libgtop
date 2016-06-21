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
#include <glibtop/procsegment.h>

#include "glibtop_private.h"

static const unsigned long _glibtop_sysdeps_proc_segment =
(1L << GLIBTOP_PROC_SEGMENT_START_CODE) +
(1L << GLIBTOP_PROC_SEGMENT_END_CODE) +
(1L << GLIBTOP_PROC_SEGMENT_START_STACK);

static const unsigned long _glibtop_sysdeps_proc_segment_statm =
(1L << GLIBTOP_PROC_SEGMENT_TEXT_RSS) +
/* Disabled due to bug in the Linux Kernel. */
/* (1L << GLIBTOP_PROC_SEGMENT_SHLIB_RSS) + */
(1L << GLIBTOP_PROC_SEGMENT_DATA_RSS) +
(1L << GLIBTOP_PROC_SEGMENT_DIRTY_SIZE);

/* Init function. */

void
_glibtop_init_proc_segment_s (glibtop *server)
{
	server->sysdeps.proc_segment = _glibtop_sysdeps_proc_segment |
	  _glibtop_sysdeps_proc_segment_statm;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_segment_s (glibtop *server, glibtop_proc_segment *buf,
			    pid_t pid)
{
	char buffer [BUFSIZ], *p;
	const size_t pagesize = getpagesize();

	memset (buf, 0, sizeof (glibtop_proc_segment));

	if (proc_stat_to_buffer(buffer, sizeof buffer, pid))
		return;

	p = proc_stat_after_cmd (buffer);
	if (!p) return;

	p = skip_multiple_token (p, 23);

	buf->start_code  = strtoull (p, &p, 0);
	buf->end_code    = strtoull (p, &p, 0);
	buf->start_stack = strtoull (p, &p, 0);

	buf->flags = _glibtop_sysdeps_proc_segment;

	if (proc_statm_to_buffer(buffer, sizeof buffer, pid))
		return;

	p = skip_multiple_token (buffer, 3);

	/* This doesn't work very well due to a bug in the Linux kernel.
	 * I'll submit a patch to the kernel mailing list soon. */

	buf->text_rss   = strtoull (p, &p, 0);
	buf->shlib_rss  = strtoull (p, &p, 0);
	buf->data_rss   = strtoull (p, &p, 0);
	buf->dirty_size = strtoull (p, &p, 0);

	buf->text_rss   *= pagesize;
	buf->shlib_rss  *= pagesize;
	buf->data_rss   *= pagesize;
	buf->dirty_size *= pagesize;

	buf->flags |= _glibtop_sysdeps_proc_segment_statm;
}
