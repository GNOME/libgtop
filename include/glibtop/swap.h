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

#ifndef __GLIBTOP_SWAP_H__
#define __GLIBTOP_SWAP_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_SWAP_TOTAL	0
#define GLIBTOP_SWAP_USED	1
#define GLIBTOP_SWAP_FREE	2
#define GLIBTOP_SWAP_PAGEIN	3
#define GLIBTOP_SWAP_PAGEOUT	4

#define GLIBTOP_MAX_SWAP	5

typedef struct _glibtop_swap	glibtop_swap;

struct _glibtop_swap
{
	guint64	flags;
	guint64 total;		/* GLIBTOP_SWAP_TOTAL	*/
	guint64 used;		/* GLIBTOP_SWAP_USED	*/
	guint64 free;		/* GLIBTOP_SWAP_FREE	*/
	guint64 pagein;		/* GLIBTOP_SWAP_PAGEIN	*/
	guint64 pageout;	/* GLIBTOP_SWAP_PAGEOUT	*/
};

void
glibtop_get_swap(glibtop_swap *buf);

#if GLIBTOP_SUID_SWAP
#define glibtop_get_swap_r		glibtop_get_swap_p
#else
#define glibtop_get_swap_r		glibtop_get_swap_s
#endif

void glibtop_get_swap_l (glibtop *server, glibtop_swap *buf);

#if GLIBTOP_SUID_SWAP
void _glibtop_init_swap_p (glibtop *server);
void glibtop_get_swap_p (glibtop *server, glibtop_swap *buf);
#else
void _glibtop_init_swap_s (glibtop *server);
void glibtop_get_swap_s (glibtop *server, glibtop_swap *buf);
#endif


G_END_DECLS

#endif
