/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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
#include <glibtop/cpu.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_cpu =
(1 << GLIBTOP_CPU_TOTAL) + (1 << GLIBTOP_CPU_USER) +
(1 << GLIBTOP_CPU_NICE) + (1 << GLIBTOP_CPU_SYS) +
(1 << GLIBTOP_CPU_IDLE) + (1 << GLIBTOP_CPU_FREQUENCY);

/* nlist structure for kernel access */
static struct nlist nlst [] = {
	{ "_cp_time" },
	{ 0 }
};

/* MIB array for sysctl */
static const int mib_length=2;
static const int mib [] = { CTL_KERN, KERN_CLOCKRATE };

/* Init function. */

void
glibtop_init_cpu_p (glibtop *server)
{
	server->sysdeps.cpu = _glibtop_sysdeps_cpu;

	if (kvm_nlist (server->machine.kd, nlst) != 0)
		glibtop_error_io_r (server, "kvm_nlist");
}

/* Provides information about cpu usage. */

void
glibtop_get_cpu_p (glibtop *server, glibtop_cpu *buf)
{
	long cpts [CPUSTATES];
	/* sysctl vars*/
	struct clockinfo ci;
	size_t length;
	
	glibtop_init_p (server, GLIBTOP_SYSDEPS_CPU, 0);
	
	memset (buf, 0, sizeof (glibtop_cpu));

	if (kvm_read (server->machine.kd, nlst [0].n_value,
		      &cpts, sizeof (cpts)))
		glibtop_error_io_r (server, "kvm_read (cp_time)");

	/* Get the clockrate data */
	length = sizeof (struct clockinfo);
	if (sysctl (mib, mib_length, &ci, &length, NULL, 0))
		glibtop_error_io_r (server, "sysctl");

	buf->flags = _glibtop_sysdeps_cpu;

	/* set user time */
	buf->user = cpts [CP_USER];
	/* set nice time */
	buf->nice = cpts [CP_NICE];
	/* set sys time */
	buf->sys = cpts [CP_SYS];
	/* set idle time */
	buf->idle = cpts [CP_IDLE];
	
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
}
