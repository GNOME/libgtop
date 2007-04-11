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

#ifndef __GLIBTOP_PROCUID_H__
#define __GLIBTOP_PROCUID_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_PROC_UID_UID		0
#define GLIBTOP_PROC_UID_EUID		1
#define GLIBTOP_PROC_UID_GID		2
#define GLIBTOP_PROC_UID_EGID		3
#define GLIBTOP_PROC_UID_SUID		4
#define GLIBTOP_PROC_UID_SGID		5
#define GLIBTOP_PROC_UID_FSUID		6
#define GLIBTOP_PROC_UID_FSGID		7
#define GLIBTOP_PROC_UID_PID		8
#define GLIBTOP_PROC_UID_PPID		9
#define GLIBTOP_PROC_UID_PGRP		10
#define GLIBTOP_PROC_UID_SESSION	11
#define GLIBTOP_PROC_UID_TTY		12
#define GLIBTOP_PROC_UID_TPGID		13
#define GLIBTOP_PROC_UID_PRIORITY	14
#define GLIBTOP_PROC_UID_NICE		15
#define GLIBTOP_PROC_UID_NGROUPS    16
#define GLIBTOP_PROC_UID_GROUPS     17

#define GLIBTOP_MAX_PROC_UID		18

typedef struct _glibtop_proc_uid	glibtop_proc_uid;

#define GLIBTOP_MAX_GROUPS          64

/* User, Group, Terminal and session */

struct _glibtop_proc_uid
{
	guint64 flags;
	gint32 uid;		/* user id */
	gint32 euid;		/* effective user id */
	gint32 gid;		/* group id */
	gint32 egid;		/* effective group id */
	gint32 suid;       /* set user id */
	gint32 sgid;       /* set group id */
	gint32 fsuid;      /* ??? user id */
	gint32 fsgid;      /* ??? group id */
	gint32 pid;		/* process id */
	gint32 ppid;		/* pid of parent process */
	gint32 pgrp;		/* process group id */
	gint32 session;	/* session id */
	gint32 tty;		/* full device number of controlling terminal */
	gint32 tpgid;		/* terminal process group id */
	gint32 priority;	/* kernel scheduling priority */
	gint32 nice;		/* standard unix nice level of process */
	gint32 ngroups;
	gint32 groups [GLIBTOP_MAX_GROUPS];

};

void glibtop_get_proc_uid(glibtop_proc_uid *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_UID
#define glibtop_get_proc_uid_r		glibtop_get_proc_uid_p
#else
#define glibtop_get_proc_uid_r		glibtop_get_proc_uid_s
#endif

void glibtop_get_proc_uid_l (glibtop *server, glibtop_proc_uid *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_UID
void _glibtop_init_proc_uid_p (glibtop *server);
void glibtop_get_proc_uid_p (glibtop *server, glibtop_proc_uid *buf, pid_t pid);
#else
void _glibtop_init_proc_uid_s (glibtop *server);
void glibtop_get_proc_uid_s (glibtop *server, glibtop_proc_uid *buf, pid_t pid);
#endif


G_END_DECLS

#endif
