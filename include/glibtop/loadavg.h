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

#ifndef __GLIBTOP_LOADAVG_H__
#define __GLIBTOP_LOADAVG_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_LOADAVG_LOADAVG		0
#define GLIBTOP_LOADAVG_NR_RUNNING	1
#define GLIBTOP_LOADAVG_NR_TASKS	2
#define GLIBTOP_LOADAVG_LAST_PID	3

#define GLIBTOP_MAX_LOADAVG		4

typedef struct _glibtop_loadavg	glibtop_loadavg;

struct _glibtop_loadavg
{
	guint64 flags;
	double loadavg [3];		/* GLIBTOP_LOADAVG_LOADAVG	*/
	guint64 nr_running;		/* GLIBTOP_LOADAVG_NR_RUNNING	*/
	guint64 nr_tasks;		/* GLIBTOP_LOADAVG_NR_TASKS	*/
	guint64 last_pid;		/* GLIBTOP_LOADAVG_LAST_PID	*/
};

void glibtop_get_loadavg(glibtop_loadavg *buf);

#if GLIBTOP_SUID_LOADAVG
#define glibtop_get_loadavg_r		glibtop_get_loadavg_p
#else
#define glibtop_get_loadavg_r		glibtop_get_loadavg_s
#endif

void glibtop_get_loadavg_l (glibtop *server, glibtop_loadavg *buf);

#if GLIBTOP_SUID_LOADAVG
void _glibtop_init_loadavg_p (glibtop *server);
void glibtop_get_loadavg_p (glibtop *server, glibtop_loadavg *buf);
#else
void _glibtop_init_loadavg_s (glibtop *server);
void glibtop_get_loadavg_s (glibtop *server, glibtop_loadavg *buf);
#endif


G_END_DECLS

#endif
