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

#ifndef __GLIBTOP_PROCSIGNAL_H__
#define __GLIBTOP_PROCSIGNAL_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_PROC_SIGNAL_SIGNAL	0
#define GLIBTOP_PROC_SIGNAL_BLOCKED	1
#define GLIBTOP_PROC_SIGNAL_SIGIGNORE	2
#define GLIBTOP_PROC_SIGNAL_SIGCATCH	3

#define GLIBTOP_MAX_PROC_SIGNAL		4

typedef struct _glibtop_proc_signal	glibtop_proc_signal;

/* Signals section */

struct _glibtop_proc_signal
{
    u_int64_t	flags,
	signal [2],		/* mask of pending signals */
	blocked [2],		/* mask of blocked signals */
	sigignore [2],		/* mask of ignored signals */
	sigcatch [2];		/* mask of caught  signals */
};

#if GLIBTOP_SUID_PROC_SIGNAL
#define glibtop_get_proc_signal_r	glibtop_get_proc_signal_p
#else
#define glibtop_get_proc_signal_r	glibtop_get_proc_signal_s
#endif

int glibtop_get_proc_signal_l (glibtop_client *client, glibtop_proc_signal *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_SIGNAL
int glibtop_init_proc_signal_p (glibtop *server);
int glibtop_get_proc_signal_p (glibtop *server, glibtop_proc_signal *buf, pid_t pid);
#else
int glibtop_init_proc_signal_s (glibtop_server *server);
int glibtop_get_proc_signal_s (glibtop_server *server, glibtop_proc_signal *buf, pid_t pid);
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_proc_signal [];
extern const unsigned glibtop_types_proc_signal [];
extern const char *glibtop_labels_proc_signal [];
extern const char *glibtop_descriptions_proc_signal [];

#endif

END_LIBGTOP_DECLS

#endif
