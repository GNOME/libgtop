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

#ifndef __GLIBTOP_FSUSAGE_H__
#define __GLIBTOP_FSUSAGE_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_FSUSAGE_BLOCKS		0
#define GLIBTOP_FSUSAGE_BFREE		1
#define GLIBTOP_FSUSAGE_BAVAIL		2
#define GLIBTOP_FSUSAGE_FILES		3
#define GLIBTOP_FSUSAGE_FFREE		4

#define GLIBTOP_MAX_FSUSAGE		5

typedef struct _glibtop_fsusage		glibtop_fsusage;

struct _glibtop_fsusage
{
	u_int64_t	flags,
		blocks,		/* Total blocks. */
		bfree,		/* Free blocks available to superuser. */
		bavail,		/* Free blocks available to non-superuser. */
		files,		/* Total file nodes. */
		ffree;		/* Free file nodes. */
};

#define glibtop_get_fsusage(fsusage,disk)	glibtop_get_fsusage_l(glibtop_global_server, fsusage, disk)

#define glibtop_get_fsusage_r		glibtop_get_fsusage_s

extern void glibtop_get_fsusage_l __P((glibtop *, glibtop_fsusage *, const char *));

extern void glibtop_get_fsusage_s __P((glibtop *, glibtop_fsusage *, const char *));

#ifdef GLIBTOP_GUILE

/* You need to link with -lgtop_guile to get this stuff here. */

extern SCM glibtop_guile_get_fsusage __P((SCM));

#endif

#ifdef GLIBTOP_GUILE_NAMES

/* You need to link with -lgtop_guile_names to get this stuff here. */

extern SCM glibtop_guile_names_fsusage __P((void));
extern SCM glibtop_guile_types_fsusage __P((void));
extern SCM glibtop_guile_labels_fsusage __P((void));
extern SCM glibtop_guile_descriptions_fsusage __P((void));

#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_fsusage [];
extern const unsigned glibtop_types_fsusage [];
extern const char *glibtop_labels_fsusage [];
extern const char *glibtop_descriptions_fsusage [];

#endif

__END_DECLS

#endif
