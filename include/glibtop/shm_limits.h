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

#ifndef __GLIBTOP_SHM_LIMITS_H__
#define __GLIBTOP_SHM_LIMITS_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_IPC_SHMMAX	0
#define GLIBTOP_IPC_SHMMIN	1
#define GLIBTOP_IPC_SHMMNI	2
#define GLIBTOP_IPC_SHMSEG	3
#define GLIBTOP_IPC_SHMALL	4

#define GLIBTOP_MAX_SHM_LIMITS	5

typedef struct _glibtop_shm_limits	glibtop_shm_limits;

struct _glibtop_shm_limits
{
	u_int64_t	flags,
		shmmax,		/* GLIBTOP_IPC_SHMMAX	*/
		shmmin,		/* GLIBTOP_IPC_SHMMIN	*/
		shmmni,		/* GLIBTOP_IPC_SHMMNI	*/
		shmseg,		/* GLIBTOP_IPC_SHMSEG	*/
		shmall;		/* GLIBTOP_IPC_SHMALL	*/
};

#define glibtop_get_shm_limits(shm)	glibtop_get_shm_limits_l(glibtop_global_server, shm)

#if GLIBTOP_SUID_SHM_LIMITS
#define glibtop_get_shm_limits_r	glibtop_get_shm_limits_p
#else
#define glibtop_get_shm_limits_r	glibtop_get_shm_limits_s
#endif

extern void glibtop_get_shm_limits_l __P((glibtop *, glibtop_shm_limits *));

#if GLIBTOP_SUID_SHM_LIMITS
extern void glibtop_get_shm_limits_p __P((glibtop *, glibtop_shm_limits *));
#else
extern void glibtop_get_shm_limits_s __P((glibtop *, glibtop_shm_limits *));
#endif

#ifdef GLIBTOP_GUILE

/* You need to link with -lgtop_guile to get this stuff here. */

extern SCM glibtop_guile_get_shm_limits __P((void));

#endif

#ifdef GLIBTOP_GUILE_NAMES

/* You need to link with -lgtop_guile_names to get this stuff here. */

extern SCM glibtop_guile_names_shm_limits __P((void));
extern SCM glibtop_guile_types_shm_limits __P((void));
extern SCM glibtop_guile_labels_shm_limits __P((void));
extern SCM glibtop_guile_descriptions_shm_limits __P((void));

#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_shm_limits [];
extern const unsigned glibtop_types_shm_limits [];
extern const char *glibtop_labels_shm_limits [];
extern const char *glibtop_descriptions_shm_limits [];

#endif

__END_DECLS

#endif
