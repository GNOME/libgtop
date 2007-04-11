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

#ifndef __GLIBTOP_NETLOAD_H__
#define __GLIBTOP_NETLOAD_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_NETLOAD_IF_FLAGS	0
#define GLIBTOP_NETLOAD_MTU		1
#define GLIBTOP_NETLOAD_SUBNET		2
#define GLIBTOP_NETLOAD_ADDRESS		3
#define GLIBTOP_NETLOAD_PACKETS_IN	4
#define GLIBTOP_NETLOAD_PACKETS_OUT	5
#define GLIBTOP_NETLOAD_PACKETS_TOTAL	6
#define GLIBTOP_NETLOAD_BYTES_IN	7
#define GLIBTOP_NETLOAD_BYTES_OUT	8
#define GLIBTOP_NETLOAD_BYTES_TOTAL	9
#define GLIBTOP_NETLOAD_ERRORS_IN	10
#define GLIBTOP_NETLOAD_ERRORS_OUT	11
#define GLIBTOP_NETLOAD_ERRORS_TOTAL	12
#define GLIBTOP_NETLOAD_COLLISIONS	13
#define GLIBTOP_NETLOAD_ADDRESS6	14
#define GLIBTOP_NETLOAD_PREFIX6		15
#define GLIBTOP_NETLOAD_SCOPE6		16
#define GLIBTOP_NETLOAD_HWADDRESS	17

#define GLIBTOP_MAX_NETLOAD		18

typedef struct _glibtop_netload	glibtop_netload;

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
	GLIBTOP_IF_FLAGS_MULTICAST,
	GLIBTOP_IF_FLAGS_WIRELESS
};

enum GLIBTOP_IF_IN6_SCOPE
{
	GLIBTOP_IF_IN6_SCOPE_UNKNOWN = 0,
	GLIBTOP_IF_IN6_SCOPE_LINK    = 1,
	GLIBTOP_IF_IN6_SCOPE_SITE    = 2,
	GLIBTOP_IF_IN6_SCOPE_GLOBAL  = 4,
	GLIBTOP_IF_IN6_SCOPE_HOST    = 8
};

struct _glibtop_netload
{
	guint64	flags;
	guint64 if_flags;		/* GLIBTOP_NETLOAD_IF_FLAGS	*/

	guint32	mtu;			/* GLIBTOP_NETLOAD_MTU		*/
	guint32 subnet;			/* GLIBTOP_NETLOAD_SUBNET	*/
	guint32 address;		/* GLIBTOP_NETLOAD_ADDRESS	*/

	guint64	packets_in;		/* GLIBTOP_NETLOAD_PACKETS_IN	*/
	guint64 packets_out;		/* GLIBTOP_NETLOAD_PACKETS_OUT	*/
	guint64 packets_total;		/* GLIBTOP_NETLOAD_PACKETS_TOTAL*/
	guint64 bytes_in;		/* GLIBTOP_NETLOAD_BYTES_IN	*/
	guint64 bytes_out;		/* GLIBTOP_NETLOAD_BYTES_OUT	*/
	guint64 bytes_total;		/* GLIBTOP_NETLOAD_BYTES_TOTAL	*/
	guint64 errors_in;		/* GLIBTOP_NETLOAD_ERRORS_IN	*/
	guint64 errors_out;		/* GLIBTOP_NETLOAD_ERRORS_OUT	*/
	guint64 errors_total;		/* GLIBTOP_NETLOAD_ERRORS_TOTAL	*/
	guint64 collisions;		/* GLIBTOP_NETLOAD_COLLISIONS	*/

	guint8 address6[16];		/* GLIBTOP_NETLOAD_ADDRESS6     */
	guint8 prefix6[16];		/* GLIBTOP_NETLOAD_PREXIF6      */
	guint8 scope6;			/* GLIBTOP_NETLOAD_SCOPE6       */

	guint8 hwaddress[8];		/* GLIBTOP_NETLOAD_HWADDRESS    */
};

void glibtop_get_netload(glibtop_netload *buf, const char *interface);

#if GLIBTOP_SUID_NETLOAD
#define glibtop_get_netload_r	glibtop_get_netload_p
#else
#define glibtop_get_netload_r	glibtop_get_netload_s
#endif

void glibtop_get_netload_l (glibtop *server, glibtop_netload *buf, const char *interface);

#if GLIBTOP_SUID_NETLOAD
void _glibtop_init_netload_p (glibtop *server);
void glibtop_get_netload_p (glibtop *server, glibtop_netload *buf, const char *interface);
#else
void _glibtop_init_netload_s (glibtop *server);
void glibtop_get_netload_s (glibtop *server, glibtop_netload *buf, const char *interface);
#endif


G_END_DECLS

#endif
