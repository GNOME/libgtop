/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, August 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <glibtop.h>
#include <glibtop/error.h>
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

/* The values in this structure never change at runtime, so we only
 * read it once during initialization. We have to use the name `_shminfo'
 * since `shminfo' is already declared external in <sys/shm.h>. */
static struct shminfo _shminfo;
  
/* nlist structure for kernel access */
static struct nlist nlst [] = {
	{ "_shminfo" },
	{ 0 }
};

/* Init function. */

void
glibtop_init_shm_limits_p (glibtop *server)
{
	server->sysdeps.shm_limits = _glibtop_sysdeps_shm_limits;

	if (kvm_nlist (server->machine.kd, nlst) != 0)
		glibtop_error_io_r (server, "kvm_nlist");
	
	if (kvm_read (server->machine.kd, nlst [0].n_value,
		      &_shminfo, sizeof (_shminfo)) != sizeof (_shminfo))
		glibtop_error_io_r (server, "kvm_read (shminfo)");
}

/* Provides information about sysv ipc limits. */

void
glibtop_get_shm_limits_p (glibtop *server, glibtop_shm_limits *buf)
{
	glibtop_init_p (server, GLIBTOP_SYSDEPS_SHM_LIMITS, 0);
	
	memset (buf, 0, sizeof (glibtop_shm_limits));

	buf->shmmax = _shminfo.shmmax;
	buf->shmmin = _shminfo.shmmin;
	buf->shmmni = _shminfo.shmmni;
	buf->shmseg = _shminfo.shmseg;
	buf->shmall = _shminfo.shmall;

	buf->flags = _glibtop_sysdeps_shm_limits;
}
