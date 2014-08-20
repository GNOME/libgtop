/* Copyright (C) 1998-99 Martin Baulig
   Copyright (C) 2004 Nicol�s Lichtmaier
   This file is part of LibGTop 1.0.

   Modified by Nicol�s Lichtmaier to give a process open files.

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
#include <glibtop/procopenfiles.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_proc_open_files =
(1L << GLIBTOP_PROC_OPEN_FILES_NUMBER)|
(1L << GLIBTOP_PROC_OPEN_FILES_TOTAL)|
(1L << GLIBTOP_PROC_OPEN_FILES_SIZE);

/* Init function. */

void
_glibtop_init_proc_open_files_s (glibtop *server)
{
	server->sysdeps.proc_open_files = _glibtop_sysdeps_proc_open_files;
}


/* XXX Unimplemented on FreeBSD */
glibtop_open_files_entry *
glibtop_get_proc_open_files_s (glibtop *server, glibtop_proc_open_files *buf,	pid_t pid)
{
	return NULL;
}
