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

#ifndef __GLIBTOP_PROCTIME_H__
#define __GLIBTOP_PROCTIME_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_PROC_TIME_START_TIME	0
#define GLIBTOP_PROC_TIME_UTIME		1
#define GLIBTOP_PROC_TIME_STIME		2
#define GLIBTOP_PROC_TIME_CUTIME	3
#define GLIBTOP_PROC_TIME_CSTIME	4
#define GLIBTOP_PROC_TIME_TIMEOUT	5
#define GLIBTOP_PROC_TIME_IT_REAL_VALUE	6

#define GLIBTOP_MAX_PROC_TIME		7

typedef struct _glibtop_proc_time	glibtop_proc_time;

/* Time section */

struct _glibtop_proc_time
{
	u_int64_t	flags,
		start_time,	/* start time of process -- 
				 * seconds since 1-1-70 */
		utime,		/* user-mode CPU time accumulated by process */
		stime,		/* kernel-mode CPU time accumulated by process */
		cutime,		/* cumulative utime of process and 
				 * reaped children */
		cstime,		/* cumulative stime of process and 
				 * reaped children */
		timeout,	/* The time (in jiffies) of the process's
				 * next timeout */
		it_real_value;	/* The time (in jiffies) before the
				 * next SIGALRM is sent to the process
				 * due to an interval timer. */
};

#define glibtop_get_proc_time(p1, p2)	glibtop_get_proc_time_l(glibtop_global_server, p1, p2)

#if GLIBTOP_SUID_PROC_TIME
#define glibtop_get_proc_time_r	glibtop_get_proc_time_p
#else
#define glibtop_get_proc_time_r	glibtop_get_proc_time_s
#endif

extern void glibtop_get_proc_time_l __P((glibtop *, glibtop_proc_time *, pid_t));

#if GLIBTOP_SUID_PROC_TIME
extern void glibtop_get_proc_time_p __P((glibtop *, glibtop_proc_time *, pid_t));
#else
extern void glibtop_get_proc_time_s __P((glibtop *, glibtop_proc_time *, pid_t));
#endif

#ifdef GLIBTOP_GUILE

/* You need to link with -lgtop_guile to get this stuff here. */

extern SCM glibtop_guile_get_proc_time __P((SCM));

#endif

#ifdef GLIBTOP_GUILE_NAMES

/* You need to link with -lgtop_guile_names to get this stuff here. */

extern SCM glibtop_guile_names_proc_time __P((void));
extern SCM glibtop_guile_types_proc_time __P((void));
extern SCM glibtop_guile_labels_proc_time __P((void));
extern SCM glibtop_guile_descriptions_proc_time __P((void));

#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_proc_time [];
extern const unsigned glibtop_types_proc_time [];
extern const char *glibtop_labels_proc_time [];
extern const char *glibtop_descriptions_proc_time [];

#endif

__END_DECLS

#endif
