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

#ifndef __GLIBTOP_SYSDEPS_H__
#define __GLIBTOP_SYSDEPS_H__

#include <glibtop.h>

__BEGIN_DECLS

#define GLIBTOP_SYSDEPS_FEATURES	0
#define GLIBTOP_SYSDEPS_CPU		1
#define GLIBTOP_SYSDEPS_MEM		2
#define GLIBTOP_SYSDEPS_SWAP		3
#define GLIBTOP_SYSDEPS_UPTIME		4
#define GLIBTOP_SYSDEPS_LOADAVG		5
#define GLIBTOP_SYSDEPS_SHM_LIMITS	6
#define GLIBTOP_SYSDEPS_MSG_LIMITS	7
#define GLIBTOP_SYSDEPS_SEM_LIMITS	8
#define GLIBTOP_SYSDEPS_PROCLIST	9
#define GLIBTOP_SYSDEPS_PROC_STATE	10
#define GLIBTOP_SYSDEPS_PROC_UID	11
#define GLIBTOP_SYSDEPS_PROC_MEM	12
#define GLIBTOP_SYSDEPS_PROC_TIME	13
#define GLIBTOP_SYSDEPS_PROC_SIGNAL	14
#define GLIBTOP_SYSDEPS_PROC_KERNEL	15
#define GLIBTOP_SYSDEPS_PROC_SEGMENT	16
#define GLIBTOP_SYSDEPS_PROC_ARGS	17
#define GLIBTOP_SYSDEPS_PROC_MAP	18
#define GLIBTOP_SYSDEPS_MOUNTLIST	19
#define GLIBTOP_SYSDEPS_FSUSAGE		20
#define GLIBTOP_SYSDEPS_NETLOAD		21
#define GLIBTOP_SYSDEPS_PPP		22

#define GLIBTOP_MAX_SYSDEPS		23

#define GLIBTOP_SYSDEPS_ALL		((1 << GLIBTOP_MAX_SYSDEPS) - 1)

typedef void (*glibtop_init_func_t) (glibtop *);
extern glibtop_init_func_t _glibtop_init_hook_s [];
extern glibtop_init_func_t _glibtop_init_hook_p [];

typedef struct _glibtop_sysdeps	glibtop_sysdeps;

struct _glibtop_sysdeps
{
	u_int64_t flags,
		features,		/* server features	*/
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

extern void glibtop_get_sysdeps_r __P((glibtop *, glibtop_sysdeps *));

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_sysdeps [];
extern const unsigned glibtop_types_sysdeps [];
extern const char *glibtop_labels_sysdeps [];
extern const char *glibtop_descriptions_sysdeps [];

#endif

__END_DECLS

#endif
