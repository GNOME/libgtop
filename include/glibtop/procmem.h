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

#ifndef __GLIBTOP_PROCMEM_H__
#define __GLIBTOP_PROCMEM_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_PROC_MEM_SIZE		0
#define GLIBTOP_PROC_MEM_VSIZE		1
#define GLIBTOP_PROC_MEM_RESIDENT	2
#define GLIBTOP_PROC_MEM_SHARE		3
#define GLIBTOP_PROC_MEM_RSS		4
#define GLIBTOP_PROC_MEM_RSS_RLIM	5

#define GLIBTOP_MAX_PROC_MEM		6

typedef struct _glibtop_proc_mem	glibtop_proc_mem;

/* Memory section */

struct _glibtop_proc_mem
{
	u_int64_t	flags,
		size,		/* total # of pages of memory */
		vsize,		/* number of pages of virtual memory ... */
		resident,	/* number of resident set
				 * (non-swapped) pages (4k) */
		share,		/* number of pages of shared (mmap'd) memory */
		rss,		/* resident set size */
		rss_rlim;	/* current limit (in bytes) of the rss
				 * of the process; usually 2,147,483,647 */
};

#define glibtop_get_proc_mem(p1, p2)	glibtop_get_proc_mem_l(glibtop_global_server, p1, p2)

#if GLIBTOP_SUID_PROC_MEM
#define glibtop_get_proc_mem_r		glibtop_get_proc_mem_p
#else
#define glibtop_get_proc_mem_r		glibtop_get_proc_mem_s
#endif

extern void glibtop_get_proc_mem_l __P((glibtop *, glibtop_proc_mem *, pid_t));

#if GLIBTOP_SUID_PROC_MEM
extern void glibtop_init_proc_mem_p __P((glibtop *));
extern void glibtop_get_proc_mem_p __P((glibtop *, glibtop_proc_mem *, pid_t));
#else
extern void glibtop_init_proc_mem_s __P((glibtop *));
extern void glibtop_get_proc_mem_s __P((glibtop *, glibtop_proc_mem *, pid_t));
#endif

#ifdef GLIBTOP_GUILE

/* You need to link with -lgtop_guile to get this stuff here. */

extern SCM glibtop_guile_get_proc_mem __P((SCM));

#endif

#ifdef GLIBTOP_GUILE_NAMES

/* You need to link with -lgtop_guile_names to get this stuff here. */

extern SCM glibtop_guile_types_proc_mem __P((void));
extern SCM glibtop_guile_names_proc_mem __P((void));
extern SCM glibtop_guile_labels_proc_mem __P((void));
extern SCM glibtop_guile_descriptions_proc_mem __P((void));

#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_proc_mem [];
extern const unsigned glibtop_types_proc_mem [];
extern const char *glibtop_labels_proc_mem [];
extern const char *glibtop_descriptions_proc_mem [];

#endif

__END_DECLS

#endif
