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
#include <glibtop/sem_limits.h>

#include <kvm.h>
#include <sys/sem.h>

static struct nlist nlst[] = { {"seminfo"}, {NULL} };
static const unsigned long _glibtop_sysdeps_sem_limits =
(1L << GLIBTOP_SEM_LIMITS_SEMMAP) + (1L << GLIBTOP_SEM_LIMITS_SEMMNI) +
(1L << GLIBTOP_SEM_LIMITS_SEMMNS) + (1L << GLIBTOP_SEM_LIMITS_SEMMNU) +
(1L << GLIBTOP_SEM_LIMITS_SEMMSL) + (1L << GLIBTOP_SEM_LIMITS_SEMOPM) +
(1L << GLIBTOP_SEM_LIMITS_SEMUME) + (1L << GLIBTOP_SEM_LIMITS_SEMUSZ) +
(1L << GLIBTOP_SEM_LIMITS_SEMVMX) + (1L << GLIBTOP_SEM_LIMITS_SEMAEM);

/* Init function. */

int
glibtop_init_sem_limits_p (glibtop *server)
{
   	kvm_t *kd = server->_priv->machine.kd;

	if(kd && !kvm_nlist(kd, nlst))
		server->sysdeps.sem_limits = _glibtop_sysdeps_sem_limits;
	else
	   	server->sysdeps.sem_limits = 0;

	return 0;
}

/* Provides information about sysv sem limits. */

int
glibtop_get_sem_limits_p (glibtop *server, glibtop_sem_limits *buf)
{
   	kvm_t *kd = server->_priv->machine.kd;
	struct seminfo sinfo;

	memset (buf, 0, sizeof (glibtop_sem_limits));

	if(!(server->sysdeps.sem_limits))
	   	return -GLIBTOP_ERROR_INCOMPATIBLE_KERNEL;
	if(kvm_read(kd, nlst[0].n_value, (void *)&sinfo,
		    sizeof(struct seminfo)) != sizeof(struct seminfo))
	   	return -GLIBTOP_ERROR_INCOMPATIBLE_KERNEL;
	buf->semmap = sinfo.semmap;
	buf->semmni = sinfo.semmni;
	buf->semmns = sinfo.semmns;
	buf->semmnu = sinfo.semmnu;
	buf->semmsl = sinfo.semmsl;
	buf->semopm = sinfo.semopm;
	buf->semume = sinfo.semume;
	buf->semusz = sinfo.semusz;
	buf->semvmx = sinfo.semvmx;
	buf->semaem = sinfo.semaem;
	buf->flags = _glibtop_sysdeps_sem_limits;

	return 0;
}
