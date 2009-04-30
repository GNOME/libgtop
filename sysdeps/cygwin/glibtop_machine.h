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

#ifndef __GLIBTOP_MACHINE_H__
#define __GLIBTOP_MACHINE_H__

#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

G_BEGIN_DECLS

typedef struct _glibtop_machine		glibtop_machine;

struct _glibtop_machine
{
	pid_t	last_pid;
	int	no_update;
	int	fd_stat, fd_meminfo, fd_loadavg;
	char	proc_stat [BUFSIZ], proc_statm [BUFSIZ];
	char	proc_status [BUFSIZ];
};

G_END_DECLS

#endif
