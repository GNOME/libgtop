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

#ifndef __GLIBTOP_INTERFACES_H__
#define __GLIBTOP_INTERFACES_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_INTERFACE_NAMES_NUMBER	0
#define GLIBTOP_INTERFACE_NAMES_SIZE	1

#define GLIBTOP_MAX_INTERFACE_NAMES	2

typedef struct _glibtop_interface_names	glibtop_interface_names;

typedef enum _glibtop_protocol		glibtop_protocol;
typedef enum _glibtop_interface		glibtop_interface;

enum _glibtop_protocol {
    GLIBTOP_NETLOAD_DEFAULT = 0,
    GLIBTOP_NETLOAD_IPV4,
    GLIBTOP_NETLOAD_IPV6,
    GLIBTOP_NETLOAD_IPX,
};

enum _glibtop_interface {
    GLIBTOP_INTERFACE_ALL = 0,
    GLIBTOP_INTERFACE_ETHERNET,
    GLIBTOP_INTERFACE_LOOPBACK,
    GLIBTOP_INTERFACE_DUMMY,
    GLIBTOP_INTERFACE_PPP,
};

enum {
    GLIBTOP_INTERFACES_ALL = 0,
    GLIBTOP_INTERFACES_BEST_MATCHING,
};

struct _glibtop_interface_names
{
    u_int64_t	flags,
	number,			/* GLIBTOP_INTERFACES_NUMBER	*/
	size;			/* GLIBTOP_INTERFACES_SIZE	*/
};

#define glibtop_get_interface_names(buf, interface,number,strategy)	glibtop_get_interface_names_l(glibtop_global_server, buf, interface, number, strategy)

#if GLIBTOP_SUID_INTERFACE_NAMES
#define glibtop_get_interface_names_r	glibtop_get_interface_names_p
#else
#define glibtop_get_interface_names_r	glibtop_get_interface_names_s
#endif

char *glibtop_get_interface_names_l (glibtop *server, glibtop_interface_names *buf, unsigned interface, unsigned number, unsigned strategy);

#if GLIBTOP_SUID_INTERFACES
int glibtop_init_interface_names_p (glibtop *server);
char *glibtop_get_interface_names_p (glibtop *server, glibtop_interface_names *buf, unsigned interface, unsigned number, unsigned strategy);
#else
int glibtop_init_interface_names_s (glibtop *server);
char *glibtop_get_interface_names_s (glibtop *server, glibtop_interface_names *buf, unsigned interface, unsigned number, unsigned strategy);
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_interface_names [];
extern const unsigned glibtop_types_interface_names [];
extern const char *glibtop_labels_interface_names [];
extern const char *glibtop_descriptions_interface_names [];

#endif

END_LIBGTOP_DECLS

#endif
