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
#include <unistd.h>
#include <sys/vminfo.h>
#include <sys/resource.h>

#include <glibtop.h>
#include <glibtop/procsegment.h>
#include <glibtop/error.h>

#include "utils.h"

static const unsigned long _glibtop_sysdeps_proc_segment =
(1 << GLIBTOP_PROC_SEGMENT_TEXT_RSS) + (1 << GLIBTOP_PROC_SEGMENT_SHLIB_RSS) +
(1 << GLIBTOP_PROC_SEGMENT_DATA_RSS) + (1 << GLIBTOP_PROC_SEGMENT_STACK_RSS);


/* Init function. */

void
_glibtop_init_proc_segment_s (glibtop *server)
{
	server->sysdeps.proc_segment = _glibtop_sysdeps_proc_segment;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_segment_s (glibtop *server, glibtop_proc_segment *buf,
			    pid_t pid)
{
	struct procsinfo *pinfo;
	int pagesize;

	glibtop_init_s (&server, (1L << GLIBTOP_SYSDEPS_PROC_SEGMENT), 0);

	memset (buf, 0, sizeof (glibtop_proc_segment));

	pinfo = _glibtop_get_procinfo(server, pid);
	if (pinfo == NULL) return;

	pagesize = sysconf(_SC_PAGESIZE);

	buf->text_rss = pinfo->pi_trss * pagesize;
	buf->data_rss = pinfo->pi_drss * pagesize;
	buf->shlib_rss = pinfo->pi_ru.ru_ixrss * pagesize;
	buf->stack_rss = pinfo->pi_ru.ru_isrss * pagesize;

	buf->flags = _glibtop_sysdeps_proc_segment;
}
