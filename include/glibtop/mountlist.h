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

#ifndef __GLIBTOP_MOUNTLIST_H__
#define __GLIBTOP_MOUNTLIST_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_MOUNTLIST_NUMBER	0
#define GLIBTOP_MOUNTLIST_TOTAL		1
#define GLIBTOP_MOUNTLIST_SIZE		2

#define GLIBTOP_MAX_MOUNTLIST		3

#define GLIBTOP_MOUNTENTRY_LEN		79

typedef struct _glibtop_mountentry	glibtop_mountentry;

typedef struct _glibtop_mountlist	glibtop_mountlist;

struct _glibtop_mountentry
{
	u_int64_t dev;
	char devname [GLIBTOP_MOUNTENTRY_LEN+1];
	char mountdir [GLIBTOP_MOUNTENTRY_LEN+1];
	char type [GLIBTOP_MOUNTENTRY_LEN+1];
};

struct _glibtop_mountlist
{
	u_int64_t	flags,
		number,			/* GLIBTOP_MOUNTLIST_NUMBER	*/
		total,			/* GLIBTOP_MOUNTLIST_TOTAL	*/
		size;			/* GLIBTOP_MOUNTLIST_SIZE	*/
};

#define glibtop_get_mountlist(mountlist,all_fs)	glibtop_get_mountlist_l(glibtop_global_server, mountlist, all_fs)

#define glibtop_get_mountlist_r		glibtop_get_mountlist_s

extern glibtop_mountentry *glibtop_get_mountlist_l __P((glibtop *, glibtop_mountlist *, int));

extern glibtop_mountentry *glibtop_get_mountlist_s __P((glibtop *, glibtop_mountlist *, int));

#ifdef GLIBTOP_GUILE

/* You need to link with -lgtop_guile to get this stuff here. */

extern SCM glibtop_guile_get_mountlist __P((SCM));

#endif

#ifdef GLIBTOP_GUILE_NAMES

/* You need to link with -lgtop_guile_names to get this stuff here. */

extern SCM glibtop_guile_names_mountlist __P((void));
extern SCM glibtop_guile_types_mountlist __P((void));
extern SCM glibtop_guile_labels_mountlist __P((void));
extern SCM glibtop_guile_descriptions_mountlist __P((void));

#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_mountlist [];
extern const unsigned glibtop_types_mountlist [];
extern const char *glibtop_labels_mountlist [];
extern const char *glibtop_descriptions_mountlist [];

#endif

__END_DECLS

#endif
