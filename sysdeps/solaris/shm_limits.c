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
#include <glibtop/shm_limits.h>

#include <kvm.h>
#include <sys/shm.h>

static const struct nlist nlst[] = { {"shminfo"}, {NULL} };

#if GLIBTOP_SOLARIS_RELEASE < 590
static const unsigned long _glibtop_sysdeps_shm_limits =
(1L << GLIBTOP_IPC_SHMMAX) + (1L << GLIBTOP_IPC_SHMMIN);

#else
static const unsigned long _glibtop_sysdeps_shm_limits =
(1L << GLIBTOP_IPC_SHMMAX) + (1L << GLIBTOP_IPC_SHMMIN) +
(1L << GLIBTOP_IPC_SHMMNI) + (1L << GLIBTOP_IPC_SHMSEG);
#endif

/* Init function. */

void
glibtop_init_shm_limits_p (glibtop *server)
{
	kvm_t * const kd = server->machine.kd;

	if(kd && !kvm_nlist(kd, nlst))
		server->sysdeps.shm_limits = _glibtop_sysdeps_shm_limits;
	else
		server->sysdeps.shm_limits = 0;
}

/* Provides information about sysv ipc limits. */

void
glibtop_get_shm_limits_p (glibtop *server, glibtop_shm_limits *buf)
{
	kvm_t * const kd = server->machine.kd;
	struct shminfo sinfo;

	memset (buf, 0, sizeof (glibtop_shm_limits));

	if(!(server->sysdeps.shm_limits))
		return;

	if(kvm_read(kd, nlst[0].n_value, (void *)&sinfo,
		    sizeof(struct shminfo)) != sizeof(struct shminfo))
		return;

	buf->shmmax = sinfo.shmmax;
	buf->shmmni = sinfo.shmmni;
#if GLIBTOP_SOLARIS_RELEASE < 590
	buf->shmmin = sinfo.shmmin;
	buf->shmseg = sinfo.shmseg;
#endif
	buf->flags = _glibtop_sysdeps_shm_limits;
}
