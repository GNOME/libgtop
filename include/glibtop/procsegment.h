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

#ifndef __GLIBTOP_PROCSEGMENT_H__
#define __GLIBTOP_PROCSEGMENT_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_PROC_SEGMENT_TEXT_RSS		0
#define GLIBTOP_PROC_SEGMENT_SHLIB_RSS		1
#define GLIBTOP_PROC_SEGMENT_DATA_RSS		2
#define GLIBTOP_PROC_SEGMENT_STACK_RSS		3
#define GLIBTOP_PROC_SEGMENT_DIRTY_SIZE		4
#define GLIBTOP_PROC_SEGMENT_START_CODE		5
#define GLIBTOP_PROC_SEGMENT_END_CODE		6
#define GLIBTOP_PROC_SEGMENT_START_STACK	7

#define GLIBTOP_MAX_PROC_SEGMENT		8

typedef struct _glibtop_proc_segment	glibtop_proc_segment;

/* Segment stuff */

struct _glibtop_proc_segment
{
	u_int64_t	flags,
		text_rss,	/* text resident set size */
		shlib_rss,	/* shared-lib resident set size */
		data_rss,	/* data resident set size */
		stack_rss,	/* stack resident set size */
		dirty_size,	/* size of dirty pages */
		start_code,
				/* address of beginning of code segment */
		end_code,	/* address of end of code segment */
		start_stack;	/* address of the bottom of stack segment */
};

#define glibtop_get_proc_segment(p1, p2)	glibtop_get_proc_segment_l(glibtop_global_server, p1, p2)

#if GLIBTOP_SUID_PROC_SEGMENT
#define glibtop_get_proc_segment_r	glibtop_get_proc_segment_p
#else
#define glibtop_get_proc_segment_r	glibtop_get_proc_segment_s
#endif

extern void glibtop_get_proc_segment_l __P((glibtop *, glibtop_proc_segment *, pid_t));

#if GLIBTOP_SUID_PROC_SEGMENT
extern void glibtop_init_proc_segment_p __P((glibtop *));
extern void glibtop_get_proc_segment_p __P((glibtop *, glibtop_proc_segment *, pid_t));
#else
extern void glibtop_init_proc_segment_s __P((glibtop *));
extern void glibtop_get_proc_segment_s __P((glibtop *, glibtop_proc_segment *, pid_t));
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_proc_segment [];
extern const unsigned glibtop_types_proc_segment [];
extern const char *glibtop_labels_proc_segment [];
extern const char *glibtop_descriptions_proc_segment [];

#endif

__END_DECLS

#endif
