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

#ifndef __GLIBTOP_PROCKERNEL_H__
#define __GLIBTOP_PROCKERNEL_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_PROC_KERNEL_K_FLAGS	0
#define GLIBTOP_PROC_KERNEL_MIN_FLT	1
#define GLIBTOP_PROC_KERNEL_MAJ_FLT	2
#define GLIBTOP_PROC_KERNEL_CMIN_FLT	3
#define GLIBTOP_PROC_KERNEL_CMAJ_FLT	4
#define GLIBTOP_PROC_KERNEL_KSTK_ESP	5
#define GLIBTOP_PROC_KERNEL_KSTK_EIP	6
#define GLIBTOP_PROC_KERNEL_WCHAN	7

#define GLIBTOP_MAX_PROC_KERNEL		8

typedef struct _glibtop_proc_kernel	glibtop_proc_kernel;

/* Kernel stuff */

struct _glibtop_proc_kernel
{
	unsigned long flags;
	unsigned long k_flags,	/* kernel flags for the process */
		min_flt,	/* number of minor page faults since
				 * process start */
		maj_flt,	/* number of major page faults since
				 * process start */
		cmin_flt,	/* cumulative min_flt of process and
				 * child processes */
		cmaj_flt,	/* cumulative maj_flt of process and
				 * child processes */
		kstk_esp,	/* kernel stack pointer */
		kstk_eip,	/* kernel stack pointer */
		wchan;		/* address of kernel wait channel
				 * proc is sleeping in */
};

#define glibtop_get_proc_kernel(p1, p2)	glibtop_get_proc_kernel__l(glibtop_global_server, p1, p2)

#if GLIBTOP_SUID_PROC_KERNEL
#define glibtop_get_proc_kernel__r	glibtop_get_proc_kernel__p
#else
#define glibtop_get_proc_kernel__r	glibtop_get_proc_kernel__s
#endif

extern void glibtop_get_proc_kernel__l __P((glibtop *, glibtop_proc_kernel *, pid_t));

#if GLIBTOP_SUID_PROC_KERNEL
extern void glibtop_get_proc_kernel__p __P((glibtop *, glibtop_proc_kernel *, pid_t));
#else
extern void glibtop_get_proc_kernel__s __P((glibtop *, glibtop_proc_kernel *, pid_t));
#endif

#ifdef HAVE_GUILE

/* You need to link with -lgtop_guile to get this stuff here. */

extern SCM glibtop_guile_get_proc_kernel __P((SCM));

#endif

#ifdef GLIBTOP_GUILE_NAMES

/* You need to link with -lgtop_guile_names to get this stuff here. */

extern SCM glibtop_guile_types_proc_kernel __P((void));
extern SCM glibtop_guile_names_proc_kernel __P((void));
extern SCM glibtop_guile_labels_proc_kernel __P((void));
extern SCM glibtop_guile_descriptions_proc_kernel __P((void));

#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_types_proc_kernel [];
extern const char *glibtop_names_proc_kernel [];
extern const char *glibtop_labels_proc_kernel [];
extern const char *glibtop_descriptions_proc_kernel [];

#endif

__END_DECLS

#endif
