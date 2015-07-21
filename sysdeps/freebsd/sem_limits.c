/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, August 1998.

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
#include <glibtop/error.h>
#include <glibtop/sem_limits.h>

#include <sys/types.h>
#include <sys/sysctl.h>

static unsigned long _glibtop_sysdeps_sem_limits =
(1L << GLIBTOP_IPC_SEMMAP) + (1L << GLIBTOP_IPC_SEMMNI) +
(1L << GLIBTOP_IPC_SEMMNS) + (1L << GLIBTOP_IPC_SEMMNU) +
(1L << GLIBTOP_IPC_SEMMSL) + (1L << GLIBTOP_IPC_SEMOPM) +
(1L << GLIBTOP_IPC_SEMUME) + (1L << GLIBTOP_IPC_SEMUSZ) +
(1L << GLIBTOP_IPC_SEMVMX) + (1L << GLIBTOP_IPC_SEMAEM);

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
	size_t len;
	int semmap, semmni, semmns, semmnu, semmsl, semopm, semume, semusz;
	int semvmx, semaem;

	memset (buf, 0, sizeof (glibtop_sem_limits));

	if (server->sysdeps.sem_limits == 0)
		return;

	len = sizeof (semmap);
	if (sysctlbyname ("kern.ipc.semmap", &semmap, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.semmap)");
		return;
	}

	len = sizeof (semmni);
	if (sysctlbyname ("kern.ipc.semmni", &semmni, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.semmni)");
		return;
	}

	len = sizeof (semmns);
	if (sysctlbyname ("kern.ipc.semmns", &semmns, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.semmns)");
		return;
	}

	len = sizeof (semmnu);
	if (sysctlbyname ("kern.ipc.semmnu", &semmnu, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.semmnu)");
		return;
	}

	len = sizeof (semmsl);
	if (sysctlbyname ("kern.ipc.semmsl", &semmsl, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.semmsl)");
		return;
	}

	len = sizeof (semopm);
	if (sysctlbyname ("kern.ipc.semopm", &semopm, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.semopm)");
		return;
	}

	len = sizeof (semume);
	if (sysctlbyname ("kern.ipc.semume", &semume, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.semume)");
		return;
	}

	len = sizeof (semusz);
	if (sysctlbyname ("kern.ipc.semusz", &semusz, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.semusz)");
		return;
	}

	len = sizeof (semvmx);
	if (sysctlbyname ("kern.ipc.semvmx", &semvmx, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.semvmx)");
		return;
	}

	len = sizeof (semaem);
	if (sysctlbyname ("kern.ipc.semaem", &semaem, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.semaem)");
		return;
	}

	buf->semmap = semmap;
	buf->semmni = semmni;
	buf->semmns = semmns;
	buf->semmnu = semmnu;
	buf->semmsl = semmsl;
	buf->semopm = semopm;
	buf->semume = semume;
	buf->semusz = semusz;
	buf->semvmx = semvmx;
	buf->semaem = semaem;

	buf->flags = _glibtop_sysdeps_sem_limits;
}

