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

#ifndef __GLIBTOP_SEM_LIMITS_H__
#define __GLIBTOP_SEM_LIMITS_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_IPC_SEMMAP	0
#define GLIBTOP_IPC_SEMMNI	1
#define GLIBTOP_IPC_SEMMNS	2
#define GLIBTOP_IPC_SEMMNU	3
#define GLIBTOP_IPC_SEMMSL	4
#define GLIBTOP_IPC_SEMOPM	5
#define GLIBTOP_IPC_SEMUME	6
#define GLIBTOP_IPC_SEMUSZ	7
#define GLIBTOP_IPC_SEMVMX	8
#define GLIBTOP_IPC_SEMAEM	9

#define GLIBTOP_MAX_SEM_LIMITS	10

typedef struct _glibtop_sem_limits	glibtop_sem_limits;

struct _glibtop_sem_limits
{
	u_int64_t	flags,
		semmap,		/* GLIBTOP_IPC_SEMMAP	*/
		semmni,		/* GLIBTOP_IPC_SEMMNI	*/
		semmns,		/* GLIBTOP_IPC_SEMMNS	*/
		semmnu,		/* GLIBTOP_IPC_SEMMNU	*/
		semmsl,		/* GLIBTOP_IPC_SEMMSL	*/
		semopm,		/* GLIBTOP_IPC_SEMOPM	*/
		semume,		/* GLIBTOP_IPC_SEMUME	*/
		semusz,		/* GLIBTOP_IPC_SEMUSZ	*/
		semvmx,		/* GLIBTOP_IPC_SEMVMX	*/
		semaem;		/* GLIBTOP_IPC_SEMAEM	*/
};

#define glibtop_get_sem_limits(sem)	glibtop_get_sem_limits_l(glibtop_global_server, sem)

#if GLIBTOP_SUID_SEM_LIMITS
#define glibtop_get_sem_limits_r	glibtop_get_sem_limits_p
#else
#define glibtop_get_sem_limits_r	glibtop_get_sem_limits_s
#endif

extern void glibtop_get_sem_limits_l __P((glibtop *, glibtop_sem_limits *));

#if GLIBTOP_SUID_SEM_LIMITS
extern void glibtop_get_sem_limits_p __P((glibtop *, glibtop_sem_limits *));
#else
extern void glibtop_get_sem_limits_s __P((glibtop *, glibtop_sem_limits *));
#endif

#ifdef GLIBTOP_GUILE

/* You need to link with -lgtop_guile to get this stuff here. */

extern SCM glibtop_guile_get_sem_limits __P((void));

#endif

#ifdef GLIBTOP_GUILE_NAMES

/* You need to link with -lgtop_guile_names to get this stuff here. */

extern SCM glibtop_guile_names_sem_limits __P((void));
extern SCM glibtop_guile_types_sem_limits __P((void));
extern SCM glibtop_guile_labels_sem_limits __P((void));
extern SCM glibtop_guile_descriptions_sem_limits __P((void));

#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_sem_limits [];
extern const unsigned glibtop_types_sem_limits [];
extern const char *glibtop_labels_sem_limits [];
extern const char *glibtop_descriptions_sem_limits [];

#endif

__END_DECLS

#endif
