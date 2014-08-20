/* Copyright (C) 1998 Joshua Sled
   This file is part of LibGTop 1.0.

   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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
#include <sys/types.h>
#include <sys/sysctl.h>
#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/cpu.h>
#include <glibtop/error.h>

#include <glibtop_private.h>


void
glibtop_open_s (glibtop *server, const char *program_name,
		const unsigned long features,
		const unsigned flags)
{
	int ncpus;
	size_t len;

	len = sizeof (ncpus);
	sysctlbyname ("hw.ncpu", &ncpus, &len, NULL, 0);
	server->real_ncpu = ncpus - 1;
	server->ncpu = MIN(GLIBTOP_NCPU - 1, server->real_ncpu);

	server->os_version_code = __FreeBSD_version;

}
