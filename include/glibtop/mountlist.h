/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef __GLIBTOP_MOUNTLIST_H__
#define __GLIBTOP_MOUNTLIST_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_MOUNTLIST_NUMBER	0
#define GLIBTOP_MOUNTLIST_TOTAL		1
#define GLIBTOP_MOUNTLIST_SIZE		2

#define GLIBTOP_MAX_MOUNTLIST		3

typedef struct _glibtop_mountentry	glibtop_mountentry;

typedef struct _glibtop_mountlist	glibtop_mountlist;

struct _glibtop_mountentry
{
	guint64 dev;
	char devname [GLIBTOP_MOUNTENTRY_LEN+1];
	char mountdir [GLIBTOP_MOUNTENTRY_LEN+1];
	char type [GLIBTOP_MOUNTENTRY_LEN+1];
};

struct _glibtop_mountlist
{
	guint64	flags;
	guint64 number;			/* GLIBTOP_MOUNTLIST_NUMBER	*/
	guint64 total;			/* GLIBTOP_MOUNTLIST_TOTAL	*/
	guint64 size;			/* GLIBTOP_MOUNTLIST_SIZE	*/
};

/**
 * glibtop_get_mountlist:
 * @buf:
 * @all_fs:
 *
 * Returns: (transfer full):
 */

/**
 * glibtop_get_mountlist_r:
 * @buf: (out):
 * @all_fs:
 *
 * Returns: (transfer none):
 */

/**
 * glibtop_get_mountlist_s:
 * @buf: (out):
 * @all_fs:
 *
 * Returns: (transfer none):
 */

/**
 * glibtop_get_mountlist_l:
 * @buf: (out):
 * @all_fs:
 *
 * Returns: (transfer none):
 */

glibtop_mountentry *
glibtop_get_mountlist(glibtop_mountlist *buf, int all_fs);

#define glibtop_get_mountlist_r		glibtop_get_mountlist_s

glibtop_mountentry *
glibtop_get_mountlist_l (glibtop *server, glibtop_mountlist *buf, int all_fs);

glibtop_mountentry *
glibtop_get_mountlist_s (glibtop *server, glibtop_mountlist *buf, int all_fs);

GType     glibtop_mountlist_get_type (void) G_GNUC_CONST;
GType     glibtop_mountentry_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif
