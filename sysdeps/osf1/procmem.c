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

#include <glibtop_suid.h>

#include <sys/user.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <mach.h>
#include <mach/mach_types.h>
#include <mach/task_info.h>

static const unsigned long _glibtop_sysdeps_proc_mem =
(1L << GLIBTOP_PROC_MEM_VSIZE) + (1L << GLIBTOP_PROC_MEM_RESIDENT) +
(1L << GLIBTOP_PROC_MEM_RSS);

/* Init function. */

void
_glibtop_init_proc_mem_p (glibtop *server)
{
	server->sysdeps.proc_mem = _glibtop_sysdeps_proc_mem;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_mem_p (glibtop *server, glibtop_proc_mem *buf,
			pid_t pid)
{
	task_basic_info_data_t taskinfo;
	int ret, info_count;
	task_t thistask;
	struct user u;

	glibtop_init_p (server, GLIBTOP_SYSDEPS_PROC_MEM, 0);

	memset (buf, 0, sizeof (glibtop_proc_mem));

	/* Get task structure. */

	ret = task_by_unix_pid (task_self(), pid, &thistask);

	if (ret != KERN_SUCCESS) return;

	/* Get taskinfo about this task. */

	info_count = TASK_BASIC_INFO_COUNT;

	ret = task_info (thistask, TASK_BASIC_INFO,
			 (task_info_t) &taskinfo, &info_count);

	if (ret != KERN_SUCCESS) return;

	buf->resident = taskinfo.resident_size;
	buf->rss = taskinfo.resident_size;
	buf->vsize = taskinfo.virtual_size;

	/* !!! THE FOLLOWING CODE RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

	glibtop_suid_enter (server);

	ret = table (TBL_UAREA, pid, (char *) &u, 1,
		     sizeof (struct user));

	glibtop_suid_leave (server);

	/* !!! END OF SUID ROOT PART !!! */

	if (ret != 1) return;

	buf->rss_rlim = u.u_rlimit [RLIMIT_RSS].rlim_cur;

	buf->share = u.u_ru.ru_ixrss;

	buf->flags |= (1L << GLIBTOP_PROC_MEM_RSS_RLIM) |
		(1L << GLIBTOP_PROC_MEM_SHARE);
}
