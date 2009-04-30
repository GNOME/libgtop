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

#include <config.h>
#include <glibtop/shm_limits.h>

#define _KERNEL
#include <sys/ipc.h>
#include <sys/shm.h>

static const unsigned long _glibtop_sysdeps_shm_limits =
(1L << GLIBTOP_IPC_SHMMAX) + (1L << GLIBTOP_IPC_SHMMIN) +
(1L << GLIBTOP_IPC_SHMMNI) + (1L << GLIBTOP_IPC_SHMSEG) +
(1L << GLIBTOP_IPC_SHMALL);

/* Init function. */

void
_glibtop_init_shm_limits_s (glibtop *server)
{
	server->sysdeps.shm_limits = _glibtop_sysdeps_shm_limits;
}

/* Provides information about sysv ipc limits. */

void
glibtop_get_shm_limits_s (glibtop *server, glibtop_shm_limits *buf)
{
	struct shminfo shminfo;

	memset (buf, 0, sizeof (glibtop_shm_limits));

	shmctl (0, IPC_INFO, (void *) &shminfo);

	buf->shmmax = shminfo.shmmax;
	buf->shmmin = shminfo.shmmin;
	buf->shmmni = shminfo.shmmni;
	buf->shmseg = shminfo.shmseg;
	buf->shmall = shminfo.shmall;
	buf->flags = _glibtop_sysdeps_shm_limits;
}
