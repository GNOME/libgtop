/* Copyright (C) 2004 Benoît Dejean
   This file is part of LibGTop 2.0.

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

#ifndef __GLIBTOP_NETLIST_H__
#define __GLIBTOP_NETLIST_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_NETLIST_NUMBER		0

#define GLIBTOP_MAX_NETLIST		1

typedef struct _glibtop_netlist	glibtop_netlist;

struct _glibtop_netlist
{
	guint64 flags;
	guint32 number;
};

char** glibtop_get_netlist(glibtop_netlist *buf);

#if GLIBTOP_SUID_NETLIST
#define glibtop_get_netlist_r		glibtop_get_netlist_p
#else
#define glibtop_get_netlist_r		glibtop_get_netlist_s
#endif

char** glibtop_get_netlist_l (glibtop *server, glibtop_netlist *buf);

#if GLIBTOP_SUID_NETLIST
void _glibtop_init_netlist_p (glibtop *server);
char** glibtop_get_netlist_p (glibtop *server, glibtop_netlist *buf);
#else
void _glibtop_init_netlist_s (glibtop *server);
char** glibtop_get_netlist_s (glibtop *server, glibtop_netlist *buf);
#endif


G_END_DECLS

#endif
