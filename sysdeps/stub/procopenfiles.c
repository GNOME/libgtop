/* Copyright (C) 1998-99 Martin Baulig
   Copyright (C) 2004 Nicolás Lichtmaier
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
#include <glibtop/error.h>
#include <glibtop/procopenfiles.h>

/* Init function. */

void
_glibtop_init_proc_open_files_s (glibtop *server)
{
	server->sysdeps.proc_open_files = 0;
}

/* Provides detailed information about a process. */

glibtop_open_files_entry *
glibtop_get_proc_open_files_s (glibtop *server, glibtop_proc_open_files *buf,	pid_t pid)
{
	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_MAP, 0);
	
	memset (buf, 0, sizeof (glibtop_proc_open_files));
	
	return NULL;
}
