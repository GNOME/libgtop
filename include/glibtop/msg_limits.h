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

#ifndef __GLIBTOP_MSG_LIMITS_H__
#define __GLIBTOP_MSG_LIMITS_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_IPC_MSGPOOL	0
#define GLIBTOP_IPC_MSGMAP	1
#define GLIBTOP_IPC_MSGMAX	2
#define GLIBTOP_IPC_MSGMNB	3
#define GLIBTOP_IPC_MSGMNI	4
#define GLIBTOP_IPC_MSGSSZ	5
#define GLIBTOP_IPC_MSGTQL	6

#define GLIBTOP_MAX_MSG_LIMITS	7

typedef struct _glibtop_msg_limits	glibtop_msg_limits;

struct _glibtop_msg_limits
{
	u_int64_t	flags,
		msgpool,	/* GLIBTOP_IPC_MSGPOOL	*/
		msgmap,		/* GLIBTOP_IPC_MSGMAP	*/
		msgmax,		/* GLIBTOP_IPC_MSGMAX	*/
		msgmnb,		/* GLIBTOP_IPC_MSGMNB	*/
		msgmni,		/* GLIBTOP_IPC_MSGMNI	*/
		msgssz,		/* GLIBTOP_IPC_MSGSSZ	*/
		msgtql;		/* GLIBTOP_IPC_MSGTQL	*/
};

#define glibtop_get_msg_limits(msg)	glibtop_get_msg_limits_l(glibtop_global_server, msg)

#if GLIBTOP_SUID_MSG_LIMITS
#define glibtop_get_msg_limits_r	glibtop_get_msg_limits_p
#else
#define glibtop_get_msg_limits_r	glibtop_get_msg_limits_s
#endif

extern void glibtop_get_msg_limits_l __P((glibtop *, glibtop_msg_limits *));

#if GLIBTOP_SUID_MSG_LIMITS
extern void glibtop_init_msg_limits_p __P((glibtop *));
extern void glibtop_get_msg_limits_p __P((glibtop *, glibtop_msg_limits *));
#else
extern void glibtop_init_msg_limits_s __P((glibtop *));
extern void glibtop_get_msg_limits_s __P((glibtop *, glibtop_msg_limits *));
#endif

#ifdef GLIBTOP_GUILE

/* You need to link with -lgtop_guile to get this stuff here. */

extern SCM glibtop_guile_get_msg_limits __P((void));

#endif

#ifdef GLIBTOP_GUILE_NAMES

/* You need to link with -lgtop_guile_names to get this stuff here. */

extern SCM glibtop_guile_names_msg_limits __P((void));
extern SCM glibtop_guile_types_msg_limits __P((void));
extern SCM glibtop_guile_labels_msg_limits __P((void));
extern SCM glibtop_guile_descriptions_msg_limits __P((void));

#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_msg_limits [];
extern const unsigned glibtop_types_msg_limits [];
extern const char *glibtop_labels_msg_limits [];
extern const char *glibtop_descriptions_msg_limits [];

#endif

__END_DECLS

#endif
