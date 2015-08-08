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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/proctime.h>

#include <glibtop_suid.h>

#ifdef __FreeBSD__
#include <osreldate.h>
#endif

static const unsigned long _glibtop_sysdeps_proc_time =
(1L << GLIBTOP_PROC_TIME_RTIME) + (1L << GLIBTOP_PROC_TIME_FREQUENCY);

static const unsigned long _glibtop_sysdeps_proc_time_user =
(1L << GLIBTOP_PROC_TIME_UTIME) + (1L << GLIBTOP_PROC_TIME_STIME) +
(1L << GLIBTOP_PROC_TIME_CUTIME) + (1L << GLIBTOP_PROC_TIME_CSTIME) +
(1L << GLIBTOP_PROC_TIME_START_TIME);

#define tv2sec(tv)	(((guint64) tv.tv_sec * 1000000) + (guint64) tv.tv_usec)

/* Init function. */

void
_glibtop_init_proc_time_p (glibtop *server)
{
	server->sysdeps.proc_time = _glibtop_sysdeps_proc_time |
		_glibtop_sysdeps_proc_time_user;
}

/* Taken from /usr/src/sys/kern/kern_resource.c */

/*
 * Transform the running time and tick information in proc p into user,
 * system, and interrupt time usage.
 */

#if !(defined(__FreeBSD__) || defined(__FreeBSD_kernel__))

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
		fprintf (stderr, "calcru: negative time: %ld usec\n",
			 (long)totusec);
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
#endif /* !__FreeBSD__ */

/* Provides detailed information about a process. */

void
glibtop_get_proc_time_p (glibtop *server, glibtop_proc_time *buf,
			 pid_t pid)
{
	struct kinfo_proc *pinfo;
#if (defined(__NetBSD__) && (__NetBSD_Version__ >= 104000000)) || (defined(OpenBSD) && (OpenBSD >= 199912))
	register struct rusage *rup;
#else
	struct user *u_addr = (struct user *)USRSTACK;
#endif
	struct pstats pstats;
	int count;

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_PROC_TIME), 0);

	memset (buf, 0, sizeof (glibtop_proc_time));

	/* It does not work for the swapper task. */
	if (pid == 0) return;

#if (defined(__NetBSD__) && (__NetBSD_Version__ >= 104000000))
	if (server->sysdeps.proc_time == 0)
		return;
#endif

	/* Get the process information */
	pinfo = kvm_getprocs (server->machine->kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1)) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		return;
	}

#if (defined(__FreeBSD__) && (__FreeBSD_version >= 500013)) || defined(__FreeBSD_kernel__)
	buf->rtime = pinfo [0].ki_runtime;
#elif (defined __FreeBSD__) && (__FreeBSD_version <= 500013)
	buf->rtime = pinfo [0].kp_proc.p_runtime;
#else
	buf->rtime = tv2sec (pinfo [0].kp_proc.p_rtime);
#endif

	buf->frequency = 1000000;
	buf->flags = _glibtop_sysdeps_proc_time;

#if (defined(__NetBSD__) && (__NetBSD_Version__ >= 104000000)) || (defined(OpenBSD) && (OpenBSD >= 199912))
	glibtop_suid_enter (server);

	if (kvm_read (server->machine->kd,
		      (unsigned long) pinfo [0].kp_proc.p_stats,
		      &pstats, sizeof (pstats)) != sizeof (pstats)) {
		glibtop_warn_io_r (server, "kvm_read (pstats)");
		return;
	}

	glibtop_suid_leave (server);

	rup = &pstats.p_ru;
	calcru(&(pinfo [0]).kp_proc,
	       &rup->ru_utime, &rup->ru_stime, NULL);

	buf->utime = tv2sec (pstats.p_ru.ru_utime);
	buf->stime = tv2sec (pstats.p_ru.ru_stime);

	buf->cutime = tv2sec (pstats.p_cru.ru_utime);
	buf->cstime = tv2sec (pstats.p_cru.ru_stime);

	buf->start_time = (guint64) pstats.p_start.tv_sec;

	buf->flags |= _glibtop_sysdeps_proc_time_user;
#else
#if (defined(__FreeBSD__) && (__FreeBSD_version >= 500013)) || defined(__FreeBSD_kernel__)
#if (__FreeBSD_version >= 500016) || defined(__FreeBSD_kernel__)
       if ((pinfo [0].ki_flag & PS_INMEM)) {
#else
       if ((pinfo [0].ki_flag & P_INMEM)) {
#endif
           buf->utime = pinfo [0].ki_runtime;
		   buf->stime = tv2sec (pinfo [0].ki_rusage.ru_stime);
           buf->cutime = tv2sec (pinfo [0].ki_childtime);
#if (__FreeBSD_version >= 600000) || (__FreeBSD_kernel_version >= 600000)
		   buf->cstime = tv2sec (pinfo [0].ki_rusage_ch.ru_stime);
#else
		   buf->cstime = 0;
#endif
           buf->start_time = tv2sec (pinfo [0].ki_start);
           buf->flags = _glibtop_sysdeps_proc_time_user;
       }

	glibtop_suid_enter (server);

#elif (__FreeBSD_version <= 500013)

        if ((pinfo [0].kp_proc.p_flag & P_INMEM) &&
            kvm_uread (server->machine->kd, &(pinfo [0]).kp_proc,
                       (unsigned long) &u_addr->u_stats,
                       (char *) &pstats, sizeof (pstats)) == sizeof (pstats))
		{

                       buf->utime = tv2sec (pinfo[0].kp_eproc.e_stats.p_ru.ru_utime);
                       buf->stime = tv2sec (pinfo[0].kp_eproc.e_stats.p_ru.ru_stime);
                       buf->cutime = tv2sec (pinfo[0].kp_eproc.e_stats.p_cru.ru_utime);
                       buf->cstime = tv2sec (pinfo[0].kp_eproc.e_stats.p_cru.ru_stime);
                       buf->start_time = tv2sec (pinfo[0].kp_eproc.e_stats.p_start);
                       buf->flags = _glibtop_sysdeps_proc_time_user;
                       glibtop_suid_leave (server);
		}
#else

	if ((pinfo [0].kp_proc.p_flag & P_INMEM) &&
	    kvm_uread (server->machine->kd, &(pinfo [0]).kp_proc,
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
#endif
	glibtop_suid_leave (server);
#endif
}

