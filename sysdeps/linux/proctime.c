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
#include <glibtop/error.h>
#include <glibtop/proctime.h>

static const unsigned long _glibtop_sysdeps_proc_time =
(1 << GLIBTOP_PROC_TIME_UTIME) + (1 << GLIBTOP_PROC_TIME_CUTIME) +
(1 << GLIBTOP_PROC_TIME_STIME) + (1 << GLIBTOP_PROC_TIME_CSTIME) +
(1 << GLIBTOP_PROC_TIME_RTIME) + (1 << GLIBTOP_PROC_TIME_FREQUENCY) +
(1 << GLIBTOP_PROC_TIME_TIMEOUT) + (1 << GLIBTOP_PROC_TIME_IT_REAL_VALUE) +
(1 << GLIBTOP_PROC_TIME_START_TIME);

static const unsigned long _glibtop_sysdeps_proc_time_smp =
(1 << GLIBTOP_PROC_TIME_XCPU_UTIME) + (1 << GLIBTOP_PROC_TIME_XCPU_STIME);

/* Init function. */

void
glibtop_init_proc_time_s (glibtop *server)
{
#if HAVE_LIBGTOP_SMP
	server->sysdeps.proc_time = _glibtop_sysdeps_proc_time |
		_glibtop_sysdeps_proc_time_smp;
#else
	server->sysdeps.proc_time = _glibtop_sysdeps_proc_time;
#endif
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_time_s (glibtop *server, glibtop_proc_time *buf, pid_t pid)
{
	char buffer [BUFSIZ], *p;
	int i;
	
	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_TIME, 0);

	memset (buf, 0, sizeof (glibtop_proc_time));

	if (proc_stat_to_buffer (buffer, pid))
		return;

	p = proc_stat_after_cmd (buffer);
	if (!p) return;

	p = skip_multiple_token (p, 11);

	buf->utime  = strtoul (p, &p, 0);
	buf->stime  = strtoul (p, &p, 0);
	buf->cutime = strtoul (p, &p, 0);
	buf->cstime = strtoul (p, &p, 0);

	buf->rtime = buf->utime + buf->stime;

	p = skip_multiple_token (p, 2);

	buf->timeout = strtoul (p, &p, 0);
	buf->it_real_value = strtoul (p, &p, 0);
	buf->start_time = strtoul (p, &p, 0);

	buf->frequency = 100;

	buf->flags = _glibtop_sysdeps_proc_time;

#if HAVE_LIBGTOP_SMP
	if (proc_file_to_buffer (buffer, "/proc/%d/cpu", pid))
		return;

	p = skip_token (buffer);
	buf->utime  = strtoul (p, &p, 0);
	buf->stime  = strtoul (p, &p, 0);

	for (i = 0; i < GLIBTOP_NCPU; i++) {
		if (strncmp (p+1, "cpu", 3) || !isdigit (p [4]))
			break;

		p += 6;
		buf->xcpu_utime [i] = strtoul (p, &p, 0);
		buf->xcpu_stime [i] = strtoul (p, &p, 0);
	}

	buf->flags |= _glibtop_sysdeps_proc_time_smp;
#endif
}
