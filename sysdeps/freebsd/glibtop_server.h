/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef __GLIBTOP_SERVER_H__
#define __GLIBTOP_SERVER_H__

__BEGIN_DECLS

#define GLIBTOP_SUID_CPU		(1 << GLIBTOP_SYSDEPS_CPU)
#define GLIBTOP_SUID_MEM		(1 << GLIBTOP_SYSDEPS_MEM)
#define GLIBTOP_SUID_SWAP		(1 << GLIBTOP_SYSDEPS_SWAP)
#define GLIBTOP_SUID_UPTIME		(1 << GLIBTOP_SYSDEPS_UPTIME)
#define GLIBTOP_SUID_LOADAVG		(1 << GLIBTOP_SYSDEPS_LOADAVG)
#define GLIBTOP_SUID_SHM_LIMITS		(1 << GLIBTOP_SYSDEPS_SHM_LIMITS)
#define GLIBTOP_SUID_MSG_LIMITS		(1 << GLIBTOP_SYSDEPS_MSG_LIMITS)
#define GLIBTOP_SUID_SEM_LIMITS		(1 << GLIBTOP_SYSDEPS_SEM_LIMITS)
#define GLIBTOP_SUID_PROCLIST		(1 << GLIBTOP_SYSDEPS_PROCLIST)
#define GLIBTOP_SUID_PROC_STATE		(1 << GLIBTOP_SYSDEPS_PROC_STATE)
#define GLIBTOP_SUID_PROC_UID		(1 << GLIBTOP_SYSDEPS_PROC_UID)
#define GLIBTOP_SUID_PROC_MEM		(1 << GLIBTOP_SYSDEPS_PROC_MEM)
#define GLIBTOP_SUID_PROC_TIME		(1 << GLIBTOP_SYSDEPS_PROC_TIME)
#define GLIBTOP_SUID_PROC_SIGNAL	(1 << GLIBTOP_SYSDEPS_PROC_SIGNAL)
#define GLIBTOP_SUID_PROC_KERNEL	(1 << GLIBTOP_SYSDEPS_PROC_KERNEL)
#define GLIBTOP_SUID_PROC_SEGMENT	(1 << GLIBTOP_SYSDEPS_PROC_SEGMENT)
#define GLIBTOP_SUID_PROC_ARGS		(1 << GLIBTOP_SYSDEPS_PROC_ARGS)
#define GLIBTOP_SUID_PROC_MAP		(1 << GLIBTOP_SYSDEPS_PROC_MAP)
#define GLIBTOP_SUID_PPP		(1 << GLIBTOP_SYSDEPS_PPP)

__END_DECLS

#endif
