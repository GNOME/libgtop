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

#ifndef __GLIBTOP_UPTIME_H__
#define __GLIBTOP_UPTIME_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_UPTIME_UPTIME		0
#define GLIBTOP_UPTIME_IDLETIME		1

#define GLIBTOP_MAX_UPTIME		2

typedef struct _glibtop_uptime	glibtop_uptime;

struct _glibtop_uptime
{
	unsigned long	flags;
	double uptime,		/* GLIBTOP_UPTIME_UPTIME	*/
		idletime;	/* GLIBTOP_UPTIME_IDLETIME	*/
};

#define glibtop_get_uptime(uptime)	glibtop_get_uptime__l(glibtop_global_server, uptime)

#if GLIBTOP_SUID_UPTIME
#define glibtop_get_uptime__r		glibtop_get_uptime__p
#else
#define glibtop_get_uptime__r		glibtop_get_uptime__s
#endif

extern void glibtop_get_uptime__l __P((glibtop *, glibtop_uptime *));

#if GLIBTOP_SUID_UPTIME
extern void glibtop_get_uptime__p __P((glibtop *, glibtop_uptime *));
#else
extern void glibtop_get_uptime__s __P((glibtop *, glibtop_uptime *));
#endif

#ifdef HAVE_GUILE

/* You need to link with -lgtop_guile to get this stuff here. */

extern SCM glibtop_guile_get_uptime __P((void));

#endif

#ifdef GLIBTOP_GUILE_NAMES

/* You need to link with -lgtop_guile_names to get this stuff here. */

extern SCM glibtop_guile_names_uptime __P((void));
extern SCM glibtop_guile_types_uptime __P((void));
extern SCM glibtop_guile_labels_uptime __P((void));
extern SCM glibtop_guile_descriptions_uptime __P((void));

#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_uptime [];
extern const char *glibtop_types_uptime [];
extern const char *glibtop_labels_uptime [];
extern const char *glibtop_descriptions_uptime [];

#endif

__END_DECLS

#endif
