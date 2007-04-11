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
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __GLIBTOP_PROC_MAP_H__
#define __GLIBTOP_PROC_MAP_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_PROC_MAP_NUMBER		0
#define GLIBTOP_PROC_MAP_TOTAL		1
#define GLIBTOP_PROC_MAP_SIZE		2

#define GLIBTOP_MAX_PROC_MAP		3

#define GLIBTOP_MAP_ENTRY_START		0
#define GLIBTOP_MAP_ENTRY_END		1
#define GLIBTOP_MAP_ENTRY_OFFSET	2
#define GLIBTOP_MAP_ENTRY_PERM		3
#define GLIBTOP_MAP_ENTRY_INODE		4
#define GLIBTOP_MAP_ENTRY_DEVICE	5
#define GLIBTOP_MAP_ENTRY_FILENAME	6

/* Smap support */
#define GLIBTOP_MAP_ENTRY_SIZE			 7
#define GLIBTOP_MAP_ENTRY_RSS			 8
#define GLIBTOP_MAP_ENTRY_SHARED_CLEAN		 9
#define GLIBTOP_MAP_ENTRY_SHARED_DIRTY		10
#define GLIBTOP_MAP_ENTRY_PRIVATE_CLEAN		11
#define GLIBTOP_MAP_ENTRY_PRIVATE_DIRTY		12

#define GLIBTOP_MAX_MAP_ENTRY		13

#define GLIBTOP_MAP_FILENAME_LEN	215

#define GLIBTOP_MAP_PERM_READ		1
#define GLIBTOP_MAP_PERM_WRITE		2
#define GLIBTOP_MAP_PERM_EXECUTE	4
#define GLIBTOP_MAP_PERM_SHARED		8
#define GLIBTOP_MAP_PERM_PRIVATE	16

typedef struct _glibtop_map_entry	glibtop_map_entry;

typedef struct _glibtop_proc_map	glibtop_proc_map;

struct _glibtop_map_entry
{
	guint64 flags; 
	guint64 start; 
	guint64 end; 
	guint64 offset; 
	guint64 perm; 
	guint64 inode; 
	guint64 device;
	guint64 size;
	guint64 rss;
	guint64 shared_clean; 
	guint64 shared_dirty;
	guint64 private_clean; 
	guint64 private_dirty;
	char filename [GLIBTOP_MAP_FILENAME_LEN+1];
};

struct _glibtop_proc_map
{
	guint64	flags;
	guint64 number;			/* GLIBTOP_PROC_MAP_NUMBER	*/
	guint64 total;			/* GLIBTOP_PROC_MAP_TOTAL	*/
	guint64 size;			/* GLIBTOP_PROC_MAP_SIZE	*/
};
glibtop_map_entry *
glibtop_get_proc_map(glibtop_proc_map *buf, pid_t pid);


#if GLIBTOP_SUID_PROC_MAP
#define glibtop_get_proc_map_r		glibtop_get_proc_map_p
#else
#define glibtop_get_proc_map_r		glibtop_get_proc_map_s
#endif

glibtop_map_entry *
glibtop_get_proc_map_l (glibtop *server, glibtop_proc_map *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_MAP
void _glibtop_init_proc_map_p (glibtop *server);

glibtop_map_entry *
glibtop_get_proc_map_p (glibtop *server, glibtop_proc_map *buf, pid_t pid);
#else
void _glibtop_init_proc_map_s (glibtop *server);

glibtop_map_entry *
glibtop_get_proc_map_s (glibtop *server, glibtop_proc_map *buf, pid_t pid);
#endif


G_END_DECLS

#endif
