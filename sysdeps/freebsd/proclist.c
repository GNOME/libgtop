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
#include <glibtop/proclist.h>

#include <kvm.h>
#include <sys/param.h>
#include <sys/sysctl.h>

#define GLIBTOP_PROCLIST_FLAGS	3

static const unsigned long _glibtop_sysdeps_proclist =
(1 << GLIBTOP_PROCLIST_TOTAL) + (1 << GLIBTOP_PROCLIST_NUMBER) +
(1 << GLIBTOP_PROCLIST_SIZE);

/* Fetch list of currently running processes.
 * The interface of this function is a little bit different from the others:
 * buf->flags is only set if the call succeeded, in this case pids_chain,
 * a list of the pids of all currently running processes is returned,
 * buf->number is the number of elements of this list and buf->size is
 * the size of one single element (sizeof (unsigned)). The total size is
 * stored in buf->total.
 *
 * The calling function has to free the memory to which a pointer is returned.
 *
 * IMPORTANT NOTE:
 *   On error, this function MUST return NULL and set buf->flags to zero !
 *   On success, it returnes a pointer to a list of buf->number elements
 *   each buf->size big. The total size is stored in buf->total.
 * The calling function has to free the memory to which a pointer is returned.
 *
 * On error, NULL is returned and buf->flags is zero. */

unsigned *
glibtop_get_proclist_s (glibtop *server, glibtop_proclist *buf)
{
  struct kinfo_proc *pinfo;
  unsigned *pids=NULL;
  int count;
  int i;

  glibtop_init_r (&server, 0, 0);

  memset (buf, 0, sizeof (glibtop_proclist));

  /* Get the process data */
  pinfo = kvm_getprocs(server->machine.kd, KERN_PROC_ALL, NULL, &count);
  /* Allocate count objects in the pids_chain array
     Same as malloc is pids is NULL, which it is. */
  pids = glibtop_realloc__r(server, pids, count * sizeof(unsigned));
  /* Copy the pids over to this chain */
  for (i=0; i < count; i++) {
    pids[i] = (unsigned)pinfo[i].kp_proc.p_pid;
  } /* end for */
  /* Set the fields in buf */
  buf->number = count;
  buf->size = sizeof(unsigned);
  buf->total = count * sizeof(unsigned);
  buf->flags = _glibtop_sysdeps_proclist;
  /* return the data */
  return pids;
}
