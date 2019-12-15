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
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/resource.h>
#include <vm/vm_object.h>
#include <vm/vm_map.h>
#include <vm/vm.h>

#define _KVM_VNODE
#include <sys/vnode.h>
#undef _KVM_VNODE

#include <sys/conf.h>
#if (__FreeBSD_version >= 800038) || (__FreeBSD_kernel_version >= 800038)
#define _WANT_FILE
#include <sys/file.h>
#undef _WANT_FILE
#else
#define _KERNEL
#include <sys/file.h>
#undef _KERNEL
#endif
#define _KERNEL
#include <sys/mount.h>
#include <ufs/ufs/quota.h>
#include <ufs/ufs/inode.h>
#include <fs/devfs/devfs.h>
#if (__FreeBSD_version >= 600006) || defined(__FreeBSD_kernel__)
#include <fs/devfs/devfs_int.h>
#endif
#undef _KERNEL


#if (__FreeBSD_version >= 1101001)
#define _KERNEL
#include <ufs/ufs/extattr.h>
#include <ufs/ufs/ufsmount.h>
#undef _KERNEL
#endif


#include <sys/ucred.h>
#include <sys/sysctl.h>

static const unsigned long _glibtop_sysdeps_proc_map =
        (1L << GLIBTOP_PROC_MAP_TOTAL) + (1L << GLIBTOP_PROC_MAP_NUMBER) +
        (1L << GLIBTOP_PROC_MAP_SIZE);

static const unsigned long _glibtop_sysdeps_map_entry =
        (1L << GLIBTOP_MAP_ENTRY_START) + (1L << GLIBTOP_MAP_ENTRY_END) +
        (1L << GLIBTOP_MAP_ENTRY_OFFSET) + (1L << GLIBTOP_MAP_ENTRY_PERM) +
        (1L << GLIBTOP_MAP_ENTRY_INODE) + (1L << GLIBTOP_MAP_ENTRY_DEVICE);

#if (__FreeBSD_version >= 600006) || defined(__FreeBSD_kernel__)
void _glibtop_sysdeps_freebsd_dev_inode (glibtop *server, struct vnode *vnode, struct vnode *vn, guint64 *inum, guint64 *dev);

void
_glibtop_sysdeps_freebsd_dev_inode (glibtop *server, struct vnode *vnode,
                                    struct vnode *vn, guint64 *inum,
                                    guint64 *dev)
{
        char *tagptr;
        char tagstr[12];
        enum FS_TYPE { UNKNOWN, IS_UFS, IS_ZFS };
        int fs_type = UNKNOWN;
        struct inode inode;
        struct cdev_priv priv;
#if __FreeBSD_version < 800039
        struct cdev si;
#endif

        *inum = 0;
        *dev = 0;

        if (kvm_read (server->machine->kd, (gulong) &vnode->v_tag,
 	             (char *) &tagptr, sizeof (tagptr)) != sizeof (tagptr) ||
            kvm_read (server->machine->kd, (gulong) tagptr,
		     (char *) tagstr, sizeof (tagstr)) != sizeof (tagstr))
        {
                glibtop_warn_io_r (server, "kvm_read (tagptr)");
                return;
        }

        tagstr[sizeof(tagstr) - 1] = '\0';

        if (!strcmp(tagstr, "ufs")) {
                fs_type = IS_UFS;
        } else if (!strcmp(tagstr, "zfs")) {
                fs_type = IS_ZFS;
        } else {
                glibtop_warn_io_r (server, "ignoring fstype %s", tagstr);
                return;
        }

        if (kvm_read (server->machine->kd, (gulong) VTOI(vn), (char *) &inode,
 	              sizeof (inode)) != sizeof (inode))
        {
                glibtop_warn_io_r (server, "kvm_read (inode)");
                return;
        }


        if (fs_type == IS_ZFS) {
		/* FIXME: I have no idea about what is the actual layout of what we've read
		   but the inode number is definitely at offset 16, 8 bytes of amd64.
		   *inum = *(guint64*) ((unsigned char*)&inode + 16);
		   So this is really hugly, but I don't have anything better for now.

                   Actually, this looks like a znode_t as described in kernel's zfs_znode.h.
                   I don't have that header file, so let's just mimic that.
                */

                struct my_zfsvfs {
                        /* vfs_t */ void *z_vfs;
                        /* zfsvfs_t */ void *z_parent;
                        /* objset_t */ void *z_os;
                        uint64_t z_root;
                        /* ... */
                };

                typedef struct my_znode {
                        struct my_zfsvfs *z_zfsvfs;
                        /* vnode_t */ void *z_vnode;
                        uint64_t z_id;
                        /* ... */
                } my_znode_t;

                G_STATIC_ASSERT(sizeof(my_znode_t) <= sizeof(struct inode));

                my_znode_t* znode = (my_znode_t*)&inode;
                *inum = znode->z_id;

                struct my_zfsvfs zvfs;

                if (kvm_read(server->machine->kd,
                             (unsigned long)(znode->z_zfsvfs),
                             &zvfs, sizeof zvfs) != sizeof zvfs) {
                        glibtop_warn_io_r(server, "kvm_read (z_zfsvfs)");
                        return;
                }

                *dev = zvfs.z_root;
        }
        else if (fs_type == IS_UFS) {
		/* Set inum as soon as possible, so that if the next kvm_reads fail
		   we still have something */
                *inum = inode.i_number;


#if (__FreeBSD_version >= 1101001)
/*
  The ufs struct inode changed between 11.0 and 11.1.

  commit 20f1e8ac63b58708989267ea34a6aefa90b46577
  Author: kib <kib@FreeBSD.org>
  Date:   Sat Sep 17 16:47:34 2016 +0000

  Reduce size of ufs inode.
  [...]
*/
		struct ufsmount um;

		if (kvm_read(server->machine->kd, (gulong)inode.i_ump, &um, sizeof um) != sizeof um) {
			glibtop_warn_io_r (server, "kvm_read (ufsmount)");
			return;
		}

		if (kvm_read(server->machine->kd, (gulong)cdev2priv(um.um_dev), &priv, sizeof priv) != sizeof priv) {
			glibtop_warn_io_r (server, "kvm_read (priv)");
			return;
		}

		*dev = priv.cdp_inode;

#else /* older versions */
#if (__FreeBSD_version >= 800039) || (__FreeBSD_kernel_version >= 800039)
        if (kvm_read (server->machine->kd, (gulong) cdev2priv(inode.i_dev), (char *) &priv,
		      sizeof (priv))
#else
        if (kvm_read (server->machine->kd, (gulong) inode.i_dev, (char *) &si,
	              sizeof (si)) != sizeof (si) ||
            kvm_read (server->machine->kd, (gulong) si.si_priv, (char *) &priv,
		      sizeof (priv))
#endif
	    != sizeof (priv))
        {
                glibtop_warn_io_r (server, "kvm_read (priv)");
                return;
        }

        *dev = (guint64) priv.cdp_inode;
#endif /* older versions */

	    } /* end-if IS_UFS */
}
#endif

/* Init function. */

void
_glibtop_init_proc_map_p (glibtop *server)
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
        GArray *maps;
        struct vnode vnode;
        int count;
        int update = 0;

        memset (buf, 0, sizeof (glibtop_proc_map));

        /* It does not work for the swapper task. */
        if (pid == 0) return NULL;

        /*return (glibtop_map_entry*) g_array_free(maps, TRUE);*/

        glibtop_suid_enter (server);

        /* Get the process data */
        pinfo = kvm_getprocs (server->machine->kd, KERN_PROC_PID, pid, &count);
        if ((pinfo == NULL) || (count < 1)) {
                glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		glibtop_suid_leave (server);
                return NULL;
        }

        /* Now we get the memory maps. */

        if (kvm_read (server->machine->kd,
                        (gulong) pinfo [0].ki_vmspace,
                        (char *) &vmspace, sizeof (vmspace)) != sizeof (vmspace)) {
                glibtop_warn_io_r (server, "kvm_read (vmspace)");
		glibtop_suid_leave (server);
                return NULL;
        }

#if (__FreeBSD_version >= 1300062)
        first = vmspace.vm_map.header.right;
#else
        first = vmspace.vm_map.header.next;
#endif

        if (kvm_read (server->machine->kd,
#if (__FreeBSD_version >= 1300062)
                        (gulong) vmspace.vm_map.header.right,
#else
                        (gulong) vmspace.vm_map.header.next,
#endif
                        (char *) &entry, sizeof (entry)) != sizeof (entry)) {
                glibtop_warn_io_r (server, "kvm_read (entry)");
		glibtop_suid_leave (server);
                return NULL;
        }

        /* Walk through the `vm_map_entry' list ... */

        /* I tested this a few times with `mmap'; as soon as you write
         * to the mmap'ed area, the object type changes from OBJT_VNODE
         * to OBJT_DEFAULT so if seems this really works. */

        maps = g_array_sized_new(FALSE, FALSE, sizeof(glibtop_map_entry),
                                 vmspace.vm_map.nentries);

        do {
                glibtop_map_entry *mentry;
                guint64 inum, dev;
                guint len;

                if (update) {
                        if (kvm_read (server->machine->kd,
#if (__FreeBSD_version >= 1300062)
                                        (gulong) entry.right,
#else
                                        (gulong) entry.next,
#endif
                                        (char *) &entry, sizeof (entry)) != sizeof (entry)) {
                                glibtop_warn_io_r (server, "kvm_read (entry)");
                                continue;
                        }
                } else {
                        update = 1;
                }

                if (entry.eflags & (MAP_ENTRY_IS_SUB_MAP))
                        continue;

                if (!entry.object.vm_object)
                        continue;

                /* We're only interested in `vm_object's */

                if (kvm_read (server->machine->kd,
                                (gulong) entry.object.vm_object,
                                (char *) &object, sizeof (object)) != sizeof (object)) {
                        glibtop_warn_io_r (server, "kvm_read (object)");
                        continue;
                }

                /* If the object is of type vnode, add its size */

                if (object.type != OBJT_VNODE)
                        continue;

                if (!object.handle)
                        continue;

                if (kvm_read (server->machine->kd,
                                (gulong) object.handle,
                                (char *) &vnode, sizeof (vnode)) != sizeof (vnode)) {
                        glibtop_warn_io_r (server, "kvm_read (vnode)");
                        continue;
                }

                switch (vnode.v_type) {
                case VNON:
                case VBAD:
                        continue;
                default:
#if (__FreeBSD_version < 600006) && !defined(__FreeBSD_kernel__)
                        inum = vnode.v_cachedid;
                        dev = vnode.v_cachedfs;

#else
                        _glibtop_sysdeps_freebsd_dev_inode (server,
					(struct vnode *) object.handle,
					&vnode, &inum, &dev);
#endif
                        break;
                }

                len = maps->len;
                g_array_set_size(maps, len + 1);
                mentry = &g_array_index(maps, glibtop_map_entry, len);

                memset (mentry, 0, sizeof (glibtop_map_entry));

                mentry->flags  = _glibtop_sysdeps_map_entry;
                mentry->start  = (guint64) entry.start;
                mentry->end    = (guint64) entry.end;
                mentry->offset = (guint64) entry.offset;
                mentry->device = (guint64) dev;
                mentry->inode  = (guint64) inum;

                mentry->perm   = (guint64) 0;

                if (entry.protection & VM_PROT_READ)
                        mentry->perm |= GLIBTOP_MAP_PERM_READ;
                if (entry.protection & VM_PROT_WRITE)
                        mentry->perm |= GLIBTOP_MAP_PERM_WRITE;
                if (entry.protection & VM_PROT_EXECUTE)
                        mentry->perm |= GLIBTOP_MAP_PERM_EXECUTE;

#if (__FreeBSD_version >= 1300062)
        } while (entry.right != first);
#else
        } while (entry.next != first);
#endif

        glibtop_suid_leave (server);

        buf->flags = _glibtop_sysdeps_proc_map;

        buf->number = (guint64) maps->len;
        buf->size   = (guint64) sizeof (glibtop_map_entry);
        buf->total  = (guint64) (buf->number * buf->size);

        return (glibtop_map_entry*) g_array_free(maps, FALSE);
}
