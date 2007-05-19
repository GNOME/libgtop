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

#ifndef __GLIBTOP_PROCLIST_H__
#define __GLIBTOP_PROCLIST_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_PROCLIST_NUMBER	0
#define GLIBTOP_PROCLIST_TOTAL	1
#define GLIBTOP_PROCLIST_SIZE	2

#define GLIBTOP_MAX_PROCLIST	3

/* You can use the folowing constants as the `which' member of
 * glibtop_get_proclist () to specify which processes to fetch. */

#define GLIBTOP_KERN_PROC_ALL		0	/* all processes */
#define GLIBTOP_KERN_PROC_PID		1
#define GLIBTOP_KERN_PROC_PGRP		2
#define GLIBTOP_KERN_PROC_SESSION	3
#define GLIBTOP_KERN_PROC_TTY		4
#define GLIBTOP_KERN_PROC_UID		5
#define GLIBTOP_KERN_PROC_RUID		6

#define GLIBTOP_KERN_PROC_MASK		15

#define GLIBTOP_EXCLUDE_IDLE		0x1000
#define GLIBTOP_EXCLUDE_SYSTEM		0x2000
#define GLIBTOP_EXCLUDE_NOTTY		0x4000

typedef struct _glibtop_proclist	glibtop_proclist;

struct _glibtop_proclist
{
	guint64	flags;
	guint64 number;			/* GLIBTOP_PROCLIST_NUMBER	*/
	guint64 total;			/* GLIBTOP_PROCLIST_TOTAL	*/
	guint64 size;			/* GLIBTOP_PROCLIST_SIZE	*/
};

pid_t*
glibtop_get_proclist(glibtop_proclist *buf, gint64 which, gint64 arg);

#if GLIBTOP_SUID_PROCLIST
#define glibtop_get_proclist_r		glibtop_get_proclist_p
#else
#define glibtop_get_proclist_r		glibtop_get_proclist_s
#endif

pid_t*
glibtop_get_proclist_l (glibtop *server, glibtop_proclist *buf,
			gint64 which, gint64 arg);

#if GLIBTOP_SUID_PROCLIST
void _glibtop_init_proclist_p (glibtop *server);

pid_t*
glibtop_get_proclist_p (glibtop *server, glibtop_proclist *buf,
			gint64 which, gint64 arg);
#else
void _glibtop_init_proclist_s (glibtop *server);

pid_t*
glibtop_get_proclist_s (glibtop *server, glibtop_proclist *buf,
			gint64 which, gint64 arg);
#endif


G_END_DECLS

#endif
