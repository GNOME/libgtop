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
#include <glibtop/procsegment.h>

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

#ifndef LOG1024
#define LOG1024		10
#endif

/* these are for getting the memory statistics */
static int pageshift;		/* log base 2 of the pagesize */

/* define pagetok in terms of pageshift */
#define pagetok(size) ((size) << pageshift)

/* Init function. */

void
glibtop_init_proc_segment_s (glibtop *server)
{
	register int pagesize;

	server->sysdeps.proc_segment = _glibtop_sysdeps_proc_segment |
	  _glibtop_sysdeps_proc_segment_statm;

	/* get the page size with "getpagesize" and calculate pageshift
	 * from it */
	pagesize = getpagesize ();
	pageshift = 0;
	while (pagesize > 1) {
		pageshift++;
		pagesize >>= 1;
	}
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_segment_s (glibtop *server, glibtop_proc_segment *buf,
			    pid_t pid)
{
	char buffer [BUFSIZ], *p;
	
	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_SEGMENT, 0);

	memset (buf, 0, sizeof (glibtop_proc_segment));

	if (proc_stat_to_buffer (buffer, pid))
		return;

	p = proc_stat_after_cmd (buffer);
	if (!p) return;

	p = skip_multiple_token (p, 23);

	buf->start_code  = strtoull (p, &p, 0);
	buf->end_code    = strtoull (p, &p, 0);
	buf->start_stack = strtoull (p, &p, 0);

	buf->flags = _glibtop_sysdeps_proc_segment;

	if (proc_statm_to_buffer (buffer, pid))
		return;

	p = skip_multiple_token (buffer, 3);

	/* This doesn't work very well due to a bug in the Linux kernel.
	 * I'll submit a patch to the kernel mailing list soon. */

	buf->text_rss   = strtoull (p, &p, 0);
	buf->shlib_rss  = strtoull (p, &p, 0);
	buf->data_rss   = strtoull (p, &p, 0);
	buf->dirty_size = strtoull (p, &p, 0);

	buf->text_rss   <<= pageshift;
	buf->shlib_rss  <<= pageshift;
	buf->data_rss   <<= pageshift;
	buf->dirty_size <<= pageshift;

	buf->flags |= _glibtop_sysdeps_proc_segment_statm;
}
