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
#include <glibtop/shm_limits.h>

#include <sys/types.h>
#include <sys/param.h>
#include <sys/sysctl.h>

static unsigned long _glibtop_sysdeps_shm_limits =
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
	size_t len;
#if __FreeBSD_version < 700002
	int shmmax, shmmin, shmmni, shmseg, shmall;
#else
	unsigned long shmmax, shmmin, shmmni, shmseg, shmall;
#endif

	memset (buf, 0, sizeof (glibtop_shm_limits));

	if (server->sysdeps.shm_limits == 0)
		return;

	len = sizeof (shmmax);
	if (sysctlbyname ("kern.ipc.shmmax", &shmmax, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.shmmax)");
		return;
	}

	len = sizeof (shmmin);
	if (sysctlbyname ("kern.ipc.shmmin", &shmmin, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.shmmin)");
		return;
	}

	len = sizeof (shmmni);
	if (sysctlbyname ("kern.ipc.shmmni", &shmmni, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.shmmni)");
		return;
	}

	len = sizeof (shmseg);
	if (sysctlbyname ("kern.ipc.shmseg", &shmseg, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.shmseg)");
		return;
	}

	len = sizeof (shmall);
	if (sysctlbyname ("kern.ipc.shmall", &shmall, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.shmall)");
		return;
	}

	buf->shmmax = shmmax;
	buf->shmmin = shmmin;
	buf->shmmni = shmmni;
	buf->shmseg = shmseg;
	buf->shmall = shmall;

	buf->flags = _glibtop_sysdeps_shm_limits;
}
