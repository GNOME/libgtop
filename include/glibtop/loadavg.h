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

#ifndef __GLIBTOP_LOADAVG_H__
#define __GLIBTOP_LOADAVG_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_LOADAVG_LOADAVG		0

#define GLIBTOP_MAX_LOADAVG		1

typedef struct _glibtop_loadavg	glibtop_loadavg;

struct _glibtop_loadavg
{
	unsigned long	flags;
	double loadavg [3];		/* GLIBTOP_LOADAVG_LOADAVG	*/
};

#define glibtop_get_loadavg(loadavg)	glibtop_get_loadavg_l(glibtop_global_server, loadavg)

#if GLIBTOP_SUID_LOADAVG
#define glibtop_get_loadavg_r		glibtop_get_loadavg_p
#else
#define glibtop_get_loadavg_r		glibtop_get_loadavg_s
#endif

extern void glibtop_get_loadavg_l __P((glibtop *, glibtop_loadavg *));

#if GLIBTOP_SUID_LOADAVG
extern void glibtop_get_loadavg_p __P((glibtop *, glibtop_loadavg *));
#else
extern void glibtop_get_loadavg_s __P((glibtop *, glibtop_loadavg *));
#endif

#ifdef HAVE_GUILE

/* You need to link with -lgtop_guile to get this stuff here. */

extern SCM glibtop_guile_get_loadavg __P((void));

#endif

#ifdef GLIBTOP_GUILE_NAMES

/* You need to link with -lgtop_guile_names to get this stuff here. */

extern SCM glibtop_guile_names_loadavg __P((void));
extern SCM glibtop_guile_types_loadavg __P((void));
extern SCM glibtop_guile_labels_loadavg __P((void));
extern SCM glibtop_guile_descriptions_loadavg __P((void));

#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_loadavg [];
extern const char *glibtop_types_loadavg [];
extern const char *glibtop_labels_loadavg [];
extern const char *glibtop_descriptions_loadavg [];

#endif

__END_DECLS

#endif
