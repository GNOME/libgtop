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

#ifndef __GLIBTOP_PPP_H__
#define __GLIBTOP_PPP_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_PPP_STATE	0
#define GLIBTOP_PPP_BYTES_IN	1	
#define GLIBTOP_PPP_BYTES_OUT	2

#define GLIBTOP_MAX_PPP		3

typedef struct _glibtop_ppp	glibtop_ppp;

enum {
    GLIBTOP_PPP_STATE_UNKNOWN = 0,
    GLIBTOP_PPP_STATE_HANGUP,
    GLIBTOP_PPP_STATE_ONLINE
};

struct _glibtop_ppp
{
    u_int64_t	flags,
	state,			/* GLIBTOP_PPP_STATE		*/
	bytes_in,		/* GLIBTOP_PPP_BYTES_IN		*/
	bytes_out;		/* GLIBTOP_PPP_BYTES_OUT	*/
};

#define glibtop_get_ppp(ppp,device)	glibtop_get_ppp_l(glibtop_global_server, ppp, device)

#if GLIBTOP_SUID_PPP
#define glibtop_get_ppp_r	glibtop_get_ppp_p
#else
#define glibtop_get_ppp_r	glibtop_get_ppp_s
#endif

void glibtop_get_ppp_l (glibtop *server, glibtop_ppp *buf, unsigned short device);

#if GLIBTOP_SUID_PPP
void glibtop_init_ppp_p (glibtop *server);
void glibtop_get_ppp_p (glibtop *server, glibtop_ppp *buf, unsigned short device);
#else
void glibtop_init_ppp_s (glibtop *server);
void glibtop_get_ppp_s (glibtop *server, glibtop_ppp *buf, unsigned short device);
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_ppp [];
extern const unsigned glibtop_types_ppp [];
extern const char *glibtop_labels_ppp [];
extern const char *glibtop_descriptions_ppp [];

#endif

END_LIBGTOP_DECLS

#endif
