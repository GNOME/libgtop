/* Copyright (C) 2007 Joe Marcus Clarke
   This file is part of LibGTop 2.

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

#include <glibtop_private.h>

#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/param.h>
#include <string.h>

static const unsigned long _glibtop_sysdeps_proc_wd =
(1 << GLIBTOP_PROC_WD_EXE) |
(1 << GLIBTOP_PROC_WD_ROOT) |
(1 << GLIBTOP_PROC_WD_NUMBER);

void
_glibtop_init_proc_wd_s(glibtop *server)
{
	server->sysdeps.proc_wd = _glibtop_sysdeps_proc_wd;
}

char**
glibtop_get_proc_wd_s(glibtop *server, glibtop_proc_wd *buf, pid_t pid)
{
	/* TODO */
	return NULL;
}
