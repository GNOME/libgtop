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
(1 << GLIBTOP_PROC_MEM_SIZE) +
(1 << GLIBTOP_PROC_MEM_VSIZE) +
(1 << GLIBTOP_PROC_MEM_SHARE) +
(1 << GLIBTOP_PROC_MEM_RESIDENT) +
(1 << GLIBTOP_PROC_MEM_RSS) +
(1 << GLIBTOP_PROC_MEM_RSS_RLIM);

#ifndef LOG1024
#define LOG1024		10
#endif

/* these are for getting the memory statistics */
static int pageshift;		/* log base 2 of the pagesize */

/* define pagetok in terms of pageshift */
#define pagetok(size) ((size) << pageshift)

/* Init function. */

void
glibtop_init_proc_mem_p (glibtop *server)
{
	register int pagesize;

	server->sysdeps.proc_mem = _glibtop_sysdeps_proc_mem;

	/* get the page size with "getpagesize" and calculate pageshift
	 * from it */
	pagesize = getpagesize ();
	pageshift = 0;
	while (pagesize > 1) {
		pageshift++;
		pagesize >>= 1;
	}

	/* we only need the amount of log(2)1024 for our conversion */
	pageshift -= LOG1024;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_mem_p (glibtop *server, glibtop_proc_mem *buf,
			pid_t pid)
{
	struct kinfo_proc *pinfo;
	struct user *u_addr = (struct user *)USRSTACK;
	struct vm_map_entry entry, *first;
	struct vmspace *vms, vmspace;
	struct vm_object object;
	struct plimit plimit;
	struct vnode vnode;
	struct inode inode;
	int count;

	glibtop_suid_enter (server);

	/* Get the process data */
	pinfo = kvm_getprocs (server->machine.kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count < 1))
		glibtop_error_io_r (server, "kvm_getprocs (proclist)");

	if (kvm_read (server->machine.kd,
		      (unsigned long) pinfo [0].kp_proc.p_limit,
		      (char *) &plimit, sizeof (plimit)) != sizeof (plimit))
		glibtop_error_io_r (server, "kvm_read (plimit)");

	buf->rss_rlim = (u_int64_t) 
		(plimit.pl_rlimit [RLIMIT_RSS].rlim_cur);
	
	vms = &pinfo [0].kp_eproc.e_vm;

	buf->vsize = buf->size = (u_int64_t) pagetok
		(vms->vm_tsize + vms->vm_dsize + vms->vm_ssize) << LOG1024;
	
	buf->resident = buf->rss = (u_int64_t) pagetok
		(vms->vm_rssize) << LOG1024;

	/* Now we get the shared memory. */

	if (kvm_read (server->machine.kd,
		      (unsigned long) pinfo [0].kp_proc.p_vmspace,
		      (char *) &vmspace, sizeof (vmspace)) != sizeof (vmspace))
		glibtop_error_io_r (server, "kvm_read (vmspace)");

	first = vmspace.vm_map.header.next;

	if (kvm_read (server->machine.kd,
		      (unsigned long) vmspace.vm_map.header.next,
		      (char *) &entry, sizeof (entry)) != sizeof (entry))
		glibtop_error_io_r (server, "kvm_read (entry)");

	/* Walk through the `vm_map_entry' list ... */

	/* I tested this a few times with `mmap'; as soon as you write
	 * to the mmap'ed area, the object type changes from OBJT_VNODE
	 * to OBJT_DEFAULT so if seems this really works. */

	while (entry.next != first) {
		if (kvm_read (server->machine.kd,
			      (unsigned long) entry.next,
			      &entry, sizeof (entry)) != sizeof (entry))
			glibtop_error_io_r (server, "kvm_read (entry)");

		if (entry.eflags & (MAP_ENTRY_IS_A_MAP|MAP_ENTRY_IS_SUB_MAP))
			continue;

		if (!entry.object.vm_object)
			continue;

		/* We're only interested in `vm_object's */

		if (kvm_read (server->machine.kd,
			      (unsigned long) entry.object.vm_object,
			      &object, sizeof (object)) != sizeof (object))
			glibtop_error_io_r (server, "kvm_read (object)");

		/* If the object is of type vnode, add its size */

		if (object.type != OBJT_VNODE)
			continue;

		buf->share += object.un_pager.vnp.vnp_size;
	}

	glibtop_suid_leave (server);

	buf->flags = _glibtop_sysdeps_proc_mem;
}
