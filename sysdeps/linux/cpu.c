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

static const unsigned long _glibtop_sysdeps_cpu_smp =
(1 << GLIBTOP_XCPU_TOTAL) + (1 << GLIBTOP_XCPU_USER) +
(1 << GLIBTOP_XCPU_NICE) + (1 << GLIBTOP_XCPU_SYS) +
(1 << GLIBTOP_XCPU_IDLE);

/* Init function. */

void
glibtop_init_cpu_s (glibtop *server)
{
#if HAVE_LIBGTOP_SMP
	server->sysdeps.cpu = _glibtop_sysdeps_cpu | _glibtop_sysdeps_cpu_smp;
#else
	server->sysdeps.cpu = _glibtop_sysdeps_cpu;
#endif
}

/* Provides information about cpu usage. */

#define FILENAME	"/proc/stat"

void
glibtop_get_cpu_s (glibtop *server, glibtop_cpu *buf)
{
	char buffer [BUFSIZ], *p;
	int fd, len, i;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_CPU, 0);

	memset (buf, 0, sizeof (glibtop_cpu));

	fd = open (FILENAME, O_RDONLY);
	if (fd < 0)
		glibtop_error_io_r (server, "open (%s)", FILENAME);

	len = read (fd, buffer, BUFSIZ-1);
	if (len < 0)
		glibtop_error_io_r (server, "read (%s)", FILENAME);

	close (fd);

	buffer [len] = '\0';

	p = skip_token (buffer);	/* "cpu" */

	buf->user = strtoul (p, &p, 0);
	buf->nice = strtoul (p, &p, 0);
	buf->sys  = strtoul (p, &p, 0);
	buf->idle = strtoul (p, &p, 0);

	buf->total = buf->user + buf->nice + buf->sys + buf->idle;

	buf->frequency = 100;
	buf->flags = _glibtop_sysdeps_cpu;

#if HAVE_LIBGTOP_SMP
	for (i = 0; i < GLIBTOP_NCPU; i++) {
		u_int64_t user, nice, sys, idle;

		if (strncmp (p+1, "cpu", 3) || !isdigit (p [4]))
			break;

		p += 6;
		user = strtoul (p, &p, 0);
		nice = strtoul (p, &p, 0);
		sys  = strtoul (p, &p, 0);
		idle = strtoul (p, &p, 0);

		buf->xcpu_user [i] = strtoul (p, &p, 0);
		buf->xcpu_nice [i] = strtoul (p, &p, 0);
		buf->xcpu_sys  [i] = strtoul (p, &p, 0);
		buf->xcpu_idle [i] = strtoul (p, &p, 0);

		buf->xcpu_total [i] = buf->xcpu_user [i] + buf->xcpu_nice [i] +
			buf->xcpu_sys [i] + buf->xcpu_idle [i];
	}

	buf->flags |= _glibtop_sysdeps_cpu_smp;
#endif
}
