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

#ifndef __GLIBTOP_PROC_ARGS_H__
#define __GLIBTOP_PROC_ARGS_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_PROC_ARGS_SIZE		0

#define GLIBTOP_MAX_PROC_ARGS		1

typedef struct _glibtop_proc_args	glibtop_proc_args;

struct _glibtop_proc_args
{
	u_int64_t	flags,
		size;			/* GLIBTOP_PROC_ARGS_SIZE	*/
};

#define glibtop_get_proc_args(proc_args,pid,max_len) glibtop_get_proc_args_l(glibtop_global_server, proc_args, pid, max_len)

#if GLIBTOP_SUID_PROC_ARGS
#define glibtop_get_proc_args_r		glibtop_get_proc_args_p
#else
#define glibtop_get_proc_args_r		glibtop_get_proc_args_s
#endif

extern const char  *glibtop_get_proc_args_l __P((glibtop *, glibtop_proc_args *, pid_t, unsigned));

#if GLIBTOP_SUID_PROC_ARGS
extern void glibtop_init_proc_args_p __P((glibtop *));
extern const char *glibtop_get_proc_args_p __P((glibtop *, glibtop_proc_args *, pid_t, unsigned));
#else
extern void glibtop_init_proc_args_s __P((glibtop *));
extern const char *glibtop_get_proc_args_s __P((glibtop *, glibtop_proc_args *, pid_t, unsigned));
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_proc_args [];
extern const unsigned glibtop_types_proc_args [];
extern const char *glibtop_labels_proc_args [];
extern const char *glibtop_descriptions_proc_args [];

#endif

__END_DECLS

#endif
