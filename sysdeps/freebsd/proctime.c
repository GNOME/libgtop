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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/proctime.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_proc_time =
(1 << GLIBTOP_PROC_TIME_RTIME) + (1 << GLIBTOP_PROC_TIME_FREQUENCY);

static const unsigned long _glibtop_sysdeps_proc_time_user =
(1 << GLIBTOP_PROC_TIME_UTIME) + (1 << GLIBTOP_PROC_TIME_STIME) +
(1 << GLIBTOP_PROC_TIME_CUTIME) + (1 << GLIBTOP_PROC_TIME_CSTIME);

#define tv2sec(tv)	(((u_int64_t) tv.tv_sec * 1000000) + (u_int64_t) tv.tv_usec)

/* Init function. */

void
glibtop_init_proc_time_p (glibtop *server)
{
	server->sysdeps.proc_time = _glibtop_sysdeps_proc_time |
		_glibtop_sysdeps_proc_time_user;
}

/* Taken from /usr/src/sys/kern/kern_resource.c */

/*
 * Transform the running time and tick information in proc p into user,
 * system, and interrupt time usage.
 */

static void
calcru(p, up, sp, ip)
     struct proc *p;
     struct timeval *up;
     struct timeval *sp;
     struct timeval *ip;
{
	quad_t totusec;
	u_quad_t u, st, ut, it, tot;
	long sec, usec;
	struct timeval tv;

	st = p->p_sticks;
	ut = p->p_uticks;
	it = p->p_iticks;

	tot = st + ut + it;
	if (tot == 0) {
		st = 1;
		tot = 1;
	}

	sec = p->p_rtime.tv_sec;
	usec = p->p_rtime.tv_usec;

	totusec = (quad_t)sec * 1000000 + usec;
	if (totusec < 0) {
		/* XXX no %qd in kernel.  Truncate. */
		printf("calcru: negative time: %ld usec\n", (long)totusec);
		totusec = 0;
	}

	u = totusec;
	st = (u * st) / tot;
	sp->tv_sec = st / 1000000;
	sp->tv_usec = st % 1000000;
	ut = (u * ut) / tot;
	up->tv_sec = ut / 1000000;
	up->tv_usec = ut % 1000000;
	if (ip != NULL) {
		it = (u * it) / tot;
		ip->tv_sec = it / 1000000;
		ip->tv_usec = it % 1000000;
	}
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_time_p (glibtop *server, glibtop_proc_time *buf,
			 pid_t pid)
{
	struct kinfo_proc *pinfo;
	struct user *u_addr = (struct user *)USRSTACK;
	struct pstats pstats;
	struct pcb pcb;
	int f, count;
	quad_t totusec;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_TIME), 0);
	
	memset (buf, 0, sizeof (glibtop_proc_time));

	glibtop_suid_enter (server);

	/* Get the process information */
	pinfo = kvm_getprocs (server->machine.kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1))
		glibtop_error_io_r (server, "kvm_getprocs (%d)", pid);

	if ((pinfo [0].kp_proc.p_flag & P_INMEM) &&
	    kvm_uread (server->machine.kd, &(pinfo [0]).kp_proc,
		       (unsigned long) &u_addr->u_stats,
		       (char *) &pstats, sizeof (pstats)) == sizeof (pstats))
		{
			/* This is taken form the kernel source code of
			 * FreeBSD 2.2.6. */

			/* Well, we just do the same getrusage () does ... */

			register struct rusage *rup;

			glibtop_suid_leave (server);
			
			rup = &pstats.p_ru;
			calcru(&(pinfo [0]).kp_proc,
			       &rup->ru_utime, &rup->ru_stime, NULL);

			buf->utime = tv2sec (pstats.p_ru.ru_utime);
			buf->stime = tv2sec (pstats.p_ru.ru_stime);
			
			buf->cutime = tv2sec (pstats.p_cru.ru_utime);
			buf->cstime = tv2sec (pstats.p_cru.ru_stime);

			buf->start_time = tv2sec (pstats.p_start);

			buf->flags = _glibtop_sysdeps_proc_time_user;
		}

	glibtop_suid_leave (server);

	buf->rtime = tv2sec (pinfo [0].kp_proc.p_rtime);

	buf->frequency = 1000000;

	buf->flags |= _glibtop_sysdeps_proc_time;
}

