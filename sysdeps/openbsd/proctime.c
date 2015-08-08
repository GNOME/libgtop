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
#include <glibtop/proctime.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_proc_time =
(1L << GLIBTOP_PROC_TIME_RTIME) + (1L << GLIBTOP_PROC_TIME_FREQUENCY);

static const unsigned long _glibtop_sysdeps_proc_time_user =
(1L << GLIBTOP_PROC_TIME_UTIME) + (1L << GLIBTOP_PROC_TIME_STIME) +
(1L << GLIBTOP_PROC_TIME_CUTIME) + (1L << GLIBTOP_PROC_TIME_CSTIME) +
(1L << GLIBTOP_PROC_TIME_START_TIME);

#define tv2sec(tv)	(((guint64) tv.tv_sec * 1000000) + (guint64) tv.tv_usec)

static unsigned int clockrate;
static int mib [] = { CTL_KERN, KERN_CLOCKRATE };

/* Init function. */

void
_glibtop_init_proc_time_p (glibtop *server)
{
	struct clockinfo ci;
	size_t length;
	length = sizeof (ci);
	if (sysctl (mib, 2, &ci, &length, NULL, 0) == 0)
		clockrate = ci.hz;
	if (!clockrate)
		clockrate = 1; /* XXX avoid div by 0 later */

	server->sysdeps.proc_time = _glibtop_sysdeps_proc_time |
		_glibtop_sysdeps_proc_time_user;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_time_p (glibtop *server, glibtop_proc_time *buf,
			 pid_t pid)
{
	struct kinfo_proc *pinfo;
	int count;

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_PROC_TIME), 0);

	memset (buf, 0, sizeof (glibtop_proc_time));

	/* It does not work for the swapper task. */
	if (pid == 0) return;


	/* Get the process information */
	pinfo = kvm_getprocs (server->machine->kd, KERN_PROC_PID, pid,
			      sizeof (*pinfo), &count);
	if (pinfo == NULL) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		return;
	}

	buf->rtime = pinfo[0].p_rtime_sec * clockrate 
		+ pinfo[0].p_rtime_usec  * clockrate / 1000000;
	buf->frequency = clockrate;

	buf->flags = _glibtop_sysdeps_proc_time;

	buf->utime = pinfo[0].p_uutime_sec * 1000000
		+ pinfo[0].p_uutime_usec;
	buf->stime = pinfo[0].p_ustime_sec * 1000000
		+ pinfo[0].p_ustime_usec;
	buf->cutime = pinfo[0].p_uctime_sec * 1000000
		+ pinfo[0].p_uctime_usec; /* XXX is u+s */
	buf->cstime = 0; /* XXX */
	buf->start_time = pinfo[0].p_ustart_sec;

	buf->flags |= _glibtop_sysdeps_proc_time_user;
}

