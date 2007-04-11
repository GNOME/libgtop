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

G_BEGIN_DECLS

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
	guint64	flags;
	guint64 state;			/* GLIBTOP_PPP_STATE		*/
	guint64 bytes_in;		/* GLIBTOP_PPP_BYTES_IN		*/
	guint64 bytes_out;		/* GLIBTOP_PPP_BYTES_OUT	*/
};

void glibtop_get_ppp(glibtop_ppp *buf, unsigned short device);

#if GLIBTOP_SUID_PPP
#define glibtop_get_ppp_r	glibtop_get_ppp_p
#else
#define glibtop_get_ppp_r	glibtop_get_ppp_s
#endif

void glibtop_get_ppp_l (glibtop *server, glibtop_ppp *buf, unsigned short device);

#if GLIBTOP_SUID_PPP
void _glibtop_init_ppp_p (glibtop *server);
void glibtop_get_ppp_p (glibtop *server, glibtop_ppp *buf, unsigned short device);
#else
void _glibtop_init_ppp_s (glibtop *server);
void glibtop_get_ppp_s (glibtop *server, glibtop_ppp *buf, unsigned short device);
#endif


G_END_DECLS

#endif
