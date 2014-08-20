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
#include <unistd.h>
#include <sys/vminfo.h>
#include <sys/resource.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procmem.h>

#include "utils.h"

static const unsigned long _glibtop_sysdeps_proc_mem =
(1 << GLIBTOP_PROC_MEM_SIZE) + (1 << GLIBTOP_PROC_MEM_VSIZE) +
(1 << GLIBTOP_PROC_MEM_RESIDENT) + (1 << GLIBTOP_PROC_MEM_RSS) +
(1 << GLIBTOP_PROC_MEM_RSS_RLIM);

/* Init function. */

void
_glibtop_init_proc_mem_s (glibtop *server)
{
	server->sysdeps.proc_mem = _glibtop_sysdeps_proc_mem;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_mem_s (glibtop *server, glibtop_proc_mem *buf,
			pid_t pid)
{
	struct procsinfo *pinfo;
	int pagesize;

	glibtop_init_s (&server, (1L << GLIBTOP_SYSDEPS_PROC_MEM), 0);

	memset (buf, 0, sizeof (glibtop_proc_mem));

	pinfo = _glibtop_get_procinfo(server, pid);
	if (pinfo == NULL) return;

	pagesize = sysconf(_SC_PAGESIZE);

	buf->vsize = buf->size = pinfo->pi_size * pagesize;
	buf->resident = buf->rss = (pinfo->pi_drss + pinfo->pi_trss) * pagesize;
	buf->rss_rlim = pinfo->pi_rlimit[RLIMIT_RSS].rlim_cur;

	buf->flags = _glibtop_sysdeps_proc_mem;
}
