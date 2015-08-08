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
#include <glibtop/error.h>

#include "utils.h"

static const unsigned long _glibtop_sysdeps_sem_limits =
(1 << GLIBTOP_IPC_SEMMNI) + (1 << GLIBTOP_IPC_SEMMSL) +
(1 << GLIBTOP_IPC_SEMOPM) + (1 << GLIBTOP_IPC_SEMUME) +
(1 << GLIBTOP_IPC_SEMUSZ) + (1 << GLIBTOP_IPC_SEMVMX) +
(1 << GLIBTOP_IPC_SEMAEM);

/* Init function. */

void
_glibtop_init_sem_limits_p (glibtop *server)
{
	off_t result;

	result = _glibtop_get_kmem_offset(server, "seminfo");
	if (result <= 0)
	{
		server->sysdeps.sem_limits = 0;
		return;
	}

	server->machine->seminfo_offset = result;

	server->sysdeps.sem_limits = _glibtop_sysdeps_sem_limits;
}

/* Provides information about sysv sem limits. */

void
glibtop_get_sem_limits_p (glibtop *server, glibtop_sem_limits *buf)
{
	int result;
	struct seminfo seminfo;

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_SEM_LIMITS), 0);

	memset (buf, 0, sizeof (glibtop_sem_limits));

	result = _glibtop_get_kmem_info(server, server->machine->seminfo_offset,
					&seminfo, sizeof(seminfo));

	if (result <= 0)
	{
		glibtop_error_io_r (server, "Cannot read seminfo");

		return;
	}

	buf->semmni = seminfo.semmni;
	buf->semmsl = seminfo.semmsl;
	buf->semopm = seminfo.semopm;
	buf->semume = seminfo.semume;
	buf->semusz = seminfo.semusz;
	buf->semvmx = seminfo.semvmx;
	buf->semaem = seminfo.semaem;

	buf->flags = _glibtop_sysdeps_sem_limits;
}
