/* fsusage.c -- return space usage of mounted filesystems
   Copyright (C) 1991, 1992, 1996 Free Software Foundation, Inc.

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
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include "fsusage.h"

int statfs ();

#if HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

#if HAVE_SYS_MOUNT_H
# include <sys/mount.h>
#endif

#if HAVE_SYS_VFS_H
# include <sys/vfs.h>
#endif

#if HAVE_SYS_FS_S5PARAM_H	/* Fujitsu UXP/V */
# include <sys/fs/s5param.h>
#endif

#if defined (HAVE_SYS_FILSYS_H) && !defined (_CRAY)
# include <sys/filsys.h>	/* SVR2 */
#endif

#if HAVE_FCNTL_H
# include <fcntl.h>
#endif

#if HAVE_SYS_STATFS_H
# include <sys/statfs.h>
#endif

#if HAVE_DUSTAT_H		/* AIX PS/2 */
# include <sys/dustat.h>
#endif

#if HAVE_SYS_STATVFS_H		/* SVR4 */
# include <sys/statvfs.h>
int statvfs ();
#endif

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/fsusage.h>

static int
get_fs_usage (const char *path, const char *disk, struct fs_usage *fsp);

int safe_read ();

/* Return the number of TOSIZE-byte blocks used by
   BLOCKS FROMSIZE-byte blocks, rounding away from zero.
   TOSIZE must be positive.  Return -1 if FROMSIZE is not positive.  */

static u_int64_t
adjust_blocks (blocks, fromsize, tosize)
     u_int64_t blocks;
     int fromsize, tosize;
{
  if (tosize <= 0)
    abort ();
  if (fromsize <= 0)
    return -1;

  if (fromsize == tosize)	/* e.g., from 512 to 512 */
    return blocks;
  else if (fromsize > tosize)	/* e.g., from 2048 to 512 */
	return blocks * (u_int64_t)(fromsize / tosize);
  else				/* e.g., from 256 to 512 */
	return (blocks + (blocks < 0 ? -1 : 1)) / (u_int64_t)(tosize / fromsize);
}

/* Fill in the fields of FSP with information about space usage for
   the filesystem on which PATH resides.
   DISK is the device on which PATH is mounted, for space-getting
   methods that need to know it.
   Return 0 if successful, -1 if not.  When returning -1, ensure that
   ERRNO is either a system error value, or zero if DISK is NULL
   on a system that requires a non-NULL value.  */
static int
get_fs_usage (path, disk, fsp)
     const char *path;
     const char *disk;
     struct fs_usage *fsp;
{
#ifdef STAT_STATFS3_OSF1
# define CONVERT_BLOCKS(B) adjust_blocks ((u_int64_t)(B), fsd.f_fsize, 512)

  struct statfs fsd;

  if (statfs (path, &fsd, sizeof (struct statfs)) != 0)
    return -1;

#endif /* STAT_STATFS3_OSF1 */

#ifdef STAT_STATFS2_FS_DATA	/* Ultrix */
# define CONVERT_BLOCKS(B) adjust_blocks ((u_int64_t)(B), 1024, 512)

  struct fs_data fsd;

  if (statfs (path, &fsd) != 1)
    return -1;
  fsp->fsu_blocks = CONVERT_BLOCKS (fsd.fd_req.btot);
  fsp->fsu_bfree = CONVERT_BLOCKS (fsd.fd_req.bfree);
  fsp->fsu_bavail = CONVERT_BLOCKS (fsd.fd_req.bfreen);
  fsp->fsu_files = fsd.fd_req.gtot;
  fsp->fsu_ffree = fsd.fd_req.gfree;

#endif /* STAT_STATFS2_FS_DATA */

#ifdef STAT_READ_FILSYS		/* SVR2 */
# ifndef SUPERBOFF
#  define SUPERBOFF (SUPERB * 512)
# endif
# define CONVERT_BLOCKS(B) \
    adjust_blocks ((u_int64_t)(B), (fsd.s_type == Fs2b ? 1024 : 512), 512)

  struct filsys fsd;
  int fd;

  if (! disk)
    {
      errno = 0;
      return -1;
    }

  fd = open (disk, O_RDONLY);
  if (fd < 0)
    return -1;
  lseek (fd, (long) SUPERBOFF, 0);
  if (safe_read (fd, (char *) &fsd, sizeof fsd) != sizeof fsd)
    {
      close (fd);
      return -1;
    }
  close (fd);
  fsp->fsu_blocks = CONVERT_BLOCKS (fsd.s_fsize);
  fsp->fsu_bfree = CONVERT_BLOCKS (fsd.s_tfree);
  fsp->fsu_bavail = CONVERT_BLOCKS (fsd.s_tfree);
  fsp->fsu_files = (fsd.s_isize - 2) * INOPB * (fsd.s_type == Fs2b ? 2 : 1);
  fsp->fsu_ffree = fsd.s_tinode;

#endif /* STAT_READ_FILSYS */

#ifdef STAT_STATFS2_BSIZE	/* 4.3BSD, SunOS 4, HP-UX, AIX */
# define CONVERT_BLOCKS(B) adjust_blocks ((u_int64_t)(B), fsd.f_bsize, 512)

  struct statfs fsd;

  if (statfs (path, &fsd) < 0)
    return -1;

# ifdef STATFS_TRUNCATES_BLOCK_COUNTS

  /* In SunOS 4.1.2, 4.1.3, and 4.1.3_U1, the block counts in the
     struct statfs are truncated to 2GB.  These conditions detect that
     truncation, presumably without botching the 4.1.1 case, in which
     the values are not truncated.  The correct counts are stored in
     undocumented spare fields.  */
  if (fsd.f_blocks == 0x1fffff && fsd.f_spare[0] > 0)
    {
      fsd.f_blocks = fsd.f_spare[0];
      fsd.f_bfree = fsd.f_spare[1];
      fsd.f_bavail = fsd.f_spare[2];
    }
# endif /* STATFS_TRUNCATES_BLOCK_COUNTS */

#endif /* STAT_STATFS2_BSIZE */

#ifdef STAT_STATFS2_FSIZE	/* 4.4BSD */
# define CONVERT_BLOCKS(B) adjust_blocks ((u_int64_t)(B), fsd.f_fsize, 512)

  struct statfs fsd;

  if (statfs (path, &fsd) < 0)
    return -1;

#endif /* STAT_STATFS2_FSIZE */

#ifdef STAT_STATFS4		/* SVR3, Dynix, Irix, AIX */
# if _AIX || defined(_CRAY)
#  define CONVERT_BLOCKS(B) adjust_blocks ((u_int64_t)(B), fsd.f_bsize, 512)
#  ifdef _CRAY
#   define f_bavail f_bfree
#  endif
# else
#  define CONVERT_BLOCKS(B) (B)
#  ifndef _SEQUENT_		/* _SEQUENT_ is DYNIX/ptx */
#   ifndef DOLPHIN		/* DOLPHIN 3.8.alfa/7.18 has f_bavail */
#    define f_bavail f_bfree
#   endif
#  endif
# endif

  struct statfs fsd;

  if (statfs (path, &fsd, sizeof fsd, 0) < 0)
    return -1;
  /* Empirically, the block counts on most SVR3 and SVR3-derived
     systems seem to always be in terms of 512-byte blocks,
     no matter what value f_bsize has.  */

#endif /* STAT_STATFS4 */

#ifdef STAT_STATVFS		/* SVR4 */
# define CONVERT_BLOCKS(B) \
    adjust_blocks ((u_int64_t)(B), fsd.f_frsize ? fsd.f_frsize : fsd.f_bsize, 512)

  struct statvfs fsd;

  if (statvfs (path, &fsd) < 0)
    return -1;
  /* f_frsize isn't guaranteed to be supported.  */

#endif /* STAT_STATVFS */

#if !defined(STAT_STATFS2_FS_DATA) && !defined(STAT_READ_FILSYS)
				/* !Ultrix && !SVR2 */

  fsp->fsu_blocks = CONVERT_BLOCKS (fsd.f_blocks);
  fsp->fsu_bfree = CONVERT_BLOCKS (fsd.f_bfree);
  fsp->fsu_bavail = CONVERT_BLOCKS (fsd.f_bavail);
  fsp->fsu_files = fsd.f_files;
  fsp->fsu_ffree = fsd.f_ffree;

#endif /* not STAT_STATFS2_FS_DATA && not STAT_READ_FILSYS */

  return 0;
}

#if defined(_AIX) && defined(_I386)
/* AIX PS/2 does not supply statfs.  */

int
statfs (path, fsb)
     char *path;
     struct statfs *fsb;
{
  struct stat stats;
  struct dustat fsd;

  if (stat (path, &stats))
    return -1;
  if (dustat (stats.st_dev, 0, &fsd, sizeof (fsd)))
    return -1;
  fsb->f_type   = 0;
  fsb->f_bsize  = fsd.du_bsize;
  fsb->f_blocks = fsd.du_fsize - fsd.du_isize;
  fsb->f_bfree  = fsd.du_tfree;
  fsb->f_bavail = fsd.du_tfree;
  fsb->f_files  = (fsd.du_isize - 2) * fsd.du_inopb;
  fsb->f_ffree  = fsd.du_tinode;
  fsb->f_fsid.val[0] = fsd.du_site;
  fsb->f_fsid.val[1] = fsd.du_pckno;
  return 0;
}

#endif /* _AIX && _I386 */

void
glibtop_get_fsusage_s (glibtop *server, glibtop_fsusage *buf,
		       const char *disk)
{
	struct fs_usage fsp;

	glibtop_init_r (&server, 0, 0);
	
	memset (buf, 0, sizeof (glibtop_fsusage));
    memset (&fsp, 0, sizeof (struct fs_usage));
	
	if (get_fs_usage (disk, disk, &fsp))
		return;

	buf->blocks = fsp.fsu_blocks;
	buf->bfree = fsp.fsu_bfree;
	buf->bavail = fsp.fsu_bavail;
	buf->files = fsp.fsu_files;
	buf->ffree = fsp.fsu_ffree;
}
