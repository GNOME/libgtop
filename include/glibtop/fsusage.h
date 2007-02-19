/* Copyright (C) 1998-99 Martin Baulig

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

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
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __GLIBTOP_FSUSAGE_H__
#define __GLIBTOP_FSUSAGE_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_FSUSAGE_BLOCKS		0
#define GLIBTOP_FSUSAGE_BFREE		1
#define GLIBTOP_FSUSAGE_BAVAIL		2
#define GLIBTOP_FSUSAGE_FILES		3
#define GLIBTOP_FSUSAGE_FFREE		4
#define GLIBTOP_FSUSAGE_BLOCK_SIZE	5
#define GLIBTOP_FSUSAGE_READ           6
#define GLIBTOP_FSUSAGE_WRITE          7
#define GLIBTOP_MAX_FSUSAGE		8


typedef struct _glibtop_fsusage		glibtop_fsusage;

struct _glibtop_fsusage
{
	guint64	flags;
	guint64 blocks;		/* Total number of blocks. */
	guint64 bfree;		/* Free blocks available to superuser. */
	guint64 bavail;		/* Free blocks available to non-superuser. */
	guint64 files;		/* Total file nodes. */
	guint64 ffree;		/* Free file nodes. */
	guint32	block_size;	/* Size of a block in bytes. */
	guint64 read, write;    /* Total blocks read and written */
};

void glibtop_get_fsusage(glibtop_fsusage *buf, const char *mount_dir);

#define glibtop_get_fsusage_r		glibtop_get_fsusage_s

void glibtop_get_fsusage_l (glibtop *server, glibtop_fsusage *buf, const char *mount_dir);
void glibtop_get_fsusage_s (glibtop *server, glibtop_fsusage *buf, const char *mount_dir);


G_END_DECLS

#endif
