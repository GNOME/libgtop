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
#include <glibtop/procmap.h>

#include <glibtop_suid.h>

#include <kvm.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/resource.h>
#include <uvm/uvm_extern.h>

#include <sys/vnode.h>
#include <sys/mount.h>
#include <ufs/ufs/quota.h>
#include <ufs/ufs/inode.h>

#include <sys/ucred.h>
#include <sys/sysctl.h>

/* XXX until uvm gets cleaned up */
#include <sys/mutex.h>
typedef int boolean_t;

#define _UVM_UVM_AMAP_I_H_ 1
#define _UVM_UVM_MAP_I_H_ 1
#include <uvm/uvm.h>

static const unsigned long _glibtop_sysdeps_proc_map =
(1L << GLIBTOP_PROC_MAP_TOTAL) + (1L << GLIBTOP_PROC_MAP_NUMBER) +
(1L << GLIBTOP_PROC_MAP_SIZE);

static const unsigned long _glibtop_sysdeps_map_entry =
(1L << GLIBTOP_MAP_ENTRY_START) + (1L << GLIBTOP_MAP_ENTRY_END) +
(1L << GLIBTOP_MAP_ENTRY_OFFSET) + (1L << GLIBTOP_MAP_ENTRY_PERM) +
(1L << GLIBTOP_MAP_ENTRY_INODE) + (1L << GLIBTOP_MAP_ENTRY_DEVICE);

/*
 * Don't implement address comparison.
 */
static __inline int
no_impl(const void *p, const void *q)
{
        abort(); /* Should not be called. */
        return 0;
}

RBT_PROTOTYPE(uvm_map_addr, vm_map_entry, daddrs.addr_entry, no_impl);
RBT_GENERATE(uvm_map_addr, vm_map_entry, daddrs.addr_entry, no_impl);

/* Local helper functions. */

ssize_t	load_vmmap_entries(glibtop*, unsigned long, struct vm_map_entry*,
	    struct vm_map_entry*);
void	unload_vmmap_entries(struct vm_map_entry *);

/* Init function. */

void
_glibtop_init_proc_map_p (glibtop *server)
{
	server->sysdeps.proc_map = _glibtop_sysdeps_proc_map;
}

/*
 * Download vmmap_entries from the kernel into our address space.
 * We fix up the addr tree while downloading.
 *
 * Returns: the size of the tree on succes, or -1 on failure.
 * On failure, *rptr needs to be passed to unload_vmmap_entries to free
 * the lot.
 */
ssize_t
load_vmmap_entries(glibtop *server, unsigned long kptr,
    struct vm_map_entry *rptr, struct vm_map_entry *parent)
{
	struct vm_map_entry *entry;
	unsigned long left_kptr, right_kptr;
	ssize_t left_sz;
	ssize_t right_sz;

	if (kptr == 0)
		return 0;

	/* Need space. */
	entry = malloc(sizeof(*entry));
	if (entry == NULL)
		return -1;

	/* Download entry at kptr. */
	if (kvm_read (server->machine->kd, kptr,
	    (char *)entry, sizeof(*entry)) != sizeof(*entry)) {
		free(entry);
		return -1;
	}

	/*
	 * Update addr pointers to have sane values in this address space.
	 * We save the kernel pointers in {left,right}_kptr, so we have them
	 * available to download children.
	 */
	left_kptr = (unsigned long) RBT_LEFT(uvm_map_addr, entry);
	right_kptr = (unsigned long) RBT_RIGHT(uvm_map_addr, entry);
	entry->daddrs.addr_entry.rbt_left =
	    entry->daddrs.addr_entry.rbt_right = NULL;
	/* Fill in parent pointer. */
	entry->daddrs.addr_entry.rbt_parent = &parent->daddrs.addr_entry;

	/*
	 * Consistent state reached, fill in *rptr.
	 */
	rptr = entry;

	/*
	 * Download left, right.
	 * On failure, our map is in a state that can be handled by
	 * unload_vmmap_entries.
	 */
	left_sz = load_vmmap_entries(server, left_kptr,
	    RBT_LEFT(uvm_map_addr, entry), entry);
	if (left_sz == -1)
		return -1;
	right_sz = load_vmmap_entries(server, right_kptr,
	    RBT_RIGHT(uvm_map_addr, entry), entry);
	if (right_sz == -1)
		return -1;

	return 1 + left_sz + right_sz;
}

/*
 * Free the vmmap entries in the given tree.
 */
void
unload_vmmap_entries(struct vm_map_entry *entry)
{
	if (entry == NULL)
		return;

	unload_vmmap_entries(RBT_LEFT(uvm_map_addr, entry));
	unload_vmmap_entries(RBT_RIGHT(uvm_map_addr, entry));
	free(entry);
}

/*
 * Provides detailed information about a process.
 * Due to the fact we are only requested info about one process, it's possible
 * the process has been reaped before we get to kvm_getprocs. Tough luck.
 */

glibtop_map_entry *
glibtop_get_proc_map_p (glibtop *server, glibtop_proc_map *buf,
			pid_t pid)
{
	struct kinfo_proc *pinfo;
	struct vm_map_entry *entry;
	struct uvm_map_addr root;
	struct vmspace vmspace;
	struct vnode vnode;
	struct inode inode;
	ssize_t nentries;
	GArray *maps = g_array_sized_new(FALSE, FALSE,
					 sizeof(glibtop_map_entry),
					 100);
	int count = 0;

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_PROC_MAP), 0);

	memset (buf, 0, sizeof (glibtop_proc_map));

	/* It does not work for the swapper task. */
	if (pid == 0) return (glibtop_map_entry*) g_array_free(maps, TRUE);

	glibtop_suid_enter (server);

	/* Get the process data */
	pinfo = kvm_getprocs (server->machine->kd, KERN_PROC_PID, pid, sizeof(struct kinfo_proc), &count);
	if (pinfo == NULL) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		glibtop_suid_leave (server);
		return (glibtop_map_entry*) g_array_free(maps, TRUE);
	}

	/* Now we get the memory maps. */

	if (kvm_read (server->machine->kd,
		      (unsigned long) pinfo [0].p_vmspace,
		      (char *) &vmspace, sizeof (vmspace)) != sizeof (vmspace)) {
			glibtop_warn_io_r (server, "kvm_read (vmspace)");
			glibtop_suid_leave (server);
			return NULL;
	}

	RBT_INIT(uvm_map_addr, &root);
	nentries = load_vmmap_entries(server,
	    (unsigned long) RBT_ROOT(uvm_map_addr, &vmspace.vm_map.addr),
	    RBT_ROOT(uvm_map_addr, &root), NULL);
	if (nentries == -1) {
		unload_vmmap_entries(RBT_ROOT(uvm_map_addr, &root));
		glibtop_error_io_r (server, "kvm_read (entry)");
	}

	/* Allocate space. */

	buf->number = nentries;
	buf->size = sizeof (glibtop_map_entry);

	buf->total = buf->number * buf->size;

	buf->flags = _glibtop_sysdeps_proc_map;

	/* Walk through the `vm_map_entry' list ... */

	/* I tested this a few times with `mmap'; as soon as you write
	 * to the mmap'ed area, the object type changes from OBJT_VNODE
	 * to OBJT_DEFAULT so it seems this really works.
	 */

	RBT_FOREACH(entry, uvm_map_addr, &root) {
		glibtop_map_entry *mentry;
		unsigned long inum, dev;
		guint len;

 		if (UVM_ET_ISSUBMAP(entry))
			continue;
		if (!entry->object.uvm_obj)
			continue;

		/* We're only interested in vnodes */

		if (kvm_read (server->machine->kd,
			      (unsigned long) entry->object.uvm_obj,
			      &vnode, sizeof (vnode)) != sizeof (vnode)) {
			glibtop_warn_io_r (server, "kvm_read (vnode)");
			unload_vmmap_entries(RBT_ROOT(uvm_map_addr, &root));
			glibtop_suid_leave (server);
			return (glibtop_map_entry*) g_array_free(maps, TRUE);
		}

#if defined(UVM_VNODE_VALID)
		if (!vnode.v_uvm.u_flags & UVM_VNODE_VALID)
			continue;
#endif
		if ((vnode.v_type != VREG) || (vnode.v_tag != VT_UFS) ||
		    !vnode.v_data) continue;

		if (kvm_read (server->machine->kd,
			      (unsigned long) vnode.v_data,
			      &inode, sizeof (inode)) != sizeof (inode)) {
			glibtop_warn_io_r (server, "kvm_read (inode)");
			unload_vmmap_entries(RBT_ROOT(uvm_map_addr, &root));
			glibtop_suid_leave (server);
			return (glibtop_map_entry*) g_array_free(maps, TRUE);
		}

		inum  = inode.i_number;
		dev = inode.i_dev;

		len = maps->len;
		g_array_set_size(maps, len + 1);
		mentry = &g_array_index(maps, glibtop_map_entry, len);

		mentry->flags  = _glibtop_sysdeps_map_entry;

		mentry->start  = (guint64) entry->start;
		mentry->end    = (guint64) entry->end;
		mentry->offset = (guint64) entry->offset;
		mentry->device = (guint64) dev;
		mentry->inode  = (guint64) inum;

		mentry->perm   = (guint64) 0;

		if (entry->protection & PROT_READ)
			mentry->perm |= GLIBTOP_MAP_PERM_READ;
		if (entry->protection & PROT_WRITE)
			mentry->perm |= GLIBTOP_MAP_PERM_WRITE;
		if (entry->protection & PROT_EXEC)
			mentry->perm |= GLIBTOP_MAP_PERM_EXECUTE;
	}

	glibtop_suid_leave (server);

	buf->flags = _glibtop_sysdeps_proc_map;

	buf->number = maps->len;
	buf->size = sizeof (glibtop_map_entry);
	buf->total = buf->number * buf->size;

	unload_vmmap_entries(RBT_ROOT(uvm_map_addr, &root));
	return (glibtop_map_entry*) g_array_free(maps, FALSE);
}
