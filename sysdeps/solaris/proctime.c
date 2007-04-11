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
#include <glibtop.h>
#include <glibtop/proctime.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proc_time =
(1L << GLIBTOP_PROC_TIME_START_TIME) + (1L << GLIBTOP_PROC_TIME_RTIME) +
(1L << GLIBTOP_PROC_TIME_UTIME) + (1L << GLIBTOP_PROC_TIME_STIME);

/* Init function. */

void
_glibtop_init_proc_time_s (glibtop *server)
{
	server->sysdeps.proc_time = _glibtop_sysdeps_proc_time;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_time_s (glibtop *server, glibtop_proc_time *buf,
			 pid_t pid)
{
	struct prusage prusage;
        GTimeVal time;

	memset (buf, 0, sizeof (glibtop_proc_time));

	/* Don't do it for scheduler, we don't want to frighten our users */

	if(pid)
	{
		if (glibtop_get_proc_data_usage_s (server, &prusage, pid))
			return;

                g_get_current_time (&time);
		/* prusage.pr_rtime.tv_sec is the during that the process existed */
                buf->start_time = time.tv_sec - prusage.pr_rtime.tv_sec;

		buf->rtime = prusage.pr_rtime.tv_sec * 1E+6 +
			prusage.pr_rtime.tv_nsec / 1E+3;
		buf->utime = prusage.pr_utime.tv_sec * 1E+6 +
			prusage.pr_utime.tv_nsec / 1E+3;
		buf->stime = prusage.pr_stime.tv_sec * 1E+6 +
			prusage.pr_stime.tv_nsec / 1E+3;
	}

	buf->flags = _glibtop_sysdeps_proc_time;
}
