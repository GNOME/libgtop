/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

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

#include <glibtop/sem_limits.h>

#include <sys/ipc.h>
#include <sys/sem.h>

#ifdef _SEM_SEMUN_UNDEFINED
union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};
#endif

static unsigned long _glibtop_sysdeps_sem_limits =
(1 << GLIBTOP_IPC_SEMMAP) + (1 << GLIBTOP_IPC_SEMMNI) +
(1 << GLIBTOP_IPC_SEMMNS) + (1 << GLIBTOP_IPC_SEMMNU) +
(1 << GLIBTOP_IPC_SEMMSL) + (1 << GLIBTOP_IPC_SEMOPM) +
(1 << GLIBTOP_IPC_SEMUME) + (1 << GLIBTOP_IPC_SEMUSZ) +
(1 << GLIBTOP_IPC_SEMVMX) + (1 << GLIBTOP_IPC_SEMAEM);

/* Provides information about sysv ipc limits. */

void
glibtop_get_sem_limits_s (glibtop *server, glibtop_sem_limits *buf)
{
	struct seminfo	seminfo;
	union semun	arg;  
  
	glibtop_init_r (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_sem_limits));
  
	buf->flags = _glibtop_sysdeps_sem_limits;
  
	arg.array = (ushort *) &seminfo;
	semctl (0, 0, IPC_INFO, arg);
  
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
}
