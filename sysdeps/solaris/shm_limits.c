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

static struct nlist nlst[] = { {"shminfo"}, {NULL} };
static const unsigned long _glibtop_sysdeps_shm_limits =
(1L << GLIBTOP_SHM_LIMITS_SHMMAX) + (1L << GLIBTOP_SHM_LIMITS_SHMMIN) +
(1L << GLIBTOP_SHM_LIMITS_SHMMNI) + (1L << GLIBTOP_SHM_LIMITS_SHMSEG);

/* Init function. */

int
glibtop_init_shm_limits_p (glibtop *server)
{
    kvm_t *kd = server->_priv->machine.kd;

    if(kd && !kvm_nlist(kd, nlst))
	server->sysdeps.shm_limits = _glibtop_sysdeps_shm_limits;
    else
	server->sysdeps.shm_limits = 0;

    return 0;
}

/* Provides information about sysv ipc limits. */

int
glibtop_get_shm_limits_p (glibtop *server, glibtop_shm_limits *buf)
{
    kvm_t *kd = server->_priv->machine.kd;
    struct shminfo sinfo;

    memset (buf, 0, sizeof (glibtop_shm_limits));

    if(!(server->sysdeps.shm_limits))
	return -GLIBTOP_ERROR_INCOMPATIBLE_KERNEL;
    if(kvm_read(kd, nlst[0].n_value, (void *)&sinfo,
		sizeof(struct shminfo)) != sizeof(struct shminfo))
	return -GLIBTOP_ERROR_INCOMPATIBLE_KERNEL;
    buf->shmmax = sinfo.shmmax;
    buf->shmmin = sinfo.shmmin;
    buf->shmmni = sinfo.shmmni;
    buf->shmseg = sinfo.shmseg;
    buf->flags = _glibtop_sysdeps_shm_limits;

    return 0;
}
