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

#ifndef __GLIBTOP_COMMAND_H__
#define __GLIBTOP_COMMAND_H__

#include <glibtop.h>
#include <glibtop/sysdeps.h>

__BEGIN_DECLS

#define GLIBTOP_CMND_QUIT		0
#define GLIBTOP_CMND_CPU		1
#define GLIBTOP_CMND_MEM		2
#define GLIBTOP_CMND_SWAP		3
#define GLIBTOP_CMND_UPTIME		4
#define GLIBTOP_CMND_LOADAVG		5
#define GLIBTOP_CMND_SHM_LIMITS		6
#define GLIBTOP_CMND_MSG_LIMITS		7
#define GLIBTOP_CMND_SEM_LIMITS		8
#define GLIBTOP_CMND_PROCLIST		9

#define GLIBTOP_CMND_PROC_STATE		10
#define GLIBTOP_CMND_PROC_UID		11
#define GLIBTOP_CMND_PROC_MEM		12
#define GLIBTOP_CMND_PROC_TIME		13
#define GLIBTOP_CMND_PROC_SIGNAL	14
#define GLIBTOP_CMND_PROC_KERNEL	15
#define GLIBTOP_CMND_PROC_SEGMENT	16

#define GLIBTOP_MAX_CMND		17

typedef struct _glibtop_command	glibtop_command;

struct _glibtop_command
{
  glibtop	server;
  unsigned	command;
  size_t	size;
};

#define glibtop_call(p1, p2, p3, p4)	glibtop_call__r(glibtop_global_server, p1, p2, p3, p4)

extern void *glibtop_call__l __P((glibtop *, unsigned, size_t, void *, size_t, void *));
extern void *glibtop_call__s __P((glibtop *, unsigned, size_t, void *, size_t, void *));

__END_DECLS

#endif
