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

#ifndef __GLIBTOP_MEM_H__
#define __GLIBTOP_MEM_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_MEM_TOTAL	0
#define GLIBTOP_MEM_USED	1
#define GLIBTOP_MEM_FREE	2
#define GLIBTOP_MEM_SHARED	3
#define GLIBTOP_MEM_BUFFER	4
#define GLIBTOP_MEM_CACHED	5
#define GLIBTOP_MEM_USER	6

#define GLIBTOP_MAX_MEM		7

typedef struct _glibtop_mem	glibtop_mem;

struct _glibtop_mem
{
	unsigned long	flags,
		total,		/* GLIBTOP_MEM_TOTAL	*/
		used,		/* GLIBTOP_MEM_USED	*/
		free,		/* GLIBTOP_MEM_FREE	*/
		shared,		/* GLIBTOP_MEM_SHARED	*/
		buffer,		/* GLIBTOP_MEM_BUFFER	*/
		cached,		/* GLIBTOP_MEM_CACHED	*/
		user;		/* GLIBTOP_MEM_USER	*/
};

#define glibtop_get_mem(mem)	glibtop_get_mem__r(glibtop_global_server, mem)

extern void glibtop_get_mem__r __P((glibtop *, glibtop_mem *));

#ifdef HAVE_GUILE

/* You need to link with -lgtop_guile to get this stuff here. */

extern SCM glibtop_guile_get_mem __P((void));

#endif

#ifdef GLIBTOP_GUILE_NAMES

/* You need to link with -lgtop_guile_names to get this stuff here. */

extern SCM glibtop_guile_names_mem __P((void));
extern SCM glibtop_guile_types_mem __P((void));
extern SCM glibtop_guile_labels_mem __P((void));
extern SCM glibtop_guile_descriptions_mem __P((void));

#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_mem [];
extern const char *glibtop_types_mem [];
extern const char *glibtop_labels_mem [];
extern const char *glibtop_descriptions_mem [];

#endif

__END_DECLS

#endif
