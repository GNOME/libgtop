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

#include <config.h>
#include <glibtop/error.h>
#include <glibtop/cpu.h>

static const unsigned long _glibtop_sysdeps_cpu =
(1 << GLIBTOP_CPU_TOTAL) + (1 << GLIBTOP_CPU_USER) +
(1 << GLIBTOP_CPU_NICE) + (1 << GLIBTOP_CPU_SYS) +
(1 << GLIBTOP_CPU_IDLE) + (1 << GLIBTOP_CPU_FREQUENCY);

/* Provides information about cpu usage. */

#define FILENAME	"/proc/stat"

void
glibtop_get_cpu_s (glibtop *server, glibtop_cpu *buf)
{
	char buffer [BUFSIZ];
	int fd = 0, ret;

	glibtop_init_r (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_cpu));

	buf->flags = _glibtop_sysdeps_cpu;

#ifdef GLIBTOP_CACHE_OPEN
	fd = server->machine.fd_stat;
#endif
	if (fd == 0) {
		fd = open (FILENAME, O_RDONLY);
		if (fd == -1)
			glibtop_error_r (server, "open (%s): %s",
					 FILENAME, strerror (errno));
	} else {
		lseek (fd, 0, SEEK_SET);
	}

	ret = read (fd, buffer, BUFSIZ);
	if (ret == -1)
		glibtop_error_r (server, "read (%s): %s",
				 FILENAME, strerror (errno));

	sscanf (buffer, "cpu %lu %lu %lu %lu\n",
		&buf->user, &buf->nice, &buf->sys, &buf->idle);

	buf->total = buf->user + buf->nice + buf->sys + buf->idle;

	buf->frequency = 100;
  
#ifdef GLIBTOP_CACHE_OPEN
	server->machine.fd_stat = fd;
#else
	close (fd);
#endif
}
