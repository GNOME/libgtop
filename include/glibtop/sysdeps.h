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

#ifndef __GLIBTOP_SYSDEPS_H__
#define __GLIBTOP_SYSDEPS_H__

#include <glibtop.h>

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_SYSDEPS_CPU		0
#define GLIBTOP_SYSDEPS_MEM		1
#define GLIBTOP_SYSDEPS_SWAP		2
#define GLIBTOP_SYSDEPS_UPTIME		3
#define GLIBTOP_SYSDEPS_LOADAVG		4
#define GLIBTOP_SYSDEPS_SHM_LIMITS	5
#define GLIBTOP_SYSDEPS_MSG_LIMITS	6
#define GLIBTOP_SYSDEPS_SEM_LIMITS	7
#define GLIBTOP_SYSDEPS_PROCLIST	8
#define GLIBTOP_SYSDEPS_PROC_STATE	9
#define GLIBTOP_SYSDEPS_PROC_UID	10
#define GLIBTOP_SYSDEPS_PROC_MEM	11
#define GLIBTOP_SYSDEPS_PROC_TIME	12
#define GLIBTOP_SYSDEPS_PROC_SIGNAL	13
#define GLIBTOP_SYSDEPS_PROC_KERNEL	14
#define GLIBTOP_SYSDEPS_PROC_SEGMENT	15
#define GLIBTOP_SYSDEPS_PROC_ARGS	16
#define GLIBTOP_SYSDEPS_PROC_MAP	17
#define GLIBTOP_SYSDEPS_MOUNTLIST	18
#define GLIBTOP_SYSDEPS_FSUSAGE		19
#define GLIBTOP_SYSDEPS_NETLOAD		20
#define GLIBTOP_SYSDEPS_PPP		21

#define GLIBTOP_MAX_SYSDEPS		24

#define GLIBTOP_SYSDEPS_ALL		((1 << GLIBTOP_MAX_SYSDEPS) - 1)

typedef int (*glibtop_init_func_t) (glibtop *);
extern glibtop_init_func_t _glibtop_init_hook_s [];
extern glibtop_init_func_t _glibtop_init_hook_p [];

typedef struct _glibtop_sysdeps	glibtop_sysdeps;

struct _glibtop_sysdeps
{
    u_int64_t flags,
	features,		/* server features	*/
	pointer_size,		/* sizeof(void*)*8	*/
	cpu,			/* glibtop_cpu		*/
	mem,			/* glibtop_mem		*/
	swap,			/* glibtop_swap		*/
	uptime,			/* glibtop_uptime	*/
	loadavg,		/* glibtop_loadavg	*/
	shm_limits,		/* glibtop_shm_limits	*/
	msg_limits,		/* glibtop_msg_limits	*/
	sem_limits,		/* glibtop_sem_limits	*/
	proclist,		/* glibtop_proclist	*/
	proc_state,		/* glibtop_proc_state	*/
	proc_uid,		/* glibtop_proc_uid	*/
	proc_mem,		/* glibtop_proc_mem	*/
	proc_time,		/* glibtop_proc_time	*/
	proc_signal,		/* glibtop_proc_signal	*/
	proc_kernel,		/* glibtop_proc_kernel	*/
	proc_segment,		/* glibtop_proc_segment	*/
	proc_args,		/* glibtop_proc_args	*/
	proc_map,		/* glibtop_proc_map	*/
	mountlist,		/* glibtop_mountlist	*/
	fsusage,		/* glibtop_fsusage	*/
	netload,		/* glibtop_netload	*/
	ppp;			/* glibtop_ppp		*/
};

#define glibtop_get_sysdeps(sysdeps)	glibtop_get_sysdeps_r(glibtop_global_server,sysdeps)

void glibtop_get_sysdeps_r (glibtop *server, glibtop_sysdeps *buf);

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_sysdeps [];
extern const unsigned glibtop_types_sysdeps [];
extern const char *glibtop_labels_sysdeps [];
extern const char *glibtop_descriptions_sysdeps [];

#endif

END_LIBGTOP_DECLS

#endif
