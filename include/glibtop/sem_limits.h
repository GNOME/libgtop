/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

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

#ifndef __GLIBTOP_SEM_LIMITS_H__
#define __GLIBTOP_SEM_LIMITS_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_SEM_LIMITS_SEMMAP	0
#define GLIBTOP_SEM_LIMITS_SEMMNI	1
#define GLIBTOP_SEM_LIMITS_SEMMNS	2
#define GLIBTOP_SEM_LIMITS_SEMMNU	3
#define GLIBTOP_SEM_LIMITS_SEMMSL	4
#define GLIBTOP_SEM_LIMITS_SEMOPM	5
#define GLIBTOP_SEM_LIMITS_SEMUME	6
#define GLIBTOP_SEM_LIMITS_SEMUSZ	7
#define GLIBTOP_SEM_LIMITS_SEMVMX	8
#define GLIBTOP_SEM_LIMITS_SEMAEM	9

#define GLIBTOP_MAX_SEM_LIMITS		10

typedef struct _glibtop_sem_limits	glibtop_sem_limits;

struct _glibtop_sem_limits
{
    u_int64_t	flags,
	semmap,		/* GLIBTOP_SEM_LIMITS_SEMMAP	*/
	semmni,		/* GLIBTOP_SEM_LIMITS_SEMMNI	*/
	semmns,		/* GLIBTOP_SEM_LIMITS_SEMMNS	*/
	semmnu,		/* GLIBTOP_SEM_LIMITS_SEMMNU	*/
	semmsl,		/* GLIBTOP_SEM_LIMITS_SEMMSL	*/
	semopm,		/* GLIBTOP_SEM_LIMITS_SEMOPM	*/
	semume,		/* GLIBTOP_SEM_LIMITS_SEMUME	*/
	semusz,		/* GLIBTOP_SEM_LIMITS_SEMUSZ	*/
	semvmx,		/* GLIBTOP_SEM_LIMITS_SEMVMX	*/
	semaem;		/* GLIBTOP_SEM_LIMITS_SEMAEM	*/
};

#define glibtop_get_sem_limits(sem)	glibtop_get_sem_limits_l(glibtop_global_server, sem)

#if GLIBTOP_SUID_SEM_LIMITS
#define glibtop_get_sem_limits_r	glibtop_get_sem_limits_p
#else
#define glibtop_get_sem_limits_r	glibtop_get_sem_limits_s
#endif

int glibtop_get_sem_limits_l (glibtop *server, glibtop_sem_limits *buf);

#if GLIBTOP_SUID_SEM_LIMITS
int glibtop_init_sem_limits_p (glibtop *server);
int glibtop_get_sem_limits_p (glibtop *server, glibtop_sem_limits *buf);
#else
int glibtop_init_sem_limits_s (glibtop *server);
int glibtop_get_sem_limits_s (glibtop *server, glibtop_sem_limits *buf);
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_sem_limits [];
extern const unsigned glibtop_types_sem_limits [];
extern const char *glibtop_labels_sem_limits [];
extern const char *glibtop_descriptions_sem_limits [];

#endif

END_LIBGTOP_DECLS

#endif
