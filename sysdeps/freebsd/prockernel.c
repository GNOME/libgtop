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
#include <glibtop/prockernel.h>

#include <kvm.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/proc.h>
#include <sys/user.h>
#include <machine/pcb.h>
#include <machine/tss.h>

#include <unistd.h>
#include <fcntl.h>

static const unsigned long _glibtop_sysdeps_proc_kernel =
(1 << GLIBTOP_PROC_KERNEL_K_FLAGS) +
(1 << GLIBTOP_PROC_KERNEL_MIN_FLT) +
(1 << GLIBTOP_PROC_KERNEL_MAJ_FLT) +
(1 << GLIBTOP_PROC_KERNEL_CMIN_FLT) +
(1 << GLIBTOP_PROC_KERNEL_CMAJ_FLT) +
(1 << GLIBTOP_PROC_KERNEL_KSTK_ESP) +
(1 << GLIBTOP_PROC_KERNEL_KSTK_EIP) +
(1 << GLIBTOP_PROC_KERNEL_WCHAN);

/* Provides detailed information about a process. */

void
glibtop_get_proc_kernel_s (glibtop *server,
			   glibtop_proc_kernel *buf,
			   pid_t pid)
{
  struct kinfo_proc *pinfo;
  int count;
  struct i386tss *pcb_tss;
  int kmem;
  struct pstats ps;
  struct user usr;

  glibtop_init_r (&server, 0, 0);

  memset (buf, 0, sizeof (glibtop_proc_kernel));

  /* Get the information pertaining to the given PID */
  pinfo = kvm_getprocs(server->machine.kd, KERN_PROC_PID, pid, &count);
  if (count != 1) {
    return; /* the 0-filled struct, since we can't get any info */
  }

  kmem = open("/dev/kmem", O_RDONLY, NULL);
  if (f == NULL) {
    /* FIXME: error */
  } 
  /* Read the p_stats struct from kernel memory */
  lseek(f, (long)pinfo[0].kp_proc.p_stats, SEEK_SET);
  read(f, &ps, sizeof(pstats));
  /* Read the struct at kp_proc.p_addr */
  lseek(f, (long)pinfo[0].kp_proc.p_addr, SEEK_SET);
  read(f, &usr, sizeof(user));
  close(f);


  /* kflags:
     kinfo_proc.e_flag?
     proc.p_flag
     proc.p_stat
     */
  buf->kflags = pinfo[0].kp_eproc.e_flag;

  /* min_flt: rusage.ru_minflt */
  buf->min_flt = ps.p_ru.ru_minflt;
  /* maj_flt: rusage.ru_majflt */
  buf->majflt = ps.p_ru.ru_majflt;
  /* cmin_flt: */
  buf->cmin_flt = ps.p_cru.ru_minflt;
  /* cmaj_flt: */
  buf->cmaj_flt = ps.p_cru.ru_majflt;

  /* kstk_esp: pcb_tss.tss_esp */
  buf->kstk_esp = (unsigned long)usr.u_pcb.pcb_ksp;
  /* kstk_eip: pcb_tss.tss_eip */
  buf->kstk_eip = (unsigned long)usr.u_pcb.pcb_pc;

  /* wchan : kinfo_proc.proc.p_wchan */
  buf->wchan = (unsigned long)pinfo[0].kp_proc.p_wchan;

}
