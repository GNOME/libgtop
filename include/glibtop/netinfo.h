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

#ifndef __GLIBTOP_NETINFO_H__
#define __GLIBTOP_NETINFO_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_NETINFO_IF_FLAGS	0
#define GLIBTOP_NETINFO_TRANSPORT	1
#define GLIBTOP_NETINFO_MTU		2
#define GLIBTOP_NETINFO_SUBNET		3
#define GLIBTOP_NETINFO_ADDRESS		4

#define GLIBTOP_MAX_NETINFO		5

typedef struct _glibtop_netinfo	glibtop_netinfo;

#include <glibtop/interfaces.h>

struct _glibtop_netinfo
{
    u_int64_t	flags,
	if_flags,			/* GLIBTOP_NETINFO_IF_FLAGS	*/
	transport,			/* GLIBTOP_NETINFO_TRANSPORT	*/
	mtu;				/* GLIBTOP_NETINFO_MTU		*/
};

#if GLIBTOP_SUID_NETINFO
#define glibtop_get_netinfo_r		glibtop_get_netinfo_p
#else
#define glibtop_get_netinfo_r		glibtop_get_netinfo_s
#endif

glibtop_ifaddr *glibtop_get_netinfo_l (glibtop_client *client, glibtop_array *array, glibtop_netinfo *buf, const char *interface, u_int64_t transport);

#if GLIBTOP_SUID_NETINFO
int glibtop_init_netinfo_p (glibtop_server *server, glibtop_closure *closure);
glibtop_ifaddr *glibtop_get_netinfo_p (glibtop_server *server, glibtop_closure *closure, glibtop_array *array, glibtop_netinfo *buf, const char *interface, u_int64_t transport);
#else
int glibtop_init_netinfo_s (glibtop_server *server, glibtop_closure *closure);
glibtop_ifaddr *glibtop_get_netinfo_s (glibtop_server *server, glibtop_closure *closure, glibtop_array *array, glibtop_netinfo *buf, const char *interface, u_int64_t transport);
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_netinfo [];
extern const unsigned glibtop_types_netinfo [];
extern const char *glibtop_labels_netinfo [];
extern const char *glibtop_descriptions_netinfo [];

#endif

END_LIBGTOP_DECLS

#endif
