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

#ifndef __GLIBTOP_UPTIME_H__
#define __GLIBTOP_UPTIME_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_UPTIME_UPTIME		0
#define GLIBTOP_UPTIME_IDLETIME		1
#define GLIBTOP_UPTIME_BOOT_TIME	2

#define GLIBTOP_MAX_UPTIME		3

typedef struct _glibtop_uptime	glibtop_uptime;

struct _glibtop_uptime
{
    u_int64_t flags;
    double uptime,		/* GLIBTOP_UPTIME_UPTIME	*/
	idletime;		/* GLIBTOP_UPTIME_IDLETIME	*/
    u_int64_t boot_time;	/* GLIBTOP_UPTIME_BOOT_TIME	*/
};

#if GLIBTOP_SUID_UPTIME
#define glibtop_get_uptime_r		glibtop_get_uptime_p
#else
#define glibtop_get_uptime_r		glibtop_get_uptime_s
#endif

int glibtop_get_uptime_l (glibtop_client *client, glibtop_uptime *buf);

#if GLIBTOP_SUID_UPTIME
int glibtop_init_uptime_p (glibtop *server);
int glibtop_get_uptime_p (glibtop *server, void *closure, glibtop_uptime *buf);
#else
int glibtop_init_uptime_s (glibtop_server *server);
int glibtop_get_uptime_s (glibtop_server *server, void *closure, glibtop_uptime *buf);
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_uptime [];
extern const unsigned glibtop_types_uptime [];
extern const char *glibtop_labels_uptime [];
extern const char *glibtop_descriptions_uptime [];

#endif

END_LIBGTOP_DECLS

#endif
