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

#ifndef __GLIBTOP_PROCSIGNAL_H__
#define __GLIBTOP_PROCSIGNAL_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_PROC_SIGNAL_SIGNAL	0
#define GLIBTOP_PROC_SIGNAL_BLOCKED	1
#define GLIBTOP_PROC_SIGNAL_SIGIGNORE	2
#define GLIBTOP_PROC_SIGNAL_SIGCATCH	3

#define GLIBTOP_MAX_PROC_SIGNAL		4

typedef struct _glibtop_proc_signal	glibtop_proc_signal;

/* Signals section */

struct _glibtop_proc_signal
{
	guint64	flags;
	guint64 signal [2];		/* mask of pending signals */
	guint64 blocked [2];		/* mask of blocked signals */
	guint64 sigignore [2];		/* mask of ignored signals */
	guint64 sigcatch [2];		/* mask of caught  signals */
};

void glibtop_get_proc_signal(glibtop_proc_signal *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_SIGNAL
#define glibtop_get_proc_signal_r	glibtop_get_proc_signal_p
#else
#define glibtop_get_proc_signal_r	glibtop_get_proc_signal_s
#endif

void glibtop_get_proc_signal_l (glibtop *server, glibtop_proc_signal *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_SIGNAL
void _glibtop_init_proc_signal_p (glibtop *server);
void glibtop_get_proc_signal_p (glibtop *server, glibtop_proc_signal *buf, pid_t pid);
#else
void _glibtop_init_proc_signal_s (glibtop *server);
void glibtop_get_proc_signal_s (glibtop *server, glibtop_proc_signal *buf, pid_t pid);
#endif


G_END_DECLS

#endif
