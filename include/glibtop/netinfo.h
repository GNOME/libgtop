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
#define GLIBTOP_NETINFO_MTU		1
#define GLIBTOP_NETINFO_SUBNET		2
#define GLIBTOP_NETINFO_ADDRESS		3

#define GLIBTOP_MAX_NETINFO		4

typedef struct _glibtop_netinfo	glibtop_netinfo;

#include <glibtop/interfaces.h>

enum {
    GLIBTOP_IF_FLAGS_UP = 1,
    GLIBTOP_IF_FLAGS_BROADCAST,
    GLIBTOP_IF_FLAGS_DEBUG,
    GLIBTOP_IF_FLAGS_LOOPBACK,
    GLIBTOP_IF_FLAGS_POINTOPOINT,
    GLIBTOP_IF_FLAGS_RUNNING,
    GLIBTOP_IF_FLAGS_NOARP,
    GLIBTOP_IF_FLAGS_PROMISC,
    GLIBTOP_IF_FLAGS_ALLMULTI,
    GLIBTOP_IF_FLAGS_OACTIVE,
    GLIBTOP_IF_FLAGS_SIMPLEX,
    GLIBTOP_IF_FLAGS_LINK0,
    GLIBTOP_IF_FLAGS_LINK1,
    GLIBTOP_IF_FLAGS_LINK2,
    GLIBTOP_IF_FLAGS_ALTPHYS,
    GLIBTOP_IF_FLAGS_MULTICAST
};

struct _glibtop_netinfo
{
    u_int64_t	flags,
	if_flags,		/* GLIBTOP_NETINFO_IF_FLAGS	*/
	mtu,			/* GLIBTOP_NETINFO_MTU		*/
	subnet,			/* GLIBTOP_NETINFO_SUBNET	*/
	address;		/* GLIBTOP_NETINFO_ADDRESS	*/
};

#define glibtop_get_netinfo(netinfo,interface)	glibtop_get_netinfo_l(glibtop_global_server, netinfo, interface)

#if GLIBTOP_SUID_NETINFO
#define glibtop_get_netinfo_r		glibtop_get_netinfo_p
#else
#define glibtop_get_netinfo_r		glibtop_get_netinfo_s
#endif

int glibtop_get_netinfo_l (glibtop *server, glibtop_netinfo *buf, const char *interface);

#if GLIBTOP_SUID_NETINFO
int glibtop_init_netinfo_p (glibtop *server);
int glibtop_get_netinfo_p (glibtop *server, glibtop_netinfo *buf, const char *interface);
#else
int glibtop_init_netinfo_s (glibtop *server);
int glibtop_get_netinfo_s (glibtop *server, glibtop_netinfo *buf, const char *interface);
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
