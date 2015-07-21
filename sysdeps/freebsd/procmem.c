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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procmem.h>

#include <glibtop_suid.h>

#include <kvm.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/resource.h>
#include <vm/vm_object.h>
#include <vm/vm_map.h>

#include <sys/vnode.h>
#include <ufs/ufs/quota.h>
#include <ufs/ufs/inode.h>

#include <sys/ucred.h>
#include <sys/user.h>
#include <sys/sysctl.h>
#include <vm/vm.h>

static const unsigned long _glibtop_sysdeps_proc_mem =
(1L << GLIBTOP_PROC_MEM_SIZE) +
(1L << GLIBTOP_PROC_MEM_VSIZE) +
(1L << GLIBTOP_PROC_MEM_RESIDENT) +
(1L << GLIBTOP_PROC_MEM_RSS) +
(1L << GLIBTOP_PROC_MEM_RSS_RLIM);

static const unsigned long _glibtop_sysdeps_proc_mem_share =
(1L << GLIBTOP_PROC_MEM_SHARE);

/* define pagetok in terms of pageshift */
#define ps_pgtok(a)     (((a) * getpagesize()) / 1024)

/* Init function. */

void
_glibtop_init_proc_mem_p (glibtop *server)
{
	server->sysdeps.proc_mem = _glibtop_sysdeps_proc_mem;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_mem_p (glibtop *server, glibtop_proc_mem *buf,
			pid_t pid)
{
	struct kinfo_proc *pinfo;
	struct vm_map_entry entry, *first;
	struct vmspace vmspace;
	struct vm_object object;
	int count;

	memset (buf, 0, sizeof (glibtop_proc_mem));

	if (server->sysdeps.proc_mem == 0)
		return;

	/* It does not work for the swapper task. */
	if (pid == 0) return;

	/* Get the process data */
	pinfo = kvm_getprocs (server->machine->kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count < 1)) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		return;
	}

#define        PROC_VMSPACE   ki_vmspace

       buf->rss_rlim = pinfo [0].ki_rssize;

       buf->vsize = buf->size = (guint64)
	       pinfo [0].ki_size;
       buf->resident = buf->rss = (guint64)
	       ps_pgtok (pinfo [0].ki_rssize) * 1024;

       buf->flags |= _glibtop_sysdeps_proc_mem;

#if 0
	/*
	 * It doesn't make any sense to count as shared each and every mmaped file
	 */

	/* Now we get the shared memory. */

	if (kvm_read (server->machine->kd,
		      (unsigned long) pinfo [0].PROC_VMSPACE,
		      (char *) &vmspace, sizeof (vmspace)) != sizeof (vmspace)) {
		glibtop_warn_io_r (server, "kvm_read (vmspace)");
		return;
	}

	first = vmspace.vm_map.header.next;

	if (kvm_read (server->machine->kd,
		      (unsigned long) vmspace.vm_map.header.next,
		      (char *) &entry, sizeof (entry)) != sizeof (entry)) {
		glibtop_warn_io_r (server, "kvm_read (entry)");
		return;
	}

	/* Walk through the `vm_map_entry' list ... */

	/* I tested this a few times with `mmap'; as soon as you write
	 * to the mmap'ed area, the object type changes from OBJT_VNODE
	 * to OBJT_DEFAULT so if seems this really works. */

	while (entry.next != first) {
		if (kvm_read (server->machine->kd,
			      (unsigned long) entry.next,
			      (char *) &entry, sizeof (entry)) != sizeof (entry)) {
			glibtop_warn_io_r (server, "kvm_read (entry)");
			return;
		}

		if (entry.eflags & (MAP_ENTRY_IS_SUB_MAP))
			continue;

		if (!entry.object.vm_object)
			continue;

		/* We're only interested in `vm_object's */

		if (kvm_read (server->machine->kd,
			      (unsigned long) entry.object.vm_object,
			      (char *) &object, sizeof (object)) != sizeof (object)) {
			glibtop_warn_io_r (server, "kvm_read (object)");
			return;
		}

		if (object.type != OBJT_VNODE)
			continue;

		buf->share += object.un_pager.vnp.vnp_size;
	}

	buf->flags |= _glibtop_sysdeps_proc_mem_share;
#endif
}
