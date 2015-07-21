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

#include <sys/time.h>
#include <osreldate.h>

static const unsigned long _glibtop_sysdeps_proc_time =
(1L << GLIBTOP_PROC_TIME_RTIME) + (1L << GLIBTOP_PROC_TIME_FREQUENCY);

static const unsigned long _glibtop_sysdeps_proc_time_user =
(1L << GLIBTOP_PROC_TIME_UTIME) + (1L << GLIBTOP_PROC_TIME_STIME) +
(1L << GLIBTOP_PROC_TIME_CUTIME) + (1L << GLIBTOP_PROC_TIME_CSTIME) +
(1L << GLIBTOP_PROC_TIME_START_TIME);

static guint64 tv2sec_freq(struct timeval tv, guint64 freq)
{
	return tv.tv_sec * freq + tv.tv_usec * freq / 1000000;
}

#define tv2sec(tv)	tv2sec_freq(tv, 1)

/* Init function. */

void
_glibtop_init_proc_time_p (glibtop *server)
{
	server->sysdeps.proc_time = _glibtop_sysdeps_proc_time |
		_glibtop_sysdeps_proc_time_user;
}

/* Taken from /usr/src/sys/kern/kern_resource.c */

/* Provides detailed information about a process. */

void
glibtop_get_proc_time_p (glibtop *server, glibtop_proc_time *buf,
			 pid_t pid)
{
	struct kinfo_proc *pinfo;
	struct clockinfo ci;
	int count;
	size_t len;

	memset (buf, 0, sizeof (glibtop_proc_time));

	/* It does not work for the swapper task. */
	if (pid == 0) return;

	glibtop_suid_enter (server);

	/* Get the process information */
	pinfo = kvm_getprocs (server->machine->kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1)) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		glibtop_suid_leave (server);
		return;
	}

	glibtop_suid_leave (server);


	len = sizeof (ci);
	if (sysctlbyname ("kern.clockrate", &ci, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.clockrate) (%d)", pid);
		glibtop_suid_leave (server);
		return;
	}

	buf->frequency = (ci.stathz ? ci.stathz : ci.hz);
	buf->rtime = pinfo [0].ki_runtime * buf->frequency / 1000000;
	buf->flags = _glibtop_sysdeps_proc_time;

	/*
	  All the following used to be 'if (pinfo [0].ki_flag & PS_INMEM)'
	  but it was never entered.
	  I have no idea what this PS_INMEM is, but it works perfectly
	  without this check.
	 */
	buf->utime = tv2sec_freq (pinfo [0].ki_rusage.ru_utime, buf->frequency);
	buf->stime = tv2sec_freq (pinfo [0].ki_rusage.ru_stime, buf->frequency);
	buf->cutime = tv2sec_freq (pinfo [0].ki_childtime, buf->frequency);
#if (__FreeBSD_version >= 600006) || defined(__FreeBSD_kernel__)
	buf->cstime = tv2sec_freq (pinfo [0].ki_rusage_ch.ru_stime, buf->frequency);
#else
	   buf->cstime = 0;
#endif
           buf->start_time = tv2sec (pinfo [0].ki_start);
           buf->flags |= _glibtop_sysdeps_proc_time_user;
}
