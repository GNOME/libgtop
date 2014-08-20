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
#include <glibtop/cpu.h>
#include <glibtop/open.h>
#include <glibtop/error.h>

#include "glibtop_private.h"

/* ======================================================= */

/* Opens pipe to gtop server. Returns 0 on success and -1 on error. */

#define FILENAME	"/proc/stat"

void
glibtop_open_s (glibtop *server, const char *program_name,
		const unsigned long features,
		const unsigned flags)
{
	char buffer [BUFSIZ], *p = buffer;

	server->name = program_name;

	file_to_buffer(server, buffer, sizeof buffer, FILENAME);

	p = skip_line(p); /* cpu */

	for (server->real_ncpu = 0; /* nop */; server->real_ncpu++) {

		if (!check_cpu_line(server, p, server->real_ncpu)) {
			server->real_ncpu--;
			break;
		}

		p = skip_line(p);
	}

	server->ncpu = MIN(GLIBTOP_NCPU - 1, server->real_ncpu);

	if (server->real_ncpu != server->ncpu) {
		glibtop_warn_r(server,
			       "This machine has %d CPUs, "
			       "%d are being monitored.",
			       server->real_ncpu + 1,
			       server->ncpu + 1);
	}
}
