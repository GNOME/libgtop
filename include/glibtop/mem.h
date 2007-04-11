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

#ifndef __GLIBTOP_MEM_H__
#define __GLIBTOP_MEM_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_MEM_TOTAL	0
#define GLIBTOP_MEM_USED	1
#define GLIBTOP_MEM_FREE	2
#define GLIBTOP_MEM_SHARED	3
#define GLIBTOP_MEM_BUFFER	4
#define GLIBTOP_MEM_CACHED	5
#define GLIBTOP_MEM_USER	6
#define GLIBTOP_MEM_LOCKED	7

#define GLIBTOP_MAX_MEM		8

typedef struct _glibtop_mem	glibtop_mem;

struct _glibtop_mem
{
	guint64	flags;
	guint64 total;		/* GLIBTOP_MEM_TOTAL	*/
	guint64 used;		/* GLIBTOP_MEM_USED	*/
	guint64 free;		/* GLIBTOP_MEM_FREE	*/
	guint64 shared;		/* GLIBTOP_MEM_SHARED	*/
	guint64 buffer;		/* GLIBTOP_MEM_BUFFER	*/
	guint64 cached;		/* GLIBTOP_MEM_CACHED	*/
	guint64 user;		/* GLIBTOP_MEM_USER	*/
	guint64 locked;		/* GLIBTOP_MEM_LOCKED	*/
};

void glibtop_get_mem(glibtop_mem *buf);

#if GLIBTOP_SUID_MEM
#define glibtop_get_mem_r		glibtop_get_mem_p
#else
#define glibtop_get_mem_r		glibtop_get_mem_s
#endif

void glibtop_get_mem_l (glibtop *server, glibtop_mem *buf);

#if GLIBTOP_SUID_MEM
void _glibtop_init_mem_p (glibtop *server);
void glibtop_get_mem_p (glibtop *server, glibtop_mem *buf);
#else
void _glibtop_init_mem_s (glibtop *server);
void glibtop_get_mem_s (glibtop *server, glibtop_mem *buf);
#endif


G_END_DECLS

#endif
