/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <glibtop/shm_limits.h>
#include <glibtop/command.h>

/* Provides information about sysv ipc limits. */

void
glibtop_get_shm_limits_l (glibtop *server, glibtop_shm_limits *buf)
{
	glibtop_init_r (&server, GLIBTOP_SYSDEPS_SHM_LIMITS, 0);

	if (server->features & GLIBTOP_SYSDEPS_SHM_LIMITS) {
		glibtop_call_l (server, GLIBTOP_CMND_SHM_LIMITS, 0, NULL,
				 sizeof (glibtop_shm_limits), buf);
	} else {
		glibtop_get_shm_limits_r (server, buf);
	}
}
