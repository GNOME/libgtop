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

#ifndef __GLIBTOP_NETLOAD_H__
#define __GLIBTOP_NETLOAD_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_NETLOAD_MTU		0
#define GLIBTOP_NETLOAD_PACKETS_IN	1
#define GLIBTOP_NETLOAD_PACKETS_OUT	2
#define GLIBTOP_NETLOAD_PACKETS_TOTAL	3
#define GLIBTOP_NETLOAD_BYTES_IN	4
#define GLIBTOP_NETLOAD_BYTES_OUT	5
#define GLIBTOP_NETLOAD_BYTES_TOTAL	6
#define GLIBTOP_NETLOAD_ERRORS_IN	7
#define GLIBTOP_NETLOAD_ERRORS_OUT	8
#define GLIBTOP_NETLOAD_ERRORS_TOTAL	9
#define GLIBTOP_NETLOAD_COLLISIONS	10

#define GLIBTOP_MAX_NETLOAD		11

typedef struct _glibtop_netload	glibtop_netload;

struct _glibtop_netload
{
	u_int64_t	flags,
		mtu,			/* GLIBTOP_NETLOAD_MTU		*/
		address,		/* GLIBTOP_NETLOAD_ADDRESS	*/
		packets_in,		/* GLIBTOP_NETLOAD_PACKETS_IN	*/
		packets_out,		/* GLIBTOP_NETLOAD_PACKETS_OUT	*/
		packets_total,		/* GLIBTOP_NETLOAD_PACKETS_TOTAL*/
		bytes_in,		/* GLIBTOP_NETLOAD_BYTES_IN	*/
		bytes_out,		/* GLIBTOP_NETLOAD_BYTES_OUT	*/
		bytes_total,		/* GLIBTOP_NETLOAD_BYTES_TOTAL	*/
		errors_in,		/* GLIBTOP_NETLOAD_ERRORS_IN	*/
		errors_out,		/* GLIBTOP_NETLOAD_ERRORS_OUT	*/
		errors_total,		/* GLIBTOP_NETLOAD_ERRORS_TOTAL	*/
		collisions;		/* GLIBTOP_NETLOAD_COLLISIONS	*/
};

#define glibtop_get_netload(netload,interface)	glibtop_get_netload_l(glibtop_global_server, netload, interface)

#if GLIBTOP_SUID_NETLOAD
#define glibtop_get_netload_r	glibtop_get_netload_p
#else
#define glibtop_get_netload_r	glibtop_get_netload_s
#endif

extern void glibtop_get_netload_l __P((glibtop *, glibtop_netload *, const char *));

#if GLIBTOP_SUID_NETLOAD
extern void glibtop_init_netload_p __P((glibtop *));
extern void glibtop_get_netload_p __P((glibtop *, glibtop_netload *, const char *));
#else
extern void glibtop_init_netload_s __P((glibtop *));
extern void glibtop_get_netload_s __P((glibtop *, glibtop_netload *, const char *));
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_netload [];
extern const unsigned glibtop_types_netload [];
extern const char *glibtop_labels_netload [];
extern const char *glibtop_descriptions_netload [];

#endif

__END_DECLS

#endif
