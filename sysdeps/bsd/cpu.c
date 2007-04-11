/* Copyright (C) 1998 Joshua Sled
   This file is part of LibGTop 1.0.

   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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

#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/cpu.h>

#include <glibtop_suid.h>

#ifdef __NetBSD__
#include <sys/sched.h>
#endif

static const unsigned long _glibtop_sysdeps_cpu =
(1L << GLIBTOP_CPU_TOTAL) + (1L << GLIBTOP_CPU_USER) +
(1L << GLIBTOP_CPU_NICE) + (1L << GLIBTOP_CPU_SYS) +
(1L << GLIBTOP_CPU_IDLE) + (1L << GLIBTOP_CPU_FREQUENCY) +
(1L << GLIBTOP_CPU_IOWAIT);

#ifndef KERN_CP_TIME
/* nlist structure for kernel access */
static struct nlist nlst [] = {
#ifdef __bsdi__
	{ "_cpustats" },
#else
	{ "_cp_time" },
#endif
	{ 0 }
};
#endif

/* MIB array for sysctl */
static int mib_length=2;
static int mib [] = { CTL_KERN, KERN_CLOCKRATE };
#ifdef KERN_CP_TIME
static int mib2 [] = { CTL_KERN, KERN_CP_TIME };
#endif

/* Init function. */

void
_glibtop_init_cpu_p (glibtop *server)
{
#ifndef KERN_CP_TIME
	if (kvm_nlist (server->machine.kd, nlst) < 0) {
		glibtop_warn_io_r (server, "kvm_nlist (cpu)");
		return;
	}
#endif

	/* Set this only if kvm_nlist () succeeded. */
	server->sysdeps.cpu = _glibtop_sysdeps_cpu;
}

/* Provides information about cpu usage. */

void
glibtop_get_cpu_p (glibtop *server, glibtop_cpu *buf)
{
#ifdef KERN_CP_TIME
	guint64 cpts [CPUSTATES];
#else
	long cpts [CPUSTATES];
#endif
	/* sysctl vars*/
	struct clockinfo ci;
	size_t length;

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_CPU), 0);

	memset (buf, 0, sizeof (glibtop_cpu));

	/* If this fails, the nlist may not be valid. */
	if (server->sysdeps.cpu == 0)
		return;

#ifdef KERN_CP_TIME
	length = sizeof (cpts);
	if (sysctl (mib2, mib_length, cpts, &length, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl");
		return;
	}
#else
	if (kvm_read (server->machine.kd, nlst [0].n_value,
		      &cpts, sizeof (cpts)) != sizeof (cpts)) {
		glibtop_warn_io_r (server, "kvm_read (cp_time)");
		return;
	}
#endif

	/* Get the clockrate data */
	length = sizeof (struct clockinfo);
	if (sysctl (mib, mib_length, &ci, &length, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl");
		return;
	}

	/* set user time */
	buf->user = cpts [CP_USER];
	/* set nice time */
	buf->nice = cpts [CP_NICE];
	/* set sys time */
	buf->sys = cpts [CP_SYS];
	/* set idle time */
	buf->idle = cpts [CP_IDLE];
	/* set iowait (really just interrupt) time */
	buf->iowait = cpts [CP_INTR];

	/* set frequency */
	/*
	   FIXME --  is hz, tick, profhz or stathz wanted?
	   buf->frequency = sysctl("kern.clockrate", ...);

	   struct clockinfo
	*/
	buf->frequency = ci.hz;
	/* set total */
	buf->total = cpts [CP_USER] + cpts [CP_NICE]
		+ cpts [CP_SYS] + cpts [CP_IDLE];

	/* Set the flags last. */
	buf->flags = _glibtop_sysdeps_cpu;
}
