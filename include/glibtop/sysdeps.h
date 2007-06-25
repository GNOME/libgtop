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

G_BEGIN_DECLS

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
#define GLIBTOP_SYSDEPS_NETLIST		23
#define GLIBTOP_SYSDEPS_PROC_OPEN_FILES	24
#define GLIBTOP_SYSDEPS_PROC_WD		25
#define GLIBTOP_SYSDEPS_PROC_AFFINITY	26

#define GLIBTOP_MAX_SYSDEPS		27

#define GLIBTOP_SYSDEPS_ALL		((1 << GLIBTOP_MAX_SYSDEPS) - 1)

typedef struct _glibtop_sysdeps	glibtop_sysdeps;

struct _glibtop_sysdeps
{
	guint64 flags;
	guint64 features;		/* server features	*/
	guint64 cpu;			/* glibtop_cpu		*/
	guint64 mem;			/* glibtop_mem		*/
	guint64 swap;			/* glibtop_swap		*/
	guint64 uptime;			/* glibtop_uptime	*/
	guint64 loadavg;		/* glibtop_loadavg	*/
	guint64 shm_limits;		/* glibtop_shm_limits	*/
	guint64 msg_limits;		/* glibtop_msg_limits	*/
	guint64 sem_limits;		/* glibtop_sem_limits	*/
	guint64 proclist;		/* glibtop_proclist	*/
	guint64 proc_state;		/* glibtop_proc_state	*/
	guint64 proc_uid;		/* glibtop_proc_uid	*/
	guint64 proc_mem;		/* glibtop_proc_mem	*/
	guint64 proc_time;		/* glibtop_proc_time	*/
	guint64 proc_signal;		/* glibtop_proc_signal	*/
	guint64 proc_kernel;		/* glibtop_proc_kernel	*/
	guint64 proc_segment;		/* glibtop_proc_segment	*/
	guint64 proc_args;		/* glibtop_proc_args	*/
	guint64 proc_map;		/* glibtop_proc_map	*/
	guint64 proc_open_files;	/* glibtop_proc_open_files */
	guint64 mountlist;		/* glibtop_mountlist	*/
	guint64 fsusage;		/* glibtop_fsusage	*/
	guint64 netlist;		/* glibtop_netlist	*/
	guint64 netload;		/* glibtop_netload	*/
	guint64 ppp;			/* glibtop_ppp		*/
	guint64	proc_wd;		/* glibtop_proc_wd	*/
	guint64	proc_affinity;		/* glibtop_proc_affinity */
};

void glibtop_get_sysdeps (glibtop_sysdeps *buf);
void glibtop_get_sysdeps_r (glibtop *server, glibtop_sysdeps *buf);


G_END_DECLS

#endif
