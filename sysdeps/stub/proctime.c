/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

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

#include <glibtop.h>
#include <glibtop/proctime.h>

static const unsigned long _glibtop_sysdeps_proc_time = 0;

/* Init function. */

int
glibtop_init_proc_time_s (glibtop_server *server)
{
    server->info->sysdeps.proc_time = _glibtop_sysdeps_proc_time;

    return 0;
}

/* Provides detailed information about a process. */

int
glibtop_get_proc_time_s (glibtop_server *server, glibtop_proc_time *buf,
			 pid_t pid)
{
    glibtop_init_s (server, GLIBTOP_SYSDEPS_PROC_TIME, 0);
	
    memset (buf, 0, sizeof (glibtop_proc_time));

    return 0;
}
