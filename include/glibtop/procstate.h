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

#ifndef __GLIBTOP_PROCSTATE_H__
#define __GLIBTOP_PROCSTATE_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_PROC_STATE_CMD		0
#define GLIBTOP_PROC_STATE_STATE	1
#define GLIBTOP_PROC_STATE_UID		2
#define GLIBTOP_PROC_STATE_GID		3

#define GLIBTOP_MAX_PROC_STATE		4

typedef struct _glibtop_proc_state	glibtop_proc_state;

/* Name and status */

struct _glibtop_proc_state
{
	unsigned long flags;
	char cmd[40],		/* basename of executable file in 
				 * call to exec(2) */
		state;		/* single-char code for process state
				 * (S=sleeping) */
	/* NOTE: when porting the library, TRY HARD to implement the
	 *       following two fields. */
	/* IMPORTANT NOTICE: For security reasons, it is extremely important
	 *                   only to set the flags value for those two
	 *                   fields if their values are corrent ! */
	int uid,		/* UID of process */
		gid;		/* GID of process */
};

#define glibtop_get_proc_state(p1, p2)	glibtop_get_proc_state_l(glibtop_global_server, p1, p2)

#if GLIBTOP_SUID_PROC_STATE
#define glibtop_get_proc_state_r	glibtop_get_proc_state_p
#else
#define glibtop_get_proc_state_r	glibtop_get_proc_state_s
#endif

extern void glibtop_get_proc_state_l __P((glibtop *, glibtop_proc_state *, pid_t));

#if GLIBTOP_SUID_PROC_STATE
extern void glibtop_get_proc_state_p __P((glibtop *, glibtop_proc_state *, pid_t));
#else
extern void glibtop_get_proc_state_s __P((glibtop *, glibtop_proc_state *, pid_t));
#endif
     
#ifdef HAVE_GUILE

/* You need to link with -lgtop_guile to get this stuff here. */

extern SCM glibtop_guile_get_proc_state __P((SCM));

#endif

#ifdef GLIBTOP_GUILE_NAMES

/* You need to link with -lgtop_guile_names to get this stuff here. */

extern SCM glibtop_guile_names_proc_state __P((void));
extern SCM glibtop_guile_types_proc_state __P((void));
extern SCM glibtop_guile_labels_proc_state __P((void));
extern SCM glibtop_guile_descriptions_proc_state __P((void));

#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_proc_state [];
extern const unsigned glibtop_types_proc_state [];
extern const char *glibtop_labels_proc_state [];
extern const char *glibtop_descriptions_proc_state [];

#endif

__END_DECLS

#endif
