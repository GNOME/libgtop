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
#include <glibtop/procmem.h>

#include "kernel.h"

static const unsigned long _glibtop_sysdeps_proc_mem =
(1 << GLIBTOP_PROC_MEM_SIZE) + (1 << GLIBTOP_PROC_MEM_VSIZE) +
(1 << GLIBTOP_PROC_MEM_RESIDENT) + (1 << GLIBTOP_PROC_MEM_SHARE) +
(1 << GLIBTOP_PROC_MEM_RSS) + (1 << GLIBTOP_PROC_MEM_RSS_RLIM);

/* Init function. */

void
glibtop_init_proc_mem_s (glibtop *server)
{
	server->sysdeps.proc_mem = _glibtop_sysdeps_proc_mem;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_mem_s (glibtop *server, glibtop_proc_mem *buf,
			pid_t pid)
{
	union table tbl;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_MEM, 0);

	memset (buf, 0, sizeof (glibtop_proc_mem));

	if (table (TABLE_PROC_MEM, &tbl, &pid))
		glibtop_error_io_r (server, "table(TABLE_PROC_MEM)");
	
	buf->rss = tbl.proc_mem.rss;
	buf->rss_rlim = tbl.proc_mem.rlim;

	if (table (TABLE_PROC_SEGMENT, &tbl, &pid))
		glibtop_error_io_r (server, "table(TABLE_PROC_SEGMENT)");
	
	buf->flags = _glibtop_sysdeps_proc_mem;

	buf->vsize = tbl.proc_segment.vsize;
	buf->size = tbl.proc_segment.size;
	buf->resident = tbl.proc_segment.resident;
	buf->share = tbl.proc_segment.shared;
}
