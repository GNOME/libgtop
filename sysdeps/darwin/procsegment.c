/*
   This file is part of LibGTop 2.0.

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
#include <unistd.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procsegment.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_proc_segment = 0;

/* Init function. */

void
_glibtop_init_proc_segment_p (glibtop *server)
{
	server->sysdeps.proc_segment = _glibtop_sysdeps_proc_segment;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_segment_p (glibtop *server, glibtop_proc_segment *buf,
			    pid_t pid)
{
	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_SEGMENT), 0);
	
	memset (buf, 0, sizeof (glibtop_proc_segment));
}
