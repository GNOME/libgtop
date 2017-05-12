/* Copyright (C) 2017 Robert Roth
   This file is part of LibGTop.

   Contributed by Robert Roth <robert.roth.off@gmail.com>, February 2017.

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
#include <glibtop/procio.h>

#include "glibtop_private.h"

static const unsigned long _glibtop_sysdeps_proc_io =
(1L << GLIBTOP_PROC_IO_DISK_RCHAR) + (1L << GLIBTOP_PROC_IO_DISK_WCHAR) +
(1L << GLIBTOP_PROC_IO_DISK_RBYTES) + (1L << GLIBTOP_PROC_IO_DISK_WBYTES);

/* Init function. */

void
_glibtop_init_proc_io_s (glibtop *server)
{
	server->sysdeps.proc_io = _glibtop_sysdeps_proc_io;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_io_s (glibtop *server, glibtop_proc_io *buf, pid_t pid)
{
	char buffer [BUFSIZ], *p;
    memset (buf, 0, sizeof (glibtop_proc_io));

    if (server->os_version_code < LINUX_VERSION_CODE(2, 6, 20))
      return;

	if (proc_file_to_buffer(buffer, sizeof buffer, "/proc/%d/io", pid))
		return;

	p = skip_token (buffer);
    buf->disk_rchar = g_ascii_strtoull (p, &p, 10);
    p = skip_line (p);
    p = skip_token (p);
    buf->disk_wchar = g_ascii_strtoull (p, &p, 10);
    p = skip_line (p);
    p = skip_line (p);
    p = skip_line (p);
    p = skip_token (p);
    buf->disk_rbytes = g_ascii_strtoull (p, &p, 10);
    p = skip_line (p);
    p = skip_token (p);
    buf->disk_wbytes = g_ascii_strtoull (p, &p, 10);    

    buf->flags = _glibtop_sysdeps_proc_io;
}
