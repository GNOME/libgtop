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

#ifndef __GLIBTOP_MSG_LIMITS_H__
#define __GLIBTOP_MSG_LIMITS_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

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

void glibtop_get_msg_limits_l (glibtop *server, glibtop_msg_limits *buf);

#if GLIBTOP_SUID_MSG_LIMITS
void glibtop_init_msg_limits_p (glibtop *server);
void glibtop_get_msg_limits_p (glibtop *server, glibtop_msg_limits *buf);
#else
void glibtop_init_msg_limits_s (glibtop *server);
void glibtop_get_msg_limits_s (glibtop *server, glibtop_msg_limits *buf);
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_msg_limits [];
extern const unsigned glibtop_types_msg_limits [];
extern const char *glibtop_labels_msg_limits [];
extern const char *glibtop_descriptions_msg_limits [];

#endif

G_END_DECLS

#endif

