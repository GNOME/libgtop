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
#include <glibtop/shm_limits.h>

#include <glibtop_suid.h>

/* #define KERNEL to get declaration of `struct shminfo'. */

#define KERNEL

#include <sys/ipc.h>
#include <sys/shm.h>

static unsigned long _glibtop_sysdeps_shm_limits =
(1 << GLIBTOP_IPC_SHMMAX) + (1 << GLIBTOP_IPC_SHMMIN) +
(1 << GLIBTOP_IPC_SHMMNI) + (1 << GLIBTOP_IPC_SHMSEG) +
(1 << GLIBTOP_IPC_SHMALL);

/* Provides information about sysv ipc limits. */

void
glibtop_get_shm_limits_p (glibtop *server, glibtop_shm_limits *buf)
{
	struct shminfo	shminfo;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_SHM_LIMITS), 0);

	memset (buf, 0, sizeof (glibtop_shm_limits));

	/* !!! THE FOLLOWING CODE RUNS SGID KMEM - CHANGE WITH CAUTION !!! */

	setregid (server->machine->gid, server->machine->egid);

	/* get the load average array */

	(void) _glibtop_getkval (server, _glibtop_nlist [X_SHMINFO].n_value,
				 (int *) &shminfo, sizeof (shminfo),
				 _glibtop_nlist [X_SHMINFO].n_name);

	if (setregid (server->machine->egid, server->machine->gid))
		_exit (1);

	/* !!! END OF SGID KMEM PART !!! */

  	buf->shmmax = shminfo.shmmax;
	buf->shmmin = shminfo.shmmin;
	buf->shmmni = shminfo.shmmni;
	buf->shmseg = shminfo.shmseg;
	buf->shmall = shminfo.shmall;

	buf->flags = _glibtop_sysdeps_shm_limits;
}
