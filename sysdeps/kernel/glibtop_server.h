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

#ifndef __GLIBTOP_SERVER_H__
#define __GLIBTOP_SERVER_H__

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_IMPL_CPU		(1 << GLIBTOP_SYSDEPS_CPU)
#define GLIBTOP_IMPL_MEM		(1 << GLIBTOP_SYSDEPS_MEM)
#define GLIBTOP_IMPL_SWAP		(1 << GLIBTOP_SYSDEPS_SWAP)
#define GLIBTOP_IMPL_UPTIME		(1 << GLIBTOP_SYSDEPS_UPTIME)
#define GLIBTOP_IMPL_LOADAVG		(1 << GLIBTOP_SYSDEPS_LOADAVG)
#define GLIBTOP_IMPL_SHM_LIMITS		0
#define GLIBTOP_IMPL_MSG_LIMITS		0
#define GLIBTOP_IMPL_SEM_LIMITS		0
#define GLIBTOP_IMPL_PROCLIST		(1 << GLIBTOP_SYSDEPS_PROCLIST)
#define GLIBTOP_IMPL_PROC_STATE		(1 << GLIBTOP_SYSDEPS_PROC_STATE)
#define GLIBTOP_IMPL_PROC_UID		(1 << GLIBTOP_SYSDEPS_PROC_UID)
#define GLIBTOP_IMPL_PROC_MEM		(1 << GLIBTOP_SYSDEPS_PROC_MEM)
#define GLIBTOP_IMPL_PROC_TIME		(1 << GLIBTOP_SYSDEPS_PROC_TIME)
#define GLIBTOP_IMPL_PROC_SIGNAL	(1 << GLIBTOP_SYSDEPS_PROC_SIGNAL)
#define GLIBTOP_IMPL_PROC_KERNEL	(1 << GLIBTOP_SYSDEPS_PROC_KERNEL)
#define GLIBTOP_IMPL_PROC_SEGMENT	(1 << GLIBTOP_SYSDEPS_PROC_SEGMENT)
#define GLIBTOP_IMPL_PROC_ARGS		(1 << GLIBTOP_SYSDEPS_PROC_ARGS)
#define GLIBTOP_IMPL_PROC_MAP		(1 << GLIBTOP_SYSDEPS_PROC_MAP)
#define GLIBTOP_IMPL_NETLOAD		(1 << GLIBTOP_SYSDEPS_NETLOAD)
#define GLIBTOP_IMPL_NETINFO		(1 << GLIBTOP_SYSDEPS_NETINFO)
#define GLIBTOP_IMPL_INTERFACE_NAMES	(1 << GLIBTOP_SYSDEPS_INTERFACE_NAMES)
#define GLIBTOP_IMPL_PPP		0

END_LIBGTOP_DECLS

#endif
