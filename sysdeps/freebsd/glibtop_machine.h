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

#ifndef __GLIBTOP_MACHINE_H__
#define __GLIBTOP_MACHINE_H__

#include <sys/types.h>
#include <sys/signal.h>
#include <sys/param.h>

#include <stdio.h>
#include <nlist.h>
#include <math.h>
#include <kvm.h>
#include <sys/errno.h>
#include <sys/sysctl.h>
#include <sys/dir.h>
#include <sys/dkstat.h>
#include <sys/file.h>
#include <sys/time.h>

#include <stdlib.h>
#include <sys/rlist.h>
#include <sys/conf.h>

__BEGIN_DECLS

#define X_CCPU		0
#define X_CP_TIME	1
#define X_HZ		2
#define X_STATHZ	3
#define X_AVENRUN	4
#define VM_SWAPLIST	5	/* list of free swap areas */
#define VM_SWDEVT	6	/* list of swap devices and sizes */
#define VM_NSWAP	7	/* size of largest swap device */
#define VM_NSWDEV	8	/* number of swap devices */
#define VM_DMMAX	9	/* maximum size of a swap block */
#define X_CNT           10	/* struct vmmeter cnt */
#define X_LASTPID	11

typedef struct _glibtop_machine		glibtop_machine;

struct _glibtop_machine
{
	uid_t uid, euid;		/* Real and effective user id */
	gid_t gid, egid;		/* Real and effective group id */
	int nlist_count;		/* Number of symbols in the nlist */
	u_int64_t hz;			/* Tick frequency */
	int ncpu;			/* Number of CPUs we have */
	kvm_t *kd;
};

/* Those functions are used internally in libgtop */

#ifdef _IN_LIBGTOP

extern struct nlist _glibtop_nlist[];

extern int _glibtop_check_nlist __P((void *, register struct nlist *));

extern int _glibtop_getkval __P((void *, unsigned long, int *, int, char *));

#endif

__END_DECLS

#endif
