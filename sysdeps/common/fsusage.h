/* fsusage.h -- declarations for filesystem space usage info
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
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* Space usage statistics for a filesystem.  Blocks are 512-byte. */
struct fs_usage
{
    u_int64_t fsu_blocks;	/* Total blocks. */
    u_int64_t fsu_bfree;	/* Free blocks available to superuser. */
    u_int64_t fsu_bavail;	/* Free blocks available to non-superuser. */
    u_int64_t fsu_files;	/* Total file nodes. */
    u_int64_t fsu_ffree;	/* Free file nodes. */
};

