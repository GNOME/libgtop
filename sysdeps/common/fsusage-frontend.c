/* fsusage-frontend.c -- return space usage of mounted filesystems.

   Copyright (C) 2003 Ole Laursen.

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

/* This code is simply a front-end for the actual code in fsusage.[ch] which
   is stolen directly from the GNU Coreutils package. */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#if HAVE_CONFIG_H
# include <config.h>
#endif

#if HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif
#ifndef UINTMAX_MAX
# define UINTMAX_MAX ((uintmax_t) -1)
#endif

#include "fsusage.h"

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/fsusage.h>

void
glibtop_get_fsusage_s (glibtop *server, glibtop_fsusage *buf,
		       const char *disk)
{
	struct fs_usage fsp;

	glibtop_init_r (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_fsusage));
	memset (&fsp, 0, sizeof (struct fs_usage));

	if (glibtop_private_get_fs_usage (disk, disk, &fsp) != 0)
		return;

	buf->blocks = fsp.fsu_blocks;
	buf->bfree = fsp.fsu_bfree;
	buf->bavail = fsp.fsu_bavail;
	buf->files = fsp.fsu_files;
	buf->ffree = fsp.fsu_ffree;
	buf->block_size = fsp.fsu_blocksize;
}
