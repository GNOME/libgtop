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

#include <config.h>
#include <glibtop/cpu.h>

#include <nlist.h>
#include <kvm.h>
#include <sys/dkstat.h>
#include <time.h>
#include <sys/types.h>
#include <sys/sysctl.h>

static const unsigned long _glibtop_sysdeps_cpu =
(1 << GLIBTOP_CPU_TOTAL) + (1 << GLIBTOP_CPU_USER) +
(1 << GLIBTOP_CPU_NICE) + (1 << GLIBTOP_CPU_SYS) +
(1 << GLIBTOP_CPU_IDLE) + (1 << GLIBTOP_CPU_FREQUENCY);

/* Provides information about cpu usage. */

void
glibtop_get_cpu_s (glibtop *server, glibtop_cpu *buf)
{
  /* kvm_* vars */
  kvm_t *kd;
  static const int nlst_length=2;
  long cpts[CPUSTATES];
  /* sysctl vars*/
  static const int mib_length=2;
  int mib[mib_length];
  struct clockinfo ci;
  size_t length;

  /* nlist structure for kernel access */
  struct nlist nlst[nlst_length] = {
    { "_cp_time" },
    { 0 }
  };

  /* MIB array for sysctl(3) use */
  mib[0] = CTL_KERN;
  mib[1] = KERN_CLOCKRATE;

  glibtop_init_r (& server);

  memset (buf, 0, sizeof (glibtop_cpu));

  /* Get the value out of the kernel */
  if (kvm_open(NULL, NULL, NULL, O_RDONLY, "kvm_open") == NULL) {
    /* If we can't get at the kernel, return the null values we just
       gave the array. */
    return;
  }
  kvm_nlist(kd, &nlst);
  kvm_read(kd, nlst[0].n_value, &cpts, sizeof(cpts));
  kvm_close(kd);

  /* Get the clockrate data */
  length = sizeof(clockinfo);
  sysctl(mib, mib_length, &ci, &len, NULL, 0);

  buf->flags = _glibtop_sysdeps_cpu;

  /* set user time */
  buf->user = cpts[CP_USER];
  /* set nice time */
  buf->nice = cpts[CP_NICE];
  /* set sys time */
  buf->sys = cpts[CP_SYS];
  /* set idle time */
  buf->idle = cpts[CP_IDLE];
  /* set frequency */
  /* 
     FIXME --  is hz, tick, profhz or stathz wanted?
     buf->frequency = sysctl("kern.clockrate", ...); 

     struct clockinfo
     */
  buf->frequency = ci.hz;
  /* set total */
  buf->total = cpts[CP_USER] + cpts[CP_NICE]
	 + cpts[CP_SYS] + cpts[CP_IDLE];

}
