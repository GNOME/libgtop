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

#ifndef __GLIBTOP_CPU_H__
#define __GLIBTOP_CPU_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_CPU_TOTAL	0
#define GLIBTOP_CPU_USER	1
#define GLIBTOP_CPU_NICE	2
#define GLIBTOP_CPU_SYS		3
#define GLIBTOP_CPU_IDLE	4
#define GLIBTOP_CPU_FREQUENCY	5

#define GLIBTOP_MAX_CPU		6

typedef struct _glibtop_cpu	glibtop_cpu;

struct _glibtop_cpu
{
	unsigned long	flags,
		total,		/* GLIBTOP_CPU_TOTAL		*/
		user,		/* GLIBTOP_CPU_USER		*/
		nice,		/* GLIBTOP_CPU_NICE		*/
		sys,		/* GLIBTOP_CPU_SYS		*/
		idle,		/* GLIBTOP_CPU_IDLE		*/
		frequency;	/* GLIBTOP_CPU_FREQUENCY	*/
};

#define glibtop_get_cpu(cpu)	glibtop_get_cpu__l(glibtop_global_server, cpu)

#if GLIBTOP_SUID_CPU
#define glibtop_get_cpu__r	glibtop_get_cpu__p
#else
#define glibtop_get_cpu__r	glibtop_get_cpu__s
#endif

extern void glibtop_get_cpu__l __P((glibtop *, glibtop_cpu *));

#if GLIBTOP_SUID_CPU
extern void glibtop_get_cpu__p __P((glibtop *, glibtop_cpu *));
#else
extern void glibtop_get_cpu__s __P((glibtop *, glibtop_cpu *));
#endif

#ifdef HAVE_GUILE

/* You need to link with -lgtop_guile to get this stuff here. */

extern SCM glibtop_guile_get_cpu __P((void));

#endif

#ifdef GLIBTOP_GUILE_NAMES

/* You need to link with -lgtop_guile_names to get this stuff here. */

extern SCM glibtop_guile_names_cpu __P((void));
extern SCM glibtop_guile_types_cpu __P((void));
extern SCM glibtop_guile_labels_cpu __P((void));
extern SCM glibtop_guile_descriptions_cpu __P((void));

#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_cpu [];
extern const unsigned glibtop_types_cpu [];
extern const char *glibtop_labels_cpu [];
extern const char *glibtop_descriptions_cpu [];

#endif

__END_DECLS

#endif
