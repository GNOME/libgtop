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

#include <glibtop/compat_10.h>
#include <glibtop/array.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_INTERFACE_IF_FLAGS	0
#define GLIBTOP_INTERFACE_TRANSPORT	1
#define GLIBTOP_INTERFACE_TYPE		2
#define GLIBTOP_INTERFACE_NUMBER	3
#define GLIBTOP_INTERFACE_INSTANCE	4
#define GLIBTOP_INTERFACE_NUM_LOGICAL	5
#define GLIBTOP_INTERFACE_NAME		6

#define GLIBTOP_MAX_INTERFACE		7

#define GLIBTOP_IFADDR_TRANSPORT	0
#define GLIBTOP_IFADDR_ADDR_LEN		1
#define GLIBTOP_IFADDR_ADDRESS		2
#define GLIBTOP_IFADDR_SUBNET		3
#define GLIBTOP_IFADDR_SCOPE		4

#define GLIBTOP_MAX_IFADDR		5

typedef struct _glibtop_interface	glibtop_interface;
typedef struct _glibtop_ifaddr		glibtop_ifaddr;

typedef enum _glibtop_interface_type	glibtop_interface_type;
typedef enum _glibtop_transport		glibtop_transport;
typedef enum _glibtop_protocol		glibtop_protocol;

typedef enum _glibtop_ipv6_scope	glibtop_ipv6_scope;

typedef enum _glibtop_strategy		glibtop_strategy;
typedef enum _glibtop_interface_flags	glibtop_interface_flags;

enum _glibtop_protocol {
    GLIBTOP_PROTOCOL_OTHERS	= 0,
    GLIBTOP_PROTOCOL_ICMP	= 1 << 0,
    GLIBTOP_PROTOCOL_TCP	= 1 << 1,
    GLIBTOP_PROTOCOL_UDP	= 1 << 2,
    GLIBTOP_PROTOCOL_IGMP	= 1 << 3,
    GLIBTOP_PROTOCOL_RAW	= 1 << 4,
    GLIBTOP_PROTOCOL_ICMP6	= 1 << 5,
    GLIBTOP_PROTOCOL_TCP6	= 1 << 6,
    GLIBTOP_PROTOCOL_UDP6	= 1 << 7,
    GLIBTOP_PROTOCOL_IGMP6	= 1 << 8,
    GLIBTOP_PROTOCOL_RAW6	= 1 << 9
};

enum _glibtop_transport {
    GLIBTOP_TRANSPORT_DEFAULT	= 0,
    GLIBTOP_TRANSPORT_IPV4	= 1 << 0,
    GLIBTOP_TRANSPORT_IPV6	= 1 << 1,
    GLIBTOP_TRANSPORT_IPX	= 1 << 2,
    GLIBTOP_TRANSPORT_X25	= 1 << 3,
    GLIBTOP_TRANSPORT_DECNET	= 1 << 4,
    GLIBTOP_TRANSPORT_APPLETALK	= 1 << 5,
    GLIBTOP_TRANSPORT_NETBEUI	= 1 << 6
};

#define GLIBTOP_TRANSPORT_ALL	GLIBTOP_UNLIMITED
#define GLIBTOP_PROTOCOL_ALL	GLIBTOP_UNLIMITED

enum _glibtop_interface_type {
    GLIBTOP_INTERFACE_ALL		= 0,
    GLIBTOP_INTERFACE_OTHER,
    /* Network interfaces */
    GLIBTOP_INTERFACE_ETHERNET		= 101,
    GLIBTOP_INTERFACE_FDDI,
    GLIBTOP_INTERFACE_FRAME_RELAY,
    GLIBTOP_INTERFACE_TOKEN_RING,
    GLIBTOP_INTERFACE_ARCNET,
    GLIBTOP_INTERFACE_ATM,
    /* Dummy interfaces */
    GLIBTOP_INTERFACE_LOOPBACK		= 501,
    GLIBTOP_INTERFACE_DUMMY,
    /* Dialup interfaces */
    GLIBTOP_INTERFACE_PARA		= 801,
    GLIBTOP_INTERFACE_SLIP,
    GLIBTOP_INTERFACE_PPP,
    GLIBTOP_INTERFACE_ISDN
};

enum _glibtop_ipv6_scope {
    GLIBTOP_IPV6_SCOPE_GLOBAL		= 0,
    GLIBTOP_IPV6_SCOPE_LOOPBACK		= 1 << 1,
    GLIBTOP_IPV6_SCOPE_LINKLOCAL	= 1 << 2,
    GLIBTOP_IPV6_SCOPE_SITELOCAL	= 1 << 3,
    GLIBTOP_IPV6_SCOPE_COMPATv4		= 1 << 4,
    GLIBTOP_IPV6_SCOPE_UNKNOWN		= 1 << 5
};

enum _glibtop_strategy {
    GLIBTOP_INTERFACES_ALL		= 0,
    GLIBTOP_INTERFACES_BEST_MATCHING,
    GLIBTOP_INTERFACES_ONLY_THIS_ONE,
    GLIBTOP_INTERFACES_INCLUDE_LOGICAL	= 1 << 7
};

#define GLIBTOP_STRATEGY_FLAGS_MASK	(1 << 7)

enum _glibtop_interface_flags {
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

struct _glibtop_ifaddr
{
    u_int64_t	flags,
	transport;			/* GLIBTOP_IFADDR_TRANSPORT	*/
    u_int8_t	addr_len,		/* GLIBTOP_IFADDR_ADDR_LEN	*/
	address [GLIBTOP_IFADDR_LEN];	/* GLIBTOP_IFADDR_ADDRESS	*/
    u_int64_t	subnet,			/* GLIBTOP_IFADDR_SUBNET	*/
	scope;				/* GLIBTOP_IFADDR_SCOPE	*/
};

struct _glibtop_interface
{
    u_int64_t	flags,
	if_flags,		/* GLIBTOP_INTERFACE_IF_FLAGS		*/
	transport,		/* GLIBTOP_INTERFACE_TRANSPORT		*/
	type,			/* GLIBTOP_INTERFACE_TYPE		*/
	number,			/* GLIBTOP_INTERFACE_NUMBER		*/
	instance,		/* GLIBTOP_INTERFACE_INSTANCE		*/
	num_logical;		/* GLIBTOP_INTERFACE_NUM_LOGICAL	*/
    char name [GLIBTOP_INTERFACE_LEN];
};

#if GLIBTOP_SUID_INTERFACE_NAMES
#define glibtop_get_interface_names_r	glibtop_get_interface_names_p
#else
#define glibtop_get_interface_names_r	glibtop_get_interface_names_s
#endif

glibtop_interface *glibtop_get_interface_names_l (glibtop_client *client, glibtop_array *array, u_int64_t interface, u_int64_t number, u_int64_t instance, u_int64_t strategy);

#if GLIBTOP_SUID_INTERFACE_NAMES
int glibtop_init_interface_names_p (glibtop *server);
glibtop_interface *glibtop_get_interface_names_p (glibtop *server, glibtop_array *array, u_int64_t interface, u_int64_t number, u_int64_t instance, u_int64_t strategy);
#else
int glibtop_init_interface_names_s (glibtop_server *server);
glibtop_interface *glibtop_get_interface_names_s (glibtop_server *server, glibtop_array *array, u_int64_t interface, u_int64_t number, u_int64_t instance, u_int64_t strategy);
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
