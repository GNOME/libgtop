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
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procmem.h>

#include "glibtop_private.h"

static const unsigned long _glibtop_sysdeps_proc_mem =
(1L << GLIBTOP_PROC_MEM_SIZE) + (1L << GLIBTOP_PROC_MEM_RESIDENT) +
(1L << GLIBTOP_PROC_MEM_SHARE);

static const unsigned long _glibtop_sysdeps_proc_mem_pss =
(1L << GLIBTOP_PROC_MEM_RSS);


static unsigned long
G_GNUC_UNUSED get_pss(glibtop* server, pid_t pid)
{
	char filepath[128];
	FILE* smaps;
	char* line = NULL;
	size_t line_size = 0;
	unsigned long pss = 0;

	snprintf(filepath, sizeof filepath, "/proc/%u/smaps", (unsigned)pid);

	if (!(smaps = fopen(filepath, "r"))) {
		glibtop_error_io_r(server, "Cannot open %s", filepath);
		goto out;
	}

	while (getline(&line, &line_size, smaps) != -1) {
		if (strncmp(line, "Pss:", 4))
			continue;
		pss += get_scaled(line + 4, NULL);
	}

out:
	if (smaps)
		fclose(smaps);

	free(line);

	return pss;
}


/* Init function. */

void
_glibtop_init_proc_mem_s (glibtop *server)
{
	server->sysdeps.proc_mem = _glibtop_sysdeps_proc_mem;
	if (server->os_version_code >= LINUX_VERSION_CODE(2, 6, 25))
		server->sysdeps.proc_mem |= _glibtop_sysdeps_proc_mem_pss;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_mem_s (glibtop *server, glibtop_proc_mem *buf, pid_t pid)
{
	char buffer [BUFSIZ], *p;
	const size_t pagesize = getpagesize();

	memset (buf, 0, sizeof (glibtop_proc_mem));

	/* As of 2.6.24 in fs/proc/ *.c

	   == rss vs. resident ==

	   stat/rss:
	   get_mm_rss where
	   #define get_mm_rss(mm)					\
		(get_mm_counter(mm, file_rss) + get_mm_counter(mm, anon_rss))

	   statm/resident:
	   *shared = get_mm_counter(mm, file_rss);
	   *resident = *shared + get_mm_counter(mm, anon_rss);

	   == vsize vs. size ==

	   stat/vsize:
	   task_vsize(mm) ... total_vm * pagesize

	   statm/size
	   mm->total_vm

	   =================
	   rss == resident
	   vsize == size
	   rss_lim is not implemented in statm, but there's limits which
		provides all limits
	   share is only implemented in statm
	*/

	if (proc_statm_to_buffer(buffer, sizeof buffer, pid))
		return;

	buf->size     = strtoull (buffer, &p, 0);
	buf->resident = strtoull (p, &p, 0);
	buf->share    = strtoull (p, &p, 0);

	buf->size     *= pagesize;
	buf->resident *= pagesize;
	buf->share    *= pagesize;

	/* dummy values */
	buf->vsize = buf->size;
	buf->rss_rlim = ~0;

	buf->flags |= _glibtop_sysdeps_proc_mem;
     
#if 0 
	/* FIXME: see previous comment */
	if (server->os_version_code >= LINUX_VERSION_CODE(2, 6, 25)) {
		buf->rss = get_pss(server, pid);
		buf->flags |= _glibtop_sysdeps_proc_mem_pss;
	}
#else
	buf->rss = buf->resident;
#endif
}
