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
#include <glibtop/procsegment.h>

#include <glibtop_suid.h>

#include <sys/user.h>

static const unsigned long _glibtop_sysdeps_proc_segment =
(1 << GLIBTOP_PROC_SEGMENT_TRS) + (1 << GLIBTOP_PROC_SEGMENT_DRS) +
(1 << GLIBTOP_PROC_SEGMENT_START_CODE) + (1 << GLIBTOP_PROC_SEGMENT_END_CODE) +
(1 << GLIBTOP_PROC_SEGMENT_START_STACK);

/* Init function. */

void
glibtop_init_proc_segment_p (glibtop *server)
{
	server->sysdeps.proc_segment = _glibtop_sysdeps_proc_segment;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_segment_p (glibtop *server, glibtop_proc_segment *buf,
			    pid_t pid)
{
	int ret;
	struct user u;

	glibtop_init_p (server, GLIBTOP_SYSDEPS_PROC_SEGMENT, 0);
	
	memset (buf, 0, sizeof (glibtop_proc_segment));

	/* !!! THE FOLLOWING CODE RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

	glibtop_suid_enter (server);
	
	ret = table (TBL_UAREA, pid, (char *) &u, 1,
		     sizeof (struct user));

	glibtop_suid_leave (server);
		     
	/* !!! END OF SUID ROOT PART !!! */
	
	if (ret != 1) return;
	
	buf->start_code = (unsigned long) u.u_text_start;
	buf->end_code = (unsigned long) u.u_data_start;
	buf->start_stack = (unsigned long) u.u_stack_start;
	
	buf->trs = u.u_tsize;
	buf->drs = u.u_dsize;
	
	buf->flags = _glibtop_sysdeps_proc_segment;
}
