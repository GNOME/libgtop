/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   LibGTop is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with LibGTop; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __GLIBTOP_PROCKERNEL_H__
#define __GLIBTOP_PROCKERNEL_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_PROC_KERNEL_K_FLAGS	0
#define GLIBTOP_PROC_KERNEL_MIN_FLT	1
#define GLIBTOP_PROC_KERNEL_MAJ_FLT	2
#define GLIBTOP_PROC_KERNEL_CMIN_FLT	3
#define GLIBTOP_PROC_KERNEL_CMAJ_FLT	4
#define GLIBTOP_PROC_KERNEL_KSTK_ESP	5
#define GLIBTOP_PROC_KERNEL_KSTK_EIP	6
#define GLIBTOP_PROC_KERNEL_NWCHAN	7
#define GLIBTOP_PROC_KERNEL_WCHAN	8

#define GLIBTOP_MAX_PROC_KERNEL		9

/* Constants for the `k_flags' field. */
#define GLIBTOP_KFLAGS_STARTING		1	/* being created */
#define GLIBTOP_KFLAGS_EXITING		2	/* getting shut down */
#define GLIBTOP_KFLAGS_PTRACED		4	/* set if ptrace (0) has
						   been called */
#define GLIBTOP_KFLAGS_TRACESYS		8	/* tracing system calls */
#define GLIBTOP_KFLAGS_FORKNOEXEC	16	/* forked but didn't exec */
#define GLIBTOP_KFLAGS_SUPERPRIV	32	/* used super-user privileges */
#define GLIBTOP_KFLAGS_DUMPEDCORE	64	/* dumped core */
#define GLIBTOP_KFLAGS_SIGNALED		128	/* killed by a signal */

typedef struct _glibtop_proc_kernel	glibtop_proc_kernel;

/* Kernel stuff */

struct _glibtop_proc_kernel
{
    u_int64_t flags;
    u_int64_t k_flags,	/* kernel flags for the process */
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
	nwchan;		/* address of kernel wait channel
			 * proc is sleeping in */
    char wchan [40];
};

#if GLIBTOP_SUID_PROC_KERNEL
#define glibtop_get_proc_kernel_r	glibtop_get_proc_kernel_p
#else
#define glibtop_get_proc_kernel_r	glibtop_get_proc_kernel_s
#endif

int glibtop_get_proc_kernel_l (glibtop_client *client, glibtop_proc_kernel *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_KERNEL
int glibtop_init_proc_kernel_p (glibtop *server);
int glibtop_get_proc_kernel_p (glibtop *server, void *closure, glibtop_proc_kernel *buf, pid_t pid);
#else
int glibtop_init_proc_kernel_s (glibtop_server *server);
int glibtop_get_proc_kernel_s (glibtop_server *server, void *closure, glibtop_proc_kernel *buf, pid_t pid);
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_proc_kernel [];
extern const unsigned glibtop_types_proc_kernel [];
extern const char *glibtop_labels_proc_kernel [];
extern const char *glibtop_descriptions_proc_kernel [];

#endif

END_LIBGTOP_DECLS

#endif
