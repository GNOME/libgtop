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
#include <glibtop/procmem.h>

#include <kvm.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/resource.h>
#include <vm/vm_map.h>
#include <machine/pmap.h>

static const unsigned long _glibtop_sysdeps_proc_mem =
(1 << GLIBTOP_PROC_MEM_SIZE) +
(1 << GLIBTOP_PROC_MEM_VSIZE) +
(1 << GLIBTOP_PROC_MEM_RESIDENT) +
/* (1 << GLIBTOP_PROC_MEM_SHARE) + */
(1 << GLIBTOP_PROC_MEM_RSS) +
(1 << GLIBTOP_PROC_MEM_RSS_RLIM);

/* Provides detailed information about a process. */

void
glibtop_get_proc_mem_s (glibtop *server, glibtop_proc_mem *buf,
			pid_t pid)
{
  struct kinfo_proc *pinfo;
  int count;
  struct vmspace vms;
  struct vm_map vmm;
  struct pstats ps;
  int f;

  glibtop_init_r(&server, 0, 0);

  memset (buf, 0, sizeof (glibtop_proc_mem));

  if (pid == 0) {
    /* Client is only interested in the flags. */
    buf->flags = _glibtop_sysdeps_proc_mem;
    return;
  }

  f = open("/dev/kmem", O_ORDONLY, NULL);
  if (f == NULL) {
    /* FIXME: error */
  }
  /* Read the vmspace from kernel memeory */
  lseek(f, (long)pinfo[0].kp_proc.p_vmspace, SEEK_SET);
  read(f, &vms, sizeof(vmspace));
  /* Read the vm_map from kernel memeory */
  lseek(f, (long)vms.vm_map, SEEK_SET);
  read(f, &vmm, sizeof(vm_map));
  /* Read the pstats [for the RSS rlimit] from kernel memory. */
  lseek(f, (long)pinfo[0].kp_proc.p_stats, SEEK_SET);
  read(f, &ps, sizeof(pstats));
  close(f);

  /* Get the process information */
  kvm_getprocs(server->machine.kd, KERN_PROC_PID, pid, &count);
  if (count != 1) {
    /* Return no information */
    return;
  }

  /* size: total # of pages in memory
     (segsz_t)pinfo[0].kp_proc.p_vmspace.(vm_tsize + vm_dsize + vm_ssize)
     */
  buf->size = (unsigned long)(vms.vm_tsize + vms.vm_dsize + vms.vm_ssize);
  /* vsize: number of pages of VM
     (vm_size_t)pinfo[0].kp_proc.p_vmspace.vm_map.size
     */
  buf->vsize = (unsigned long)vmm.size;

  /* resident: number of resident (non-swapped) pages [4k]
     (long)pmap_resident_count(pinfo[0]->kp_proc.p_vmspace.vm_map.pmap);
     */
  buf->resident = (unsigned long)pmap_resident_count(vmm.pmap);

  /* share: number of pages shared (mmap'd) memory
     ??? vm_object has this info, but how to get it?
     Even if we could, it's not reachable information.
     */


  /* rss: resident set size 
     (segsz_t)kp_proc.p_vmspace.vm_rssize
     */
  buf->rss = (unsigned long)vms.vm_rssize;
  /* rss_rlim: current rss limit [bytes]
     (rlim_t)kp_proc.p_limit.pl_rlimit[RLIMIT_RSS].rlim_cur
     or
     (long)kp_proc.p_stats->p_ru.ru_maxrss */
  buf->rss_rlim = ps.p_ru.ru_maxrss;

  /* Set the flags */
  buf->flags = _glibtop_sysdeps_proc_mem;

}
