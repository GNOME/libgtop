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
#include <glibtop/procsegment.h>

#include "kernel.h"

static const unsigned long _glibtop_sysdeps_proc_segment =
(1 << GLIBTOP_PROC_SEGMENT_TRS) + (1 << GLIBTOP_PROC_SEGMENT_LRS) +
(1 << GLIBTOP_PROC_SEGMENT_DRS) + (1 << GLIBTOP_PROC_SEGMENT_DT) +
(1 << GLIBTOP_PROC_SEGMENT_START_CODE) + (1 << GLIBTOP_PROC_SEGMENT_END_CODE) +
(1 << GLIBTOP_PROC_SEGMENT_START_STACK);

/* Init function. */

void
glibtop_init_proc_segment_s (glibtop *server)
{
	server->sysdeps.proc_segment = _glibtop_sysdeps_proc_segment;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_segment_s (glibtop *server, glibtop_proc_segment *buf,
			    pid_t pid)
{
	union table tbl;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_SEGMENT, 0);

	memset (buf, 0, sizeof (glibtop_proc_segment));

	if (table (TABLE_PROC_MEM, &tbl, &pid))
		glibtop_error_io_r (server, "table(TABLE_PROC_MEM)");
	
	buf->start_code = tbl.proc_mem.start_code;
	buf->end_code = tbl.proc_mem.end_code;
	buf->start_stack = tbl.proc_mem.start_stack;

	if (table (TABLE_PROC_SEGMENT, &tbl, &pid))
		glibtop_error_io_r (server, "table(TABLE_PROC_SEGMENT)");

	buf->flags = _glibtop_sysdeps_proc_segment;

	buf->trs = tbl.proc_segment.trs;
	buf->lrs = tbl.proc_segment.lrs;
	buf->drs = tbl.proc_segment.drs;
	buf->dt = tbl.proc_segment.dt;
}
