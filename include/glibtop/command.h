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
#include <glibtop/union.h>

__BEGIN_DECLS

#define GLIBTOP_CMND_QUIT		0
#define GLIBTOP_CMND_SYSDEPS		1

#define GLIBTOP_CMND_CPU		2
#define GLIBTOP_CMND_MEM		3
#define GLIBTOP_CMND_SWAP		4
#define GLIBTOP_CMND_UPTIME		5
#define GLIBTOP_CMND_LOADAVG		6
#define GLIBTOP_CMND_SHM_LIMITS		7
#define GLIBTOP_CMND_MSG_LIMITS		8
#define GLIBTOP_CMND_SEM_LIMITS		9
#define GLIBTOP_CMND_PROCLIST		10

#define GLIBTOP_CMND_PROC_STATE		11
#define GLIBTOP_CMND_PROC_UID		12
#define GLIBTOP_CMND_PROC_MEM		13
#define GLIBTOP_CMND_PROC_TIME		14
#define GLIBTOP_CMND_PROC_SIGNAL	15
#define GLIBTOP_CMND_PROC_KERNEL	16
#define GLIBTOP_CMND_PROC_SEGMENT	17
#define GLIBTOP_CMND_PROC_ARGS		18
#define GLIBTOP_CMND_PROC_MAP		19

#define GLIBTOP_CMND_MOUNTLIST		20
#define GLIBTOP_CMND_FSUSAGE		21
#define GLIBTOP_CMND_NETLOAD		22
#define GLIBTOP_CMND_PPP		23

#define GLIBTOP_MAX_CMND		24

#define _GLIBTOP_PARAM_SIZE		16

typedef struct _glibtop_command		glibtop_command;

typedef struct _glibtop_response	glibtop_response;
typedef union  _glibtop_response_union	glibtop_response_union;

struct _glibtop_command
{
	u_int64_t	command;
	u_int64_t	size, data_size;
	char		parameter [_GLIBTOP_PARAM_SIZE];
};

union _glibtop_response_union
{
	glibtop_union	data;
	glibtop_sysdeps	sysdeps;
};

struct _glibtop_response
{
	int64_t offset;
	u_int64_t size, data_size;
	glibtop_response_union u;
};

#define glibtop_call(p1, p2, p3, p4)	glibtop_call_r(glibtop_global_server, p1, p2, p3, p4)

void *
glibtop_call_l (glibtop *server, unsigned command, size_t send_size,
		const void *send_buf, size_t recv_size, void *recv_buf);

void *
glibtop_call_s (glibtop *server, unsigned command, size_t send_size,
		const void *send_buf, size_t recv_size, void *recv_buf);

__END_DECLS

#endif
