/* $Id$ */

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
#include <glibtop/error.h>
#include <glibtop/cpu.h>

static const unsigned long _glibtop_sysdeps_cpu =
(1L << GLIBTOP_CPU_TOTAL) + (1L << GLIBTOP_CPU_USER) +
(1L << GLIBTOP_CPU_NICE) + (1L << GLIBTOP_CPU_SYS) +
(1L << GLIBTOP_CPU_IDLE) + (1L << GLIBTOP_CPU_FREQUENCY);

static const unsigned long _glibtop_sysdeps_cpu_smp =
(1L << GLIBTOP_XCPU_TOTAL) + (1L << GLIBTOP_XCPU_USER) +
(1L << GLIBTOP_XCPU_NICE) + (1L << GLIBTOP_XCPU_SYS) +
(1L << GLIBTOP_XCPU_IDLE);

/* Init function. */

void
glibtop_init_cpu_s (glibtop *server)
{
	server->sysdeps.cpu = _glibtop_sysdeps_cpu;

	if (server->ncpu)
		server->sysdeps.cpu |= _glibtop_sysdeps_cpu_smp;
}

/* Provides information about cpu usage. */

#define FILENAME	"/proc/stat"

void
glibtop_get_cpu_s (glibtop *server, glibtop_cpu *buf)
{
	char buffer [BUFSIZ], *p;
	int fd, len, i;
	guint64 total;

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

	total = buf->user;
	total += buf->nice;
	total += buf->sys;
	total += buf->idle;
	buf->total = total;

	buf->frequency = 100;
	buf->flags = _glibtop_sysdeps_cpu;

	for (i = 0; i < server->ncpu; i++) {
		if (strncmp (p+1, "cpu", 3) || !isdigit (p [4]))
			break;

		p += 6;
		buf->xcpu_user [i] = strtoul (p, &p, 0);
		buf->xcpu_nice [i] = strtoul (p, &p, 0);
		buf->xcpu_sys  [i] = strtoul (p, &p, 0);
		buf->xcpu_idle [i] = strtoul (p, &p, 0);

		total = buf->xcpu_user [i];
		total += buf->xcpu_nice [i];
		total += buf->xcpu_sys [i];
		total += buf->xcpu_idle [i];

		buf->xcpu_total [i] = total;
	}

	buf->flags |= _glibtop_sysdeps_cpu_smp;
}
