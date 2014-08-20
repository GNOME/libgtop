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
#include <glibtop/proctime.h>

#include <glibtop_suid.h>

#include <sys/user.h>
#include <sys/time.h>

static const unsigned long _glibtop_sysdeps_proc_time =
(1L << GLIBTOP_PROC_TIME_UTIME) + (1L << GLIBTOP_PROC_TIME_CUTIME) +
(1L << GLIBTOP_PROC_TIME_STIME) + (1L << GLIBTOP_PROC_TIME_CSTIME) +
(1L << GLIBTOP_PROC_TIME_START_TIME);

/* Init function. */

void
_glibtop_init_proc_time_p (glibtop *server)
{
	server->sysdeps.proc_time = _glibtop_sysdeps_proc_time;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_time_p (glibtop *server, glibtop_proc_time *buf,
			 pid_t pid)
{
	struct user u;
	int ret;

	glibtop_init_p (server, GLIBTOP_SYSDEPS_PROC_TIME, 0);

	memset (buf, 0, sizeof (glibtop_proc_time));

	/* !!! THE FOLLOWING CODE RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

	glibtop_suid_enter (server);

	ret = table (TBL_UAREA, pid, (char *) &u, 1,
		     sizeof (struct user));

	glibtop_suid_leave (server);

	/* !!! END OF SUID ROOT PART !!! */

	if (ret != 1) return;

	buf->start_time = u.u_start.tv_sec;

	buf->utime = u.u_ru.ru_utime.tv_sec;
	buf->stime = u.u_ru.ru_stime.tv_sec;
	buf->cutime = u.u_cru.ru_utime.tv_sec;
	buf->cstime = u.u_cru.ru_stime.tv_sec;

	buf->flags = _glibtop_sysdeps_proc_time;
}
