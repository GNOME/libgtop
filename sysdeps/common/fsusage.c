/* fsusage.c -- return space usage of mounted filesystems
   Copyright (C) 1991, 1992, 1996, 1998, 1999, 2002, 2003 Free Software
   Foundation, Inc.

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
# include <config.h>
#endif

#include <glibtop.h>
#include <glibtop/fsusage.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif
#ifndef CHAR_BIT
# define CHAR_BIT 8
#endif

#ifdef HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

#ifdef HAVE_SYS_MOUNT_H
# include <sys/mount.h>
#endif

#ifdef HAVE_SYS_VFS_H
# include <sys/vfs.h>
#endif

#ifdef HAVE_SYS_FS_S5PARAM_H	/* Fujitsu UXP/V */
# include <sys/fs/s5param.h>
#endif

#if defined HAVE_SYS_FILSYS_H && !defined _CRAY
# include <sys/filsys.h>	/* SVR2 */
#endif

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#ifdef HAVE_SYS_STATFS_H
# include <sys/statfs.h>
#endif

#ifdef HAVE_DUSTAT_H		/* AIX PS/2 */
# include <sys/dustat.h>
#endif

#ifdef HAVE_SYS_STATVFS_H		/* SVR4 */
# include <sys/statvfs.h>
int statvfs (const char *path, struct statvfs *buf);
#endif

/* Many space usage primitives use all 1 bits to denote a value that is
   not applicable or unknown.  Propagate this information by returning
   a guint64 value that is all 1 bits if X is all 1 bits, even if X
   is unsigned and narrower than guint64.  */
#define PROPAGATE_ALL_ONES(x) \
  ((sizeof (x) < sizeof (guint64) \
    && (~ (x) == (sizeof (x) < sizeof (int) \
		  ? - (1 << (sizeof (x) * CHAR_BIT)) \
		  : 0))) \
   ? G_MAXUINT64 : (x))

/* Extract the top bit of X as an guint64 value.  */
#define EXTRACT_TOP_BIT(x) ((x) \
			    & ((guint64) 1 << (sizeof (x) * CHAR_BIT - 1)))

/* If a value is negative, many space usage primitives store it into an
   integer variable by assignment, even if the variable's type is unsigned.
   So, if a space usage variable X's top bit is set, convert X to the
   guint64 value V such that (- (guint64) V) is the negative of
   the original value.  If X's top bit is clear, just yield X.
   Use PROPAGATE_TOP_BIT if the original value might be negative;
   otherwise, use PROPAGATE_ALL_ONES.  */
#define PROPAGATE_TOP_BIT(x) ((x) | ~ (EXTRACT_TOP_BIT (x) - 1))

/* Fill in the fields of FSP with information about space usage for
   the filesystem on which PATH resides.
   DISK is the device on which PATH is mounted, for space-getting
   methods that need to know it.
   Return 0 if successful, -1 if not.  When returning -1, ensure that
   ERRNO is either a system error value, or zero if DISK is NULL
   on a system that requires a non-NULL value.  */


static const unsigned long _glibtop_sysdeps_fsusage =
(1L << GLIBTOP_FSUSAGE_BLOCKS) + (1L << GLIBTOP_FSUSAGE_BFREE)
+ (1L << GLIBTOP_FSUSAGE_BAVAIL) + (1L << GLIBTOP_FSUSAGE_FILES)
+ (1L << GLIBTOP_FSUSAGE_FFREE) + (1L << GLIBTOP_FSUSAGE_BLOCK_SIZE);


/*
 * _glibtop_get_fsusage_read_write
 * New function to retrieve total read and write
 *
 * Each arch should have its own function()
 * and the proper #define. This is more readable than one single
 * function full of #something where everything is mixed.
 * These functions are private.
 *
 * void  _glibtop_<arch>_get_fsusage_read_write(glibtop*server,
 *                                              glibtop_fsusage *buf,
 *                                              const char *path);
 *
 * TODO: split this file properly, is possible
 */

#ifdef linux
void
_glibtop_linux_get_fsusage_read_write(glibtop *server,
				      glibtop_fsusage *buf,
				      const char *path);

static inline void
_glibtop_get_fsusage_read_write(glibtop *server,
				glibtop_fsusage *buf,
				const char *path)
{
	_glibtop_linux_get_fsusage_read_write(server, buf, path);
}


#elif defined(__FreeBSD__)
void
_glibtop_freebsd_get_fsusage_read_write(glibtop *server,
					glibtop_fsusage *buf,
					const char *path);

#define _glibtop_get_fsusage_read_write(S, B, P) \
	_glibtop_freebsd_get_fsusage_read_write(S, B, P)

#elif defined(__OpenBSD__)
void
_glibtop_openbsd_get_fsusage_read_write(glibtop *server,
					glibtop_fsusage *buf,
					const char *path);

#define _glibtop_get_fsusage_read_write(S, B, P) \
	_glibtop_openbsd_get_fsusage_read_write(S, B, P)

#else /* default fallback */
#warning glibtop_get_fsusage .read .write are not implemented.
static inline void
_glibtop_get_fsusage_read_write(glibtop *server,
				glibtop_fsusage *buf,
				const char *path)
{
  /* NOOP */
}
#endif /* default fallback */

/* end _glibtop_get_fsusage_read_write */



void
glibtop_get_fsusage_s (glibtop *server, glibtop_fsusage *buf,
		       const char *path)
{
#if defined STAT_STATFS3_OSF1
  struct statfs fsd;
#elif defined STAT_STATFS2_FS_DATA  /* Ultrix */
  struct fs_data fsd;
#elif defined STAT_STATFS2_BSIZE    /* 4.3BSD, SunOS 4, HP-UX, AIX */
  struct statfs fsd;
#elif defined STAT_STATVFS          /* SVR4 */
  struct statvfs fsd;
#elif defined STAT_STATFS2_FSIZE    /* 4.4BSD */
  struct statfs fsd;
#elif defined STAT_STATFS4         /* SVR3, Dynix, Irix, AIX */
  struct stafs fsd;
#endif

  glibtop_init_r (&server, 0, 0);

  memset (buf, 0, sizeof (glibtop_fsusage));

#ifdef STAT_STATFS3_OSF1

  if (statfs (path, &fsd, sizeof (struct statfs)) != 0)
    return;

  buf->block_size = PROPAGATE_ALL_ONES (fsd.f_fsize);

#endif /* STAT_STATFS3_OSF1 */

#ifdef STAT_STATFS2_FS_DATA	/* Ultrix */

  if (statfs (path, &fsd) != 1)
    return;

  buf->block_size = 1024;
  buf->blocks = PROPAGATE_ALL_ONES (fsd.fd_req.btot);
  buf->bfree = PROPAGATE_ALL_ONES (fsd.fd_req.bfree);
  buf->bavail = PROPAGATE_TOP_BIT (fsd.fd_req.bfreen);
  /* buf->bavail_top_bit_set = EXTRACT_TOP_BIT (fsd.fd_req.bfreen) != 0; */
  buf->files = PROPAGATE_ALL_ONES (fsd.fd_req.gtot);
  buf->ffree = PROPAGATE_ALL_ONES (fsd.fd_req.gfree);

#endif /* STAT_STATFS2_FS_DATA */

#ifdef STAT_STATFS2_BSIZE	/* 4.3BSD, SunOS 4, HP-UX, AIX */

  if (statfs (path, &fsd) < 0)
    return;

  buf->block_size = PROPAGATE_ALL_ONES (fsd.f_bsize);

# ifdef STATFS_TRUNCATES_BLOCK_COUNTS

  /* In SunOS 4.1.2, 4.1.3, and 4.1.3_U1, the block counts in the
     struct statfs are truncated to 2GB.  These conditions detect that
     truncation, presumably without botching the 4.1.1 case, in which
     the values are not truncated.  The correct counts are stored in
     undocumented spare fields.  */
  if (fsd.f_blocks == 0x7fffffff / fsd.f_bsize && fsd.f_spare[0] > 0)
    {
      fsd.f_blocks = fsd.f_spare[0];
      fsd.f_bfree = fsd.f_spare[1];
      fsd.f_bavail = fsd.f_spare[2];
    }
# endif /* STATFS_TRUNCATES_BLOCK_COUNTS */

#endif /* STAT_STATFS2_BSIZE */

#ifdef STAT_STATFS2_FSIZE	/* 4.4BSD */

  if (statfs (path, &fsd) < 0)
    return;

  buf->block_size = PROPAGATE_ALL_ONES (fsd.f_fsize);

#endif /* STAT_STATFS2_FSIZE */

#ifdef STAT_STATFS4		/* SVR3, Dynix, Irix, AIX */

# if !_AIX && !defined _SEQUENT_ && !defined DOLPHIN
#  define f_bavail f_bfree
# endif

  if (statfs (path, &fsd, sizeof fsd, 0) < 0)
    return;

  /* Empirically, the block counts on most SVR3 and SVR3-derived
     systems seem to always be in terms of 512-byte blocks,
     no matter what value f_bsize has.  */
# if _AIX || defined _CRAY
   buf->block_size = PROPAGATE_ALL_ONES (fsd.f_bsize);
# else
   buf->block_size = 512;
# endif

#endif /* STAT_STATFS4 */

#ifdef STAT_STATVFS		/* SVR4 */
   /* Linux, Solaris */

  if (statvfs (path, &fsd) < 0)
    return;

#if (defined(sun) || defined(__sun)) && (defined(__SVR4) || defined(__svr4__)) \
	|| defined(__FreeBSD__) || defined(__OpenBSD__)
  /* Solaris but not SunOS and FreeBSD */
  buf->block_size = fsd.f_frsize;
#else
  /* else, including Linux */
  buf->block_size = fsd.f_bsize;
#endif

#endif /* STAT_STATVFS */

#if !defined STAT_STATFS2_FS_DATA && !defined STAT_READ_FILSYS
				/* !Ultrix && !SVR2 */
  /* Linux */

  buf->blocks = fsd.f_blocks;
  buf->bfree  = fsd.f_bfree;
  buf->bavail = (fsd.f_bavail > fsd.f_bfree) ? 0 : fsd.f_bavail;
  buf->files  = fsd.f_files;
  buf->ffree  = fsd.f_ffree;

#endif /* not STAT_STATFS2_FS_DATA && not STAT_READ_FILSYS */

  buf->flags = _glibtop_sysdeps_fsusage;

  /* setting additional flags is delegated */
  _glibtop_get_fsusage_read_write(server, buf, path);
}

#if defined _AIX && defined _I386
/* AIX PS/2 does not supply statfs.  */

static int
statfs (const char *path, struct statfs *fsb)
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
