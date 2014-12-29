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

/* =====================================================
 * Linux kernel version information for procps utilities
 * Copyright (c) 1996 Charles Blake <cblake@bbn.com>
 */
#include <sys/utsname.h>

static void set_linux_version(glibtop *server)
{
	struct utsname uts;
	unsigned x = 0, y = 0, z = 0;	/* cleared in case sscanf() < 3 */
	int version_string_depth;

	if (uname(&uts) == -1) /* failure most likely implies impending death */
		glibtop_error_r(server, "uname() failed");

	version_string_depth = sscanf(uts.release, "%u.%u.%u", &x, &y, &z);

	if ((version_string_depth < 2) ||                /* Non-standard for all known kernels */
	   ((version_string_depth < 3) && (x < 3)))      /* Non-standard for 2.x.x kernels */
		glibtop_warn_r(server,
			       "Non-standard uts for running kernel:\n"
			       "release %s=%u.%u.%u gives version code %d\n",
			       uts.release, x, y, z, LINUX_VERSION_CODE(x,y,z));

	if (LINUX_VERSION_CODE(x, y, z) >= LINUX_VERSION_CODE(2, 6, 0)
	    && !has_sysfs())
		glibtop_warn_r(server,
			       "You're running a 2.6 kernel without /sys."
			       "You should mount it.");

    server->os_version_code = LINUX_VERSION_CODE(x, y, z);
}

/* ======================================================= */

/* Opens pipe to gtop server. Returns 0 on success and -1 on error. */

#define FILENAME	"/proc/stat"
#define STAT_BUFSIZ     81920

void
glibtop_open_s (glibtop *server, const char *program_name,
		const unsigned long features,
		const unsigned flags)
{
	char buffer [STAT_BUFSIZ], *p = buffer;

	server->name = program_name;

	set_linux_version(server);

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
