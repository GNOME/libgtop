/*
   This file is part of LibGTop 2.0.

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
#include <glibtop/procwd.h>
#include <glibtop/error.h>

#include <glibtop_suid.h>

#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

static const unsigned long _glibtop_sysdeps_proc_wd = 0;

/* Init function. */

void
_glibtop_init_proc_wd_p (glibtop *server)
{
	server->sysdeps.proc_wd = _glibtop_sysdeps_proc_wd;
}

/* Provides detailed information about a process. */

char **
glibtop_get_proc_wd_p (glibtop *server, glibtop_proc_wd *buf,
			pid_t pid)
{
	glibtop_init_p (server, GLIBTOP_SYSDEPS_PROC_WD, 0);

	memset (buf, 0, sizeof (glibtop_proc_wd));
	return NULL;
}
