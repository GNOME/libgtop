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

#include <glibtop_suid.h>

/* #define KERNEL to get declaration of `struct seminfo'. */

#define KERNEL

#include <sys/ipc.h>
#include <sys/sem.h>

static unsigned long _glibtop_sysdeps_sem_limits =
(1 << GLIBTOP_IPC_SEMMAP) + (1 << GLIBTOP_IPC_SEMMNI) +
(1 << GLIBTOP_IPC_SEMMNS) + (1 << GLIBTOP_IPC_SEMMNU) +
(1 << GLIBTOP_IPC_SEMMSL) + (1 << GLIBTOP_IPC_SEMOPM) +
(1 << GLIBTOP_IPC_SEMUME) + (1 << GLIBTOP_IPC_SEMUSZ) +
(1 << GLIBTOP_IPC_SEMVMX) + (1 << GLIBTOP_IPC_SEMAEM);

/* Provides information about sysv sem limits. */

void
glibtop_get_sem_limits_p (glibtop *server, glibtop_sem_limits *buf)
{
	struct seminfo	seminfo;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_SEM_LIMITS), 0);

	memset (buf, 0, sizeof (glibtop_sem_limits));

	/* !!! THE FOLLOWING CODE RUNS SGID KMEM - CHANGE WITH CAUTION !!! */

	setregid (server->machine->gid, server->machine->egid);

	/* get the load average array */

	(void) _glibtop_getkval (server, _glibtop_nlist [X_SEMINFO].n_value,
				 (int *) &seminfo, sizeof (seminfo),
				 _glibtop_nlist [X_SEMINFO].n_name);

	if (setregid (server->machine->egid, server->machine->gid))
		_exit (1);

	/* !!! END OF SGID KMEM PART !!! */

	buf->semmap = seminfo.semmap;
	buf->semmni = seminfo.semmni;
	buf->semmns = seminfo.semmns;
	buf->semmnu = seminfo.semmnu;
	buf->semmsl = seminfo.semmsl;
	buf->semopm = seminfo.semopm;
	buf->semume = seminfo.semume;
	buf->semusz = seminfo.semusz;
	buf->semvmx = seminfo.semvmx;
	buf->semaem = seminfo.semaem;

	buf->flags = _glibtop_sysdeps_sem_limits;
}
