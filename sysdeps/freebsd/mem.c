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
#include <glibtop/mem.h>

#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/vmmeter.h>

static const unsigned long _glibtop_sysdeps_mem =
(1 << GLIBTOP_MEM_TOTAL) + (1 << GLIBTOP_MEM_USED) +
(1 << GLIBTOP_MEM_FREE) + (1 << GLIBTOP_MEM_SHARED) +
(1 << GLIBTOP_MEM_BUFFER) + (1 << GLIBTOP_MEM_CACHED);
/* + (1 << GLIBTOP_MEM_USER); */

/* Provides information about memory usage. */

void
glibtop_get_mem_s (glibtop *server, glibtop_mem *buf)
{
  /* for kvm_*, which is needed to get cache and buffer data */
  kvm_t *kd;
  struct nlist nlst[3];
  static unsigned long cnt_offset;
  static unsigned long bufsize_offset;
  struct vmtotal vmt;
  static int bufspace = 0;
  /* for sysctl(3) */
  size_t length_vmt;
  int mib_vm[2];
  struct vmmeter vmm;

  /* Setup nlist array */
  nlst = {
    { "_cnt" },
    { "_bufspace" },
    { 0 }
  }

  /* Setup MIB array for sysctl */
  mib_vm[0] = CTL_VM;
  mib_vm[1] = VM_METER;

  glibtop_init_r (&server, 0, 0);

  memset (buf, 0, sizeof (glibtop_mem));

  /* Get the data from sysctl */
  length_vmt = sizeof(vmt);
  sysctl(mib, 2, &vmt, &length_vmt, NULL, 0);

  /* Get the data from kvm_* */
  kd = kvm_open(NULL, NULL, NULL, OD_RDONLY, "kvm_open");
  kvm_nlist(kd, &nlst);
  cnt_offset = nlst[0].n_value;
  bufspace_offset = nlst[1].n_value;
  kvm_read(kd, cnt_offset, &vmm, sizeof(vmm));
  kvm_read(kd, bufspace_offset, &bufspace, sizeof(bufspace));
  kvm_close(kd);
  
  /* Set the values to return */
  buf->flags = _glibtop_sysdeps_mem;
  /* total */
  buf->total = (unsigned long)vmt.t_vm;
  /* used */
  buf->used = (unsigned long)vmt.t_avm;
  /* free */
  buf->free = (unsigned long)vmt.t_free;
  /* shared */
  buf->shared = (unsigned long)vmt.t_vmshr;
  /* buffer */
  buf->buffer = (unsigned long)bufspace;
  /* cached */
  buf->cache = (unsigned long)(vmm.v_cache_count * vmm.v_page_size);

  /* user */
  /* FIXME: Any way to get or calculate this? */
  
}
