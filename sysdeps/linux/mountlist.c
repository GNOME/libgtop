/* mountlist.c -- return a list of mounted filesystems
   Copyright (C) 1991, 1992 Free Software Foundation, Inc.

   This file is a fork of sysdeps/common/mountlist.c.

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


/*
Local Variables:
mode: C
c-file-style: "gnu"
End:
*/

#include <config.h>

#include <stdio.h>
#include <mntent.h>

#include <glibtop.h>
#include <glibtop/mountlist.h>
#include <glibtop/error.h>


/* IgnoreList */


typedef struct
{
  GHashTable *table_fstype;
  GHashTable *table_mntdir;
} IgnoreList;


static IgnoreList*
ignore_list_new(void)
{
  IgnoreList* ig;
  ig = g_new(IgnoreList, 1);
  ig->table_fstype = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
  ig->table_mntdir = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
  return ig;
}


static void
ignore_list_delete(IgnoreList* ig)
{
  if (ig) {
    g_hash_table_destroy(ig->table_fstype);
    g_hash_table_destroy(ig->table_mntdir);
    g_free(ig);
  }
}


static void
ignore_list_add_fstype(IgnoreList* ig, const char* fstype)
{
  g_hash_table_insert(ig->table_fstype, g_strdup(fstype), GINT_TO_POINTER(1));
}

static void
ignore_list_add_mntdir(IgnoreList* ig, const char* mntdir)
{
  g_hash_table_insert(ig->table_mntdir, g_strdup(mntdir), GINT_TO_POINTER(1));
}

static gboolean
ignore_list_has_fstype(IgnoreList* ig, const char* fstype)
{
  gpointer data;
  data = g_hash_table_lookup(ig->table_fstype, fstype);
  return data != NULL;
}

static gboolean
ignore_list_has_mntdir(IgnoreList* ig, const char* mntdir)
{
  gpointer data;
  data = g_hash_table_lookup(ig->table_mntdir, mntdir);
  return data != NULL;
}


/* ~IgnoreList */


static gboolean
ignore_fs(const char *mntdir, const char *fstype, IgnoreList** ig)
{
  if (!*ig) {
    FILE* fs;
    char line[128];

    *ig = ignore_list_new();

    ignore_list_add_fstype(*ig, "none");

    if ((fs = fopen("/proc/filesystems", "r")) != NULL) {
      while (fgets(line, sizeof line, fs)) {
	if (!strncmp(line, "nodev", 5) && strncmp(line+strlen(line)-4, "zfs", 3)) {
	  char *type;
	  type = g_strstrip(line + 5);
	  ignore_list_add_fstype(*ig, type);
	}
      }
      fclose(fs);
    }

    if ((fs = fopen("/run/mount/utab", "r")) != NULL) {
	size_t len = 0;
	char *uline = NULL;

	while (getline(&uline, &len, fs) != -1) {
	  if (strstr(uline, "x-gdu.hide")) {
	    char * pch = strtok (uline, " ");
	    while (pch != NULL) {
	      if (!strncmp(pch, "TARGET=", 7)) {
		ignore_list_add_mntdir(*ig, pch+7);
	      }
	      pch = strtok (NULL, " ");
	    }
	  }
	}

	free(uline);
	fclose(fs);
    }
  }

  return ignore_list_has_fstype(*ig, fstype) || ignore_list_has_mntdir(*ig, mntdir);
}

glibtop_mountentry *
glibtop_get_mountlist_s(glibtop *server, glibtop_mountlist *buf, int all_fs)
{
  const struct mntent *mnt;
  FILE *fp;
  GArray* entries;
  IgnoreList* ig = NULL;

  memset(buf, 0, sizeof(glibtop_mountlist));

  /* wild guess, preallocate 8 entries
     on a desktop, almost everyone has / and a tmpfs for udev
     if all_fs, there are also proc, sys, fuse, binfmt, etc */
  entries = g_array_sized_new(FALSE, FALSE, sizeof(glibtop_mountentry), 8);

  if (!(fp = setmntent(MOUNTED, "r"))) {
      glibtop_warn_io_r(server, "Could not open %s", MOUNTED);
      goto out;
    }

  while ((mnt = getmntent(fp)))
    {
      glibtop_mountentry *me;
      const char *devopt;
      gsize len;

      if (!all_fs && ignore_fs(mnt->mnt_dir, mnt->mnt_type, &ig))
	continue;

      len = entries->len;
      g_array_set_size(entries, len + 1);
      me = &g_array_index(entries, glibtop_mountentry, len);

      g_strlcpy(me->devname, mnt->mnt_fsname, sizeof me->devname);
      g_strlcpy(me->mountdir, mnt->mnt_dir, sizeof me->mountdir);
      g_strlcpy(me->type, mnt->mnt_type, sizeof me->type);
      devopt = strstr(mnt->mnt_opts, "dev=");
      me->dev = (devopt ? strtoull(devopt + sizeof "dev=", NULL, 0) : (dev_t)-1);
    }

    endmntent(fp);


  out:
    ignore_list_delete(ig);

    buf->size = sizeof(glibtop_mountentry);
    buf->number = entries->len;
    buf->total = buf->number * buf->size;
    buf->flags = (1 << GLIBTOP_MOUNTLIST_SIZE)
    | (1 << GLIBTOP_MOUNTLIST_NUMBER)
    | (1 << GLIBTOP_MOUNTLIST_TOTAL);

  return (glibtop_mountentry*) g_array_free(entries, FALSE);
}
