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

/* The values in this structure never change at runtime, so we only
 * read it once during initialization. We have to use the name `_seminfo'
 * since `seminfo' is already declared external in <sys/sem.h>. */
static struct seminfo _seminfo;
  
/* nlist structure for kernel access */
static struct nlist nlst [] = {
	{ "_seminfo" },
	{ 0 }
};

/* Init function. */

void
glibtop_init_sem_limits_p (glibtop *server)
{
	server->sysdeps.sem_limits = _glibtop_sysdeps_sem_limits;

	if (kvm_nlist (server->machine.kd, nlst) != 0)
		glibtop_error_io_r (server, "kvm_nlist");
	
	if (kvm_read (server->machine.kd, nlst [0].n_value,
		      &_seminfo, sizeof (_seminfo)) != sizeof (_seminfo))
		glibtop_error_io_r (server, "kvm_read (seminfo)");
}

/* Provides information about sysv sem limits. */

void
glibtop_get_sem_limits_p (glibtop *server, glibtop_sem_limits *buf)
{
	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_SEM_LIMITS), 0);
	
	memset (buf, 0, sizeof (glibtop_sem_limits));
	
	buf->semmap = _seminfo.semmap;
	buf->semmni = _seminfo.semmni;
	buf->semmns = _seminfo.semmns;
	buf->semmnu = _seminfo.semmnu;
	buf->semmsl = _seminfo.semmsl;
	buf->semopm = _seminfo.semopm;
	buf->semvmx = _seminfo.semvmx;
	buf->semaem = _seminfo.semaem;
	
	buf->flags = _glibtop_sysdeps_sem_limits;
}
