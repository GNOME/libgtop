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
#include <glibtop/sem_limits.h>

static const unsigned long _glibtop_sysdeps_sem_limits = 0;

/* Init function. */

void
_glibtop_init_sem_limits_s (glibtop *server)
{
	server->sysdeps.sem_limits = _glibtop_sysdeps_sem_limits;
}

/* Provides information about sysv sem limits. */

void
glibtop_get_sem_limits_s (glibtop *server, glibtop_sem_limits *buf)
{
	memset (buf, 0, sizeof (glibtop_sem_limits));
}
