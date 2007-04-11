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

#ifndef __GLIBTOP_CPU_H__
#define __GLIBTOP_CPU_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_CPU_TOTAL	0
#define GLIBTOP_CPU_USER	1
#define GLIBTOP_CPU_NICE	2
#define GLIBTOP_CPU_SYS		3
#define GLIBTOP_CPU_IDLE	4
#define GLIBTOP_CPU_FREQUENCY	5

#define GLIBTOP_XCPU_TOTAL	6
#define GLIBTOP_XCPU_USER	7
#define GLIBTOP_XCPU_NICE	8
#define GLIBTOP_XCPU_SYS	9
#define GLIBTOP_XCPU_IDLE	10
#define GLIBTOP_XCPU_FLAGS	11

#define GLIBTOP_CPU_IOWAIT	12
#define GLIBTOP_CPU_IRQ		13
#define GLIBTOP_CPU_SOFTIRQ	14
#define GLIBTOP_XCPU_IOWAIT	15
#define GLIBTOP_XCPU_IRQ	16
#define GLIBTOP_XCPU_SOFTIRQ	17

#define GLIBTOP_MAX_CPU		18

/* Nobody should really be using more than 4 processors.
   Yes we are :)
   Nobody should really be using more than 32 processors.
*/
#define GLIBTOP_NCPU		32

typedef struct _glibtop_cpu	glibtop_cpu;

struct _glibtop_cpu
{
	guint64	flags;
	guint64 total;				/* GLIBTOP_CPU_TOTAL		*/
	guint64 user;				/* GLIBTOP_CPU_USER		*/
	guint64 nice;				/* GLIBTOP_CPU_NICE		*/
	guint64 sys;				/* GLIBTOP_CPU_SYS		*/
	guint64 idle;				/* GLIBTOP_CPU_IDLE		*/
	guint64 iowait;				/* GLIBTOP_CPU_IOWAIT		*/
	guint64 irq;				/* GLIBTOP_CPU_IRQ		*/
	guint64 softirq;			/* GLIBTOP_CPU_SOFTIRQ		*/
	guint64 frequency;			/* GLIBTOP_CPU_FREQUENCY	*/
	guint64 xcpu_total [GLIBTOP_NCPU];	/* GLIBTOP_XCPU_TOTAL		*/
	guint64 xcpu_user [GLIBTOP_NCPU];	/* GLIBTOP_XCPU_USER		*/
	guint64 xcpu_nice [GLIBTOP_NCPU];	/* GLIBTOP_XCPU_NICE		*/
	guint64 xcpu_sys  [GLIBTOP_NCPU];	/* GLIBTOP_XCPU_SYS		*/
	guint64 xcpu_idle [GLIBTOP_NCPU];	/* GLIBTOP_XCPU_IDLE		*/
	guint64 xcpu_iowait [GLIBTOP_NCPU];	/* GLIBTOP_XCPU_IOWAIT		*/
	guint64 xcpu_irq [GLIBTOP_NCPU];	/* GLIBTOP_XCPU_IRQ		*/
	guint64 xcpu_softirq  [GLIBTOP_NCPU];	/* GLIBTOP_XCPU_SOFTIRQ		*/
	guint64 xcpu_flags;	                /* GLIBTOP_XCPU_IDLE		*/
};

void glibtop_get_cpu(glibtop_cpu *buf);

#if GLIBTOP_SUID_CPU
#define glibtop_get_cpu_r	glibtop_get_cpu_p
#else
#define glibtop_get_cpu_r	glibtop_get_cpu_s
#endif

void glibtop_get_cpu_l (glibtop *server, glibtop_cpu *buf);

#if GLIBTOP_SUID_CPU
void _glibtop_init_cpu_p (glibtop *server);
void glibtop_get_cpu_p (glibtop *server, glibtop_cpu *buf);
#else
void _glibtop_init_cpu_s (glibtop *server);
void glibtop_get_cpu_s (glibtop *server, glibtop_cpu *buf);
#endif


G_END_DECLS

#endif
