/* mountlist.c -- return a list of mounted filesystems
   Copyright (C) 1991, 1992 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#if defined(STDC_HEADERS) || defined(HAVE_STRING_H)
#include <string.h>
#else
#include <strings.h>
#endif

#include <glibtop.h>
#include <glibtop/mountlist.h>

/* A mount table entry. */
struct mount_entry
{
  char *me_devname;             /* Device node pathname, including "/dev/". */
  char *me_mountdir;            /* Mount point directory pathname. */
  char *me_type;                /* "nfs", "4.2", etc. */
  dev_t me_dev;                 /* Device number of me_mountdir. */
  struct mount_entry *me_next;
};


static struct mount_entry *read_filesystem_list (void);

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#if defined (MOUNTED_GETFSSTAT)	/* __alpha running OSF_1 */
#  include <sys/mount.h>
#  include <sys/fs_types.h>
#endif /* MOUNTED_GETFSSTAT */

#ifdef MOUNTED_GETMNTENT1	/* 4.3BSD, SunOS, HP-UX, Dynix, Irix.  */
#include <mntent.h>
#if !defined(MOUNTED)
#  if defined(MNT_MNTTAB)	/* HP-UX.  */
#    define MOUNTED MNT_MNTTAB
#  endif
#  if defined(MNTTABNAME)	/* Dynix.  */
#    define MOUNTED MNTTABNAME
#  endif
#endif
#endif

#ifdef MOUNTED_GETMNTINFO	/* 4.4BSD.  */
#include <sys/mount.h>
#endif

#ifdef MOUNTED_GETMNT		/* Ultrix.  */
#include <sys/mount.h>
#include <sys/fs_types.h>
#endif

#ifdef MOUNTED_FREAD		/* SVR2.  */
#include <mnttab.h>
#endif

#ifdef MOUNTED_FREAD_FSTYP	/* SVR3.  */
#include <mnttab.h>
#include <sys/fstyp.h>
#include <sys/statfs.h>
#endif

#ifdef MOUNTED_LISTMNTENT
#include <mntent.h>
#endif

#ifdef MOUNTED_GETMNTENT2	/* SVR4.  */
#include <sys/mnttab.h>
#endif

#ifdef MOUNTED_VMOUNT		/* AIX.  */
#include <fshelp.h>
#include <sys/vfs.h>
#endif

#ifdef DOLPHIN
/* So special that it's not worth putting this in autoconf.  */
#undef MOUNTED_FREAD_FSTYP
#define MOUNTED_GETMNTTBL
#endif


#if defined (MOUNTED_GETMNTINFO) && !defined (__NetBSD__) && !defined (__OpenBSD__) && !defined(__FreeBSD__)
static const char *
fstype_to_string (short t)
{
  switch (t)
    {
#ifdef MOUNT_PC
    case MOUNT_PC:
      return "pc";
#endif
#ifdef MOUNT_MFS
    case MOUNT_MFS:
      return "mfs";
#endif
#ifdef MOUNT_LO
    case MOUNT_LO:
      return "lo";
#endif
#ifdef MOUNT_TFS
    case MOUNT_TFS:
      return "tfs";
#endif
#ifdef MOUNT_TMP
    case MOUNT_TMP:
      return "tmp";
#endif
#ifdef MOUNT_UFS
   case MOUNT_UFS:
     return "ufs" ;
#endif
#ifdef MOUNT_NFS
   case MOUNT_NFS:
     return "nfs" ;
#endif
#ifdef MOUNT_MSDOS
   case MOUNT_MSDOS:
     return "msdos" ;
#endif
#ifdef MOUNT_LFS
   case MOUNT_LFS:
     return "lfs" ;
#endif
#ifdef MOUNT_LOFS
   case MOUNT_LOFS:
     return "lofs" ;
#endif
#ifdef MOUNT_FDESC
   case MOUNT_FDESC:
     return "fdesc" ;
#endif
#ifdef MOUNT_PORTAL
   case MOUNT_PORTAL:
     return "portal" ;
#endif
#ifdef MOUNT_NULL
   case MOUNT_NULL:
     return "null" ;
#endif
#ifdef MOUNT_UMAP
   case MOUNT_UMAP:
     return "umap" ;
#endif
#ifdef MOUNT_KERNFS
   case MOUNT_KERNFS:
     return "kernfs" ;
#endif
#ifdef MOUNT_PROCFS
   case MOUNT_PROCFS:
     return "procfs" ;
#endif
#ifdef MOUNT_AFS
   case MOUNT_AFS:
     return "afs" ;
#endif
#ifdef MOUNT_CD9660
   case MOUNT_CD9660:
     return "cd9660" ;
#endif
#ifdef MOUNT_UNION
   case MOUNT_UNION:
     return "union" ;
#endif
#ifdef MOUNT_DEVFS
   case MOUNT_DEVFS:
     return "devfs" ;
#endif
#ifdef MOUNT_EXT2FS
   case MOUNT_EXT2FS:
     return "ext2fs" ;
#endif
    default:
      return "?";
    }
}
#endif /* MOUNTED_GETMNTINFO */

#ifdef MOUNTED_VMOUNT		/* AIX.  */
static const char *
fstype_to_string (int t)
{
  struct vfs_ent *e;

  e = getvfsbytype (t);
  if (!e || !e->vfsent_name)
    return "none";
  else
    return e->vfsent_name;
}
#endif /* MOUNTED_VMOUNT */

/* Return a list of the currently mounted filesystems, or NULL on error.
   Add each entry to the tail of the list so that they stay in order.
*/

static struct mount_entry *
read_filesystem_list (void)
{
  struct mount_entry *mount_list;
  struct mount_entry *me;
  struct mount_entry *mtail;

  /* Start the list off with a dummy entry. */
  me = g_new (struct mount_entry, 1);
  me->me_next = NULL;
  mount_list = mtail = me;

#ifdef MOUNTED_LISTMNTENT
  {
    struct tabmntent *mntlist, *p;
    struct mntent *mnt;
    struct mount_entry *me;

    /* the third and fourth arguments could be used to filter mounts,
       but Crays doesn't seem to have any mounts that we want to
       remove. Specifically, automount create normal NFS mounts.
       */

    if(listmntent(&mntlist, KMTAB, NULL, NULL) < 0)
      return NULL;
    p = mntlist;
    while(p){
      mnt = p->ment;
      me = (struct mount_entry*) g_malloc(sizeof (struct mount_entry));
      me->me_devname = g_strdup(mnt->mnt_fsname);
      me->me_mountdir = g_strdup(mnt->mnt_dir);
      me->me_type = g_strdup(mnt->mnt_type);
      me->me_dev = -1;
      me->me_next = NULL;
      mtail->me_next = me;
      mtail = me;
      p = p->next;
    }
    freemntlist(mntlist);
  }
#endif

#ifdef MOUNTED_GETMNTENT1   /* Linux, 4.3BSD, SunOS, HP-UX, Dynix, Irix.  */
  {
    const struct mntent *mnt;
    FILE *fp;
    const char *devopt;

    fp = setmntent (MOUNTED, "r");
    if (fp == NULL)
      return NULL;

    while ((mnt = getmntent (fp)))
      {
	me = g_new(struct mount_entry, 1);
	me->me_devname = g_strdup (mnt->mnt_fsname);
	me->me_mountdir = g_strdup (mnt->mnt_dir);
	me->me_type = g_strdup (mnt->mnt_type);
	devopt = strstr (mnt->mnt_opts, "dev=");
	if (devopt)
	    me->me_dev = (dev_t) strtoull( devopt + 4, NULL, 0);
	else
	    me->me_dev = (dev_t) -1;	/* Magic; means not known yet. */

	me->me_next = NULL;

	/* Add to the linked list. */
	mtail->me_next = me;
	mtail = me;
      }

    if (endmntent (fp) == 0)
      return NULL;
  }
#endif /* MOUNTED_GETMNTENT1. */

#ifdef MOUNTED_GETMNTINFO	/* 4.4BSD.  */
  {
    struct statfs *fsp;
    int entries;

    entries = getmntinfo (&fsp, MNT_NOWAIT);
    if (entries < 0)
      return NULL;
    while (entries-- > 0)
      {
	me = (struct mount_entry *) g_malloc (sizeof (struct mount_entry));
	me->me_devname = g_strdup (fsp->f_mntfromname);
	me->me_mountdir = g_strdup (fsp->f_mntonname);
#if defined(__NetBSD__) || defined(__OpenBSD__) || defined(__FreeBSD__)
	me->me_type = g_strdup (fsp->f_fstypename);
#else
	me->me_type = g_strdup (fstype_to_string (fsp->f_type));
#endif
	me->me_dev = (dev_t) -1;	/* Magic; means not known yet. */
	me->me_next = NULL;

	/* Add to the linked list. */
	mtail->me_next = me;
	mtail = me;
	fsp++;
      }
  }
#endif /* MOUNTED_GETMNTINFO */

#ifdef MOUNTED_GETMNT		/* Ultrix.  */
  {
    int offset = 0;
    int val;
    struct fs_data fsd;

    while ((val = getmnt (&offset, &fsd, sizeof (fsd), NOSTAT_MANY,
			  (char *) 0)) > 0)
      {
	me = (struct mount_entry *) g_malloc (sizeof (struct mount_entry));
	me->me_devname = g_strdup (fsd.fd_req.devname);
	me->me_mountdir = g_strdup (fsd.fd_req.path);
	me->me_type = g_strdup (gt_names[fsd.fd_req.fstype]);
	me->me_dev = fsd.fd_req.dev;
	me->me_next = NULL;

	/* Add to the linked list. */
	mtail->me_next = me;
	mtail = me;
      }
    if (val < 0)
      return NULL;
  }
#endif /* MOUNTED_GETMNT. */

#if defined (MOUNTED_GETFSSTAT)	/* __alpha running OSF_1 */
  {
    int numsys, counter, bufsize;
    struct statfs *stats;

    numsys = getfsstat ((struct statfs *)0, 0L, MNT_WAIT);
    if (numsys < 0)
      return (NULL);

    bufsize = (1 + numsys) * sizeof (struct statfs);
    stats = (struct statfs *) g_malloc (bufsize);
    numsys = getfsstat (stats, bufsize, MNT_WAIT);

    if (numsys < 0)
      {
	g_free (stats);
	return (NULL);
      }

    for (counter = 0; counter < numsys; counter++)
      {
	me = (struct mount_entry *) g_malloc (sizeof (struct mount_entry));
	me->me_devname = g_strdup (stats[counter].f_mntfromname);
	me->me_mountdir = g_strdup (stats[counter].f_mntonname);
	me->me_type = g_strdup (mnt_names[stats[counter].f_type]);
	me->me_dev = (dev_t) -1;	/* Magic; means not known yet. */
	me->me_next = NULL;

	/* Add to the linked list. */
	mtail->me_next = me;
	mtail = me;
      }

    g_free (stats);
  }
#endif /* MOUNTED_GETFSSTAT */

#if defined (MOUNTED_FREAD) || defined (MOUNTED_FREAD_FSTYP) /* SVR[23].  */
  {
    struct mnttab mnt;
    FILE *fp;
    fp = fopen ("/etc/mnttab", "r");
    if (fp == NULL)
      return NULL;

    while (fread (&mnt, sizeof mnt, 1, fp) > 0)
      {
	me = (struct mount_entry *) g_malloc (sizeof (struct mount_entry));
#ifdef GETFSTYP			/* SVR3.  */
	me->me_devname = g_strdup (mnt.mt_dev);
#else
	me->me_devname = g_strdup_printf("/dev/%s", mnt.mt_dev);
#endif
	me->me_mountdir = g_strdup (mnt.mt_filsys);
	me->me_dev = (dev_t) -1;	/* Magic; means not known yet. */

#ifdef GETFSTYP			/* SVR3.  */
	  {
	    struct statfs fsd;
	    char typebuf[FSTYPSZ];

	    if (statfs (me->me_mountdir, &fsd, sizeof fsd, 0) != -1
		&& sysfs (GETFSTYP, fsd.f_fstyp, typebuf) != -1)
	      me->me_type = g_strdup (typebuf);
	  }
#else
	me->me_type = g_strdup ("");
#endif

	me->me_next = NULL;

	/* Add to the linked list. */
	mtail->me_next = me;
	mtail = me;
      }

    if (fclose (fp) == EOF)
      return NULL;
  }
#endif /* MOUNTED_FREAD || MOUNTED_FREAD_FSTYP.  */

#ifdef MOUNTED_GETMNTTBL	/* DolphinOS goes it's own way */
  {
    struct mntent **mnttbl=getmnttbl(),**ent;
    for (ent=mnttbl;*ent;ent++)
      {
	me = (struct mount_entry *) g_malloc (sizeof (struct mount_entry));
	me->me_devname = g_strdup ( (*ent)->mt_resource);
	me->me_mountdir = g_strdup( (*ent)->mt_directory);
	me->me_type =  g_strdup ((*ent)->mt_fstype);
	me->me_dev = (dev_t) -1;	/* Magic; means not known yet. */
	me->me_next = NULL;

	/* Add to the linked list. */
	mtail->me_next = me;
	mtail = me;
      }
    endmnttbl();
  }
#endif

#ifdef MOUNTED_GETMNTENT2	/* SVR4.  */
  {
    struct mnttab mnt;
    FILE *fp;
    int ret;

    fp = fopen (MNTTAB, "r");
    if (fp == NULL)
      return NULL;

    while ((ret = getmntent (fp, &mnt)) == 0)
      {
	me = (struct mount_entry *) g_malloc (sizeof (struct mount_entry));
	me->me_devname = g_strdup (mnt.mnt_special);
	me->me_mountdir = g_strdup (mnt.mnt_mountp);
	me->me_type = g_strdup (mnt.mnt_fstype);
	me->me_dev = (dev_t) -1;	/* Magic; means not known yet. */
	me->me_next = NULL;

	/* Add to the linked list. */
	mtail->me_next = me;
	mtail = me;
      }

   if (fclose (fp) == EOF)
      return NULL;
    if (ret > 0)
      return NULL;
  }
#endif /* MOUNTED_GETMNTENT2.  */

#ifdef MOUNTED_VMOUNT		/* AIX.  */
  {
    int bufsize;
    char *entries, *thisent;
    struct vmount *vmp;

    /* Ask how many bytes to allocate for the mounted filesystem info.  */
    mntctl (MCTL_QUERY, sizeof bufsize, (struct vmount *) &bufsize);
    entries = g_malloc (bufsize);

    /* Get the list of mounted filesystems.  */
    mntctl (MCTL_QUERY, bufsize, (struct vmount *) entries);

    for (thisent = entries; thisent < entries + bufsize;
	 thisent += vmp->vmt_length)
      {
	vmp = (struct vmount *) thisent;
	me = (struct mount_entry *) g_malloc (sizeof (struct mount_entry));
	if (vmp->vmt_flags & MNT_REMOTE)
	  {
	    /* Prepend the remote pathname.  */
	    me->me_devname = \
	      g_strdup_printf("%s:%s",
			      thisent + vmp->vmt_data[VMT_HOSTNAME].vmt_off
			      thisent + vmp->vmt_data[VMT_OBJECT  ].vmt_off);
	  }
	else
	  {
	    me->me_devname = g_strdup (thisent +
				      vmp->vmt_data[VMT_OBJECT].vmt_off);
	  }
	me->me_mountdir = g_strdup (thisent + vmp->vmt_data[VMT_STUB].vmt_off);
	me->me_type = g_strdup (fstype_to_string (vmp->vmt_gfstype));
	me->me_dev = (dev_t) -1; /* vmt_fsid might be the info we want.  */
	me->me_next = NULL;

	/* Add to the linked list. */
	mtail->me_next = me;
	mtail = me;
      }
    g_free (entries);
  }
#endif /* MOUNTED_VMOUNT. */

  /* Free the dummy head. */
  me = mount_list;
  mount_list = mount_list->me_next;
  g_free (me);
  return mount_list;
}


static gboolean ignore_mount_entry(const struct mount_entry *me)
{
	/* keep sorted */
	static const char ignored[][12] = {
		"autofs",
		"binfmt_misc",
		"ctfs",
		"devfs",
		"devpts",
		"fusectl",
		"linprocfs",
		"mfs",
		"mntfs",
		"mqueue",
		"none",
		"nsfd",
		"objfs",
		"openpromfs",
		"proc",
		"procfs",
		"rpc_pipefs",
		"securityfs",
		"supermount",
		"sysfs",
		"tmpfs",
		"unknown",
		"usbdevfs",
		"usbfs"
	};

	typedef int (*Comparator)(const void*, const void*);

	return bsearch(me->me_type,
		       ignored, G_N_ELEMENTS(ignored), sizeof ignored[0],
		       (Comparator) strcmp) != NULL;
}


glibtop_mountentry *
glibtop_get_mountlist_s (glibtop *server, glibtop_mountlist *buf, int all_fs)
{
	struct mount_entry *entries, *cur, *next;

	GArray *mount_array = g_array_new(FALSE, FALSE,
					  sizeof(glibtop_mountentry));

	glibtop_init_r (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_mountlist));

	/* Read filesystem list. */

	if((entries = read_filesystem_list ()) == NULL)
		return NULL;

	for (cur = &entries[0]; cur != NULL; cur = next) {

		if(all_fs || !ignore_mount_entry(cur)) {
			/* add a new glibtop_mountentry */
			glibtop_mountentry e;

			g_strlcpy(e.devname,  cur->me_devname,  sizeof e.devname);
			g_strlcpy(e.mountdir, cur->me_mountdir, sizeof e.mountdir);
			g_strlcpy(e.type,     cur->me_type,     sizeof e.type);
			e.dev = cur->me_dev;

			g_array_append_val(mount_array, e);
		}

		/* free current mount_entry and move to the next */
		next = cur->me_next;
		g_free(cur->me_devname);
		g_free(cur->me_mountdir);
		g_free(cur->me_type);
		g_free(cur);
	}

	buf->size   = sizeof (glibtop_mountentry);
	buf->number = mount_array->len;
	buf->total  = buf->number * buf->size;

	buf->flags  = (1 << GLIBTOP_MOUNTLIST_SIZE)
	  | (1 << GLIBTOP_MOUNTLIST_NUMBER)
	  | (1 << GLIBTOP_MOUNTLIST_TOTAL);

	return (glibtop_mountentry*) g_array_free(mount_array, FALSE);
}
