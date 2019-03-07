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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

/**
 * GLIBTOP_KERN_PROC_ALL:
 *
 * Return information about all processes
 **/
#define GLIBTOP_KERN_PROC_ALL		0

/**
 * GLIBTOP_KERN_PROC_PID:
 *
 * Return all processes with the pid which is passed in @arg. You can use this to find out whether some process still exists.
 **/
#define GLIBTOP_KERN_PROC_PID		1

/**
 * GLIBTOP_KERN_PROC_PGRP:
 *
 * Return all processes in the process group passed in @arg. 
 **/
#define GLIBTOP_KERN_PROC_PGRP		2

/**
 * GLIBTOP_KERN_PROC_SESSION:
 *
 * Return all processes in the session passed in @arg. 
 **/
#define GLIBTOP_KERN_PROC_SESSION	3

/**
 * GLIBTOP_KERN_PROC_TTY:
 *
 * Return all processes which have the controlling tty passed in @arg
 * (which is interpreted as the device number). 
 **/
#define GLIBTOP_KERN_PROC_TTY		4

/**
 * GLIBTOP_KERN_PROC_UID:
 *
 * Return all processes with the effective uid passed in @arg. 
 **/
#define GLIBTOP_KERN_PROC_UID		5

/**
 * GLIBTOP_KERN_PROC_RUID:
 *
 * Return all processes with the real uid passed in @arg. 
 **/
#define GLIBTOP_KERN_PROC_RUID		6

#define GLIBTOP_KERN_PROC_MASK		15


/**
 * GLIBTOP_EXCLUDE_IDLE:
 *
 * Exclude idle processes. 
 **/
#define GLIBTOP_EXCLUDE_IDLE		0x1000

/**
 * GLIBTOP_EXCLUDE_SYSTEM:
 *
 * Exclude system (on most UNIXes root's) processes.
 **/
#define GLIBTOP_EXCLUDE_SYSTEM		0x2000

/**
 * GLIBTOP_EXCLUDE_NOTTY:
 *
 * Exclude processes without a controlling terminal.
 **/
#define GLIBTOP_EXCLUDE_NOTTY		0x4000

typedef struct _glibtop_proclist	glibtop_proclist;

/**
 * glibtop_proclist:
 * @number: Number of entries in the returned list.
 * @total: Total size of the returned list (this equals @number * @size).
 * @size: Size of a single entry in the returned list 
 * (this equals <type>sizeof(unsigned)</type>).
 */
struct _glibtop_proclist
{
	guint64	flags;
	guint64 number;			/* GLIBTOP_PROCLIST_NUMBER	*/
	guint64 total;			/* GLIBTOP_PROCLIST_TOTAL	*/
	guint64 size;			/* GLIBTOP_PROCLIST_SIZE	*/
};

/**
 * glibtop_get_proclist:
 * @buf: a #glibtop_proclist
 * @which: a #GLIBTOP_* constant specifying process type
 * @arg: an argument specific for the process type
 *
 * Returns: (array zero-terminated=1) (transfer full): an array of process
 *     ids
 */
pid_t*
glibtop_get_proclist(glibtop_proclist *buf, gint64 which, gint64 arg);

#if GLIBTOP_SUID_PROCLIST
#define glibtop_get_proclist_r		glibtop_get_proclist_p
#else
#define glibtop_get_proclist_r		glibtop_get_proclist_s
#endif

/**
 * glibtop_get_proclist_l:
 * @server: a #glibtop server
 * @buf: a #glibtop_proclist
 * @which: a #GLIBTOP_* constant specifying process type
 * @arg: an argument specific for the process type
 *
 * Returns: (array zero-terminated=1) (transfer full): an array of process
 *     ids
 */
pid_t*
glibtop_get_proclist_l (glibtop *server, glibtop_proclist *buf,
			gint64 which, gint64 arg);

#if GLIBTOP_SUID_PROCLIST
void _glibtop_init_proclist_p (glibtop *server);

/**
 * glibtop_get_proclist_p:
 * @server: a #glibtop server
 * @buf: a #glibtop_proclist
 * @which: a #GLIBTOP_* constant specifying process type
 * @arg: an argument specific for the process type
 *
 * Returns: (array zero-terminated=1) (transfer full): an array of process
 *     ids
 */
pid_t*
glibtop_get_proclist_p (glibtop *server, glibtop_proclist *buf,
			gint64 which, gint64 arg);
#else
void _glibtop_init_proclist_s (glibtop *server);

/**
 * glibtop_get_proclist_s:
 * @server: a #glibtop server
 * @buf: a #glibtop_proclist
 * @which: a #GLIBTOP_* constant specifying process type
 * @arg: an argument specific for the process type
 *
 * Returns: (array zero-terminated=1) (transfer full): an array of process
 *     ids
 */
pid_t*
glibtop_get_proclist_s (glibtop *server, glibtop_proclist *buf,
			gint64 which, gint64 arg);
#endif


G_END_DECLS

#endif
