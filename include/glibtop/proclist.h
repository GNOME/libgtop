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

#ifndef __GLIBTOP_PROCLIST_H__
#define __GLIBTOP_PROCLIST_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_PROCLIST_NUMBER	0
#define GLIBTOP_PROCLIST_TOTAL	1
#define GLIBTOP_PROCLIST_SIZE	2

#define GLIBTOP_MAX_PROCLIST	3

/* You can use the folowing constants as the `which' member of
 * glibtop_get_proclist () to specify which processes to fetch. */

#define GLIBTOP_KERN_PROC_ALL		0	/* all processes */
#define GLIBTOP_KERN_PROC_PID		1
#define GLIBTOP_KERN_PROC_PGRP		2
#define GLIBTOP_KERN_PROC_SESSION	3
#define GLIBTOP_KERN_PROC_TTY		4
#define GLIBTOP_KERN_PROC_UID		5
#define GLIBTOP_KERN_PROC_RUID		6

#define GLIBTOP_KERN_PROC_MASK		15

#define GLIBTOP_EXCLUDE_IDLE		0x1000
#define GLIBTOP_EXCLUDE_SYSTEM		0x2000
#define GLIBTOP_EXCLUDE_NOTTY		0x4000

typedef struct _glibtop_proclist	glibtop_proclist;

struct _glibtop_proclist
{
	u_int64_t	flags,
		number,			/* GLIBTOP_PROCLIST_NUMBER	*/
		total,			/* GLIBTOP_PROCLIST_TOTAL	*/
		size;			/* GLIBTOP_PROCLIST_SIZE	*/
};

#define glibtop_get_proclist(proclist,which,arg) glibtop_get_proclist_l(glibtop_global_server, proclist, which, arg)

#if GLIBTOP_SUID_PROCLIST
#define glibtop_get_proclist_r		glibtop_get_proclist_p
#else
#define glibtop_get_proclist_r		glibtop_get_proclist_s
#endif

extern unsigned *glibtop_get_proclist_l __P((glibtop *, glibtop_proclist *, int64_t, int64_t));

#if GLIBTOP_SUID_PROCLIST
extern void glibtop_init_proclist_p __P((glibtop *));
extern unsigned *glibtop_get_proclist_p __P((glibtop *, glibtop_proclist *, int64_t, int64_t));
#else
extern void glibtop_init_proclist_s __P((glibtop *));
extern unsigned *glibtop_get_proclist_s __P((glibtop *, glibtop_proclist *, int64_t, int64_t));
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_proclist [];
extern const unsigned glibtop_types_proclist [];
extern const char *glibtop_labels_proclist [];
extern const char *glibtop_descriptions_proclist [];

#endif

__END_DECLS

#endif
