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
#include <glibtop/procmem.h>

static const unsigned long _glibtop_sysdeps_proc_mem =
(1 << GLIBTOP_PROC_MEM_SIZE) + (1 << GLIBTOP_PROC_MEM_VSIZE) +
(1 << GLIBTOP_PROC_MEM_RESIDENT) + (1 << GLIBTOP_PROC_MEM_SHARE) +
(1 << GLIBTOP_PROC_MEM_RSS) + (1 << GLIBTOP_PROC_MEM_RSS_RLIM);

/* Provides detailed information about a process. */

void
glibtop_get_proc_mem_s (glibtop *server, glibtop_proc_mem *buf, pid_t pid)
{
	char buffer [BUFSIZ], input [BUFSIZ], *tmp;
	int fd = 0, nread;
	
	glibtop_init_r (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_proc_mem));

	if (pid == 0) {
		/* Client is only interested in the flags. */
		buf->flags = _glibtop_sysdeps_proc_mem;
		return;
	}

	if (pid != server->machine.last_pid) {
		server->machine.last_pid = pid;
		server->machine.no_update = 0;
	}

	if (!server->machine.no_update) {
		server->machine.proc_status [0] = 0;
		server->machine.proc_statm [0] = 0;
		server->machine.proc_stat [0] = 0;
	}

	if (server->machine.proc_stat [0]) {
		strcpy (buffer, server->machine.proc_stat);
	} else {
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
		strcpy (server->machine.proc_stat, buffer);
		close (fd);
	}
	
	/* This is from guile-utils/gtop/proc/readproc.c */
	
	/* split into "PID (cmd" and "<rest>" */
	tmp = strrchr (buffer, ')');
	*tmp = '\0';		/* replace trailing ')' with NUL */
	/* parse these two strings separately, skipping the leading "(". */
	sscanf(tmp + 2,		/* skip space after ')' too */
	       "%*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u "
	       "%*d %*d %*d %*d %*d %*d %*u %*u %*d %lu "
	       "%lu %lu", &buf->vsize, &buf->rss, &buf->rss_rlim);
	
	if (server->machine.proc_statm [0]) {
		strcpy (buffer, server->machine.proc_statm);
	} else {
		sprintf (input, "/proc/%d/statm", pid);
		
		fd = open (input, O_RDONLY);
		if (fd == -1)
			glibtop_error_r (server, "open (%s): %s",
					 input, strerror (errno));
		
		nread = read (fd, buffer, BUFSIZ);
		if (nread == -1)
			glibtop_error_r (server, "read (%s): %s",
					 input, strerror (errno));

		buffer [nread] = 0;
		strcpy (server->machine.proc_statm, buffer);
		close (fd);
	}
		
	sscanf (buffer, "%ld %ld %ld",
		&buf->size, &buf->resident, &buf->share);

	buf->flags = _glibtop_sysdeps_proc_mem;
}
