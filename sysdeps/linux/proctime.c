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
(1 << GLIBTOP_PROC_TIME_START_TIME) + (1 << GLIBTOP_PROC_TIME_UTIME) +
(1 << GLIBTOP_PROC_TIME_STIME) + (1 << GLIBTOP_PROC_TIME_CUTIME) +
(1 << GLIBTOP_PROC_TIME_CSTIME) + (1 << GLIBTOP_PROC_TIME_TIMEOUT) +
(1 << GLIBTOP_PROC_TIME_IT_REAL_VALUE);

/* Provides detailed information about a process. */

void
glibtop_get_proc_time_s (glibtop *server, glibtop_proc_time *buf, pid_t pid)
{
	char buffer [BUFSIZ], input [BUFSIZ], *tmp;
	int fd = 0, nread;
	
	glibtop_init_r (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_proc_time));

	if (pid == 0) {
		/* Client is only interested in the flags. */
		buf->flags = _glibtop_sysdeps_proc_time;
		return;
	}

	sprintf (input, "/proc/%d/stat", pid);
		
	fd = open (input, O_RDONLY);
	if (fd == -1)
		glibtop_error_r (server, "open (%s): %s",
				 input, strerror (errno));
		
	nread = read (fd, buffer, BUFSIZ);
	if (nread == -1)
		glibtop_error_r (server, "read (%s): %s",
				 input, strerror (errno));
		
	buffer [nread] = 0;
	close (fd);

	/* This is from guile-utils/gtop/proc/readproc.c */
	
	/* split into "PID (cmd" and "<rest>" */
	tmp = strrchr (buffer, ')');
	*tmp = '\0';		/* replace trailing ')' with NUL */
	/* parse these two strings separately, skipping the leading "(". */
	sscanf(tmp + 2,		/* skip space after ')' too */
	       "%*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u "
	       "%ld %ld %ld %ld %*d %*d %lu %lu %ld",
	       &buf->utime, &buf->stime, &buf->cutime, &buf->cstime,
	       &buf->timeout, &buf->it_real_value, &buf->start_time);
	
	buf->flags = _glibtop_sysdeps_proc_time;
}
