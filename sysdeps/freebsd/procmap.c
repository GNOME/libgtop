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
#include <glibtop/xmalloc.h>
#include <glibtop/procmap.h>

#include <glibtop_suid.h>

#include <kvm.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/resource.h>
#include <vm/vm_object.h>
#include <vm/vm_prot.h>
#include <vm/vm_map.h>

#include <sys/vnode.h>
#include <sys/mount.h>
#include <ufs/ufs/quota.h>
#include <ufs/ufs/inode.h>

#include <sys/ucred.h>
#include <sys/user.h>
#include <sys/sysctl.h>
#include <vm/vm.h>

static const unsigned long _glibtop_sysdeps_proc_map =
(1 << GLIBTOP_PROC_MAP_TOTAL) + (1 << GLIBTOP_PROC_MAP_NUMBER) +
(1 << GLIBTOP_PROC_MAP_SIZE);

static const unsigned long _glibtop_sysdeps_map_entry =
(1 << GLIBTOP_MAP_ENTRY_START) + (1 << GLIBTOP_MAP_ENTRY_END) +
(1 << GLIBTOP_MAP_ENTRY_OFFSET) + (1 << GLIBTOP_MAP_ENTRY_PERM) +
(1 << GLIBTOP_MAP_ENTRY_INODE) + (1 << GLIBTOP_MAP_ENTRY_DEVICE);

/* Init function. */

void
glibtop_init_proc_map_p (glibtop *server)
{
	server->sysdeps.proc_map = _glibtop_sysdeps_proc_map;
}

/* Provides detailed information about a process. */

glibtop_map_entry *
glibtop_get_proc_map_p (glibtop *server, glibtop_proc_map *buf,
			pid_t pid)
{
	struct kinfo_proc *pinfo;
	struct vm_map_entry entry, *first;
	struct vmspace vmspace;
	struct vm_object object;
	glibtop_map_entry *maps;
	struct vnode vnode;
	struct inode inode;
	struct mount mount;
	int count, i = 0;
	int update = 0;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_MAP), 0);
	
	memset (buf, 0, sizeof (glibtop_proc_map));

	/* It does not work for the swapper task. */
	if (pid == 0) return NULL;
	
	glibtop_suid_enter (server);

	/* Get the process data */
	pinfo = kvm_getprocs (server->machine.kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count < 1))
		glibtop_error_io_r (server, "kvm_getprocs (%d)", pid);

	/* Now we get the memory maps. */

	if (kvm_read (server->machine.kd,
		      (unsigned long) pinfo [0].kp_proc.p_vmspace,
		      (char *) &vmspace, sizeof (vmspace)) != sizeof (vmspace))
		glibtop_error_io_r (server, "kvm_read (vmspace)");

	first = vmspace.vm_map.header.next;

	if (kvm_read (server->machine.kd,
		      (unsigned long) vmspace.vm_map.header.next,
		      (char *) &entry, sizeof (entry)) != sizeof (entry))
		glibtop_error_io_r (server, "kvm_read (entry)");

	/* Allocate space. */

	buf->number = vmspace.vm_map.nentries;
	buf->size = sizeof (glibtop_map_entry);

	buf->total = buf->number * buf->size;

	maps = glibtop_malloc_r (server, buf->total);

	memset (maps, 0, buf->total);

	buf->flags = _glibtop_sysdeps_proc_map;

	/* Walk through the `vm_map_entry' list ... */

	/* I tested this a few times with `mmap'; as soon as you write
	 * to the mmap'ed area, the object type changes from OBJT_VNODE
	 * to OBJT_DEFAULT so if seems this really works. */

	do {
		if (update) {
			if (kvm_read (server->machine.kd,
				      (unsigned long) entry.next,
				      &entry, sizeof (entry)) != sizeof (entry))
				glibtop_error_io_r (server, "kvm_read (entry)");
		} else {
			update = 1;
		}

#ifdef __FreeBSD__
		if (entry.eflags & (MAP_ENTRY_IS_A_MAP|MAP_ENTRY_IS_SUB_MAP))
			continue;
#else
		if (entry.is_a_map || entry.is_sub_map)
			continue;
#endif

		maps [i].flags  = _glibtop_sysdeps_map_entry;

		maps [i].start  = entry.start;
		maps [i].end    = entry.end;
		maps [i].offset = entry.offset;

		maps [i].perm   = 0;

		if (entry.protection & VM_PROT_READ)
			maps [i].perm |= GLIBTOP_MAP_PERM_READ;
		if (entry.protection & VM_PROT_WRITE)
			maps [i].perm |= GLIBTOP_MAP_PERM_WRITE;
		if (entry.protection & VM_PROT_EXECUTE)
			maps [i].perm |= GLIBTOP_MAP_PERM_EXECUTE;

		i++;

		if (!entry.object.vm_object)
			continue;

		/* We're only interested in `vm_object's */

		if (kvm_read (server->machine.kd,
			      (unsigned long) entry.object.vm_object,
			      &object, sizeof (object)) != sizeof (object))
			glibtop_error_io_r (server, "kvm_read (object)");

#ifdef __FreeBSD__
		/* If the object is of type vnode, add its size */

		if (object.type != OBJT_VNODE)
			continue;

		if (!object.handle)
			continue;
		
		if (kvm_read (server->machine.kd,
			      (unsigned long) object.handle,
			      &vnode, sizeof (vnode)) != sizeof (vnode))
			glibtop_error_io_r (server, "kvm_read (vnode)");

		if ((vnode.v_type != VREG) || (vnode.v_tag != VT_UFS) ||
		    !vnode.v_data) continue;

		if (kvm_read (server->machine.kd,
			      (unsigned long) vnode.v_data,
			      &inode, sizeof (inode)) != sizeof (inode))
			glibtop_error_io_r (server, "kvm_read (inode)");

		if (kvm_read (server->machine.kd,
			      (unsigned long) vnode.v_mount,
			      &mount, sizeof (mount)) != sizeof (mount))
			glibtop_error_io_r (server, "kvm_read (mount)");

		maps [i-1].inode  = inode.i_number;
		maps [i-1].device = inode.i_dev;
#endif
	} while (entry.next != first);

	return maps;
}
