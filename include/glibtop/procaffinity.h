/* Copyright (C) 2007 Benoît Dejean
   This file is part of LibGTop 2.

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

#ifndef __GLIBTOP_PROCAFFINITY_H__
#define __GLIBTOP_PROCAFFINITY_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

typedef struct _glibtop_proc_affinity	glibtop_proc_affinity;

#define GLIBTOP_PROC_AFFINITY_NUMBER	0
#define GLIBTOP_PROC_AFFINITY_ALL	1

struct _glibtop_proc_affinity
{
	guint64	flags;
	guint32 number;
	gboolean all;
};


guint16 * glibtop_get_proc_affinity(glibtop_proc_affinity *buf, pid_t pid);
guint16 * glibtop_get_proc_affinity_l(glibtop *server, glibtop_proc_affinity *buf, pid_t pid);


#if GLIBTOP_SUID_PROC_AFFINITY

#define glibtop_get_proc_affinity_r glibtop_get_proc_affinity_p
void _glibtop_init_proc_affinity_p(glibtop *server);
guint16 * glibtop_get_proc_affinity_p(glibtop *server, glibtop_proc_affinity *buf, pid_t pid);

#else

#define glibtop_get_proc_affinity_r glibtop_get_proc_affinity_s
void _glibtop_init_proc_affinity_s(glibtop *server);
guint16 * glibtop_get_proc_affinity_s(glibtop *server, glibtop_proc_affinity *buf, pid_t pid);

#endif

G_END_DECLS

#endif
