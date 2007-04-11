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

#ifndef __GLIBTOP_SHM_LIMITS_H__
#define __GLIBTOP_SHM_LIMITS_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_IPC_SHMMAX	0
#define GLIBTOP_IPC_SHMMIN	1
#define GLIBTOP_IPC_SHMMNI	2
#define GLIBTOP_IPC_SHMSEG	3
#define GLIBTOP_IPC_SHMALL	4

#define GLIBTOP_MAX_SHM_LIMITS	5

typedef struct _glibtop_shm_limits	glibtop_shm_limits;

struct _glibtop_shm_limits
{
	guint64	flags;
	guint64 shmmax;		/* GLIBTOP_IPC_SHMMAX	*/
	guint64 shmmin;		/* GLIBTOP_IPC_SHMMIN	*/
	guint64 shmmni;		/* GLIBTOP_IPC_SHMMNI	*/
	guint64 shmseg;		/* GLIBTOP_IPC_SHMSEG	*/
	guint64 shmall;		/* GLIBTOP_IPC_SHMALL	*/
};

void glibtop_get_shm_limits(glibtop_shm_limits *buf);

#if GLIBTOP_SUID_SHM_LIMITS
#define glibtop_get_shm_limits_r	glibtop_get_shm_limits_p
#else
#define glibtop_get_shm_limits_r	glibtop_get_shm_limits_s
#endif

void glibtop_get_shm_limits_l (glibtop *server, glibtop_shm_limits *buf);

#if GLIBTOP_SUID_SHM_LIMITS
void _glibtop_init_shm_limits_p (glibtop *server);
void glibtop_get_shm_limits_p (glibtop *, glibtop_shm_limits *buf);
#else
void _glibtop_init_shm_limits_s (glibtop *server);
void glibtop_get_shm_limits_s (glibtop *server, glibtop_shm_limits *buf);
#endif


G_END_DECLS

#endif
