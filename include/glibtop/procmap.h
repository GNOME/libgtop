/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef __GLIBTOP_PROC_MAP_H__
#define __GLIBTOP_PROC_MAP_H__

#include <glibtop.h>
#include <glibtop/global.h>

__BEGIN_DECLS

#define GLIBTOP_PROC_MAP_NUMBER		0
#define GLIBTOP_PROC_MAP_TOTAL		1
#define GLIBTOP_PROC_MAP_SIZE		2

#define GLIBTOP_MAX_PROC_MAP		3

#define GLIBTOP_MAP_ENTRY_START		1
#define GLIBTOP_MAP_ENTRY_END		2
#define GLIBTOP_MAP_ENTRY_OFFSET	3
#define GLIBTOP_MAP_ENTRY_PERM		4
#define GLIBTOP_MAP_ENTRY_INODE		5
#define GLIBTOP_MAP_ENTRY_DEVICE	6
#define GLIBTOP_MAP_ENTRY_FILENAME	7

#define GLIBTOP_MAX_MAP_ENTRY		8

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
	u_int64_t flags, start, end, offset, perm, inode, device;
	char filename [GLIBTOP_MAP_FILENAME_LEN+1];
};

struct _glibtop_proc_map
{
	u_int64_t	flags,
		number,			/* GLIBTOP_PROC_MAP_NUMBER	*/
		total,			/* GLIBTOP_PROC_MAP_TOTAL	*/
		size;			/* GLIBTOP_PROC_MAP_SIZE	*/
};

#define glibtop_get_proc_map(proc_map,pid) glibtop_get_proc_map_l(glibtop_global_server, proc_map, pid)

#if GLIBTOP_SUID_PROC_MAP
#define glibtop_get_proc_map_r		glibtop_get_proc_map_p
#else
#define glibtop_get_proc_map_r		glibtop_get_proc_map_s
#endif

glibtop_map_entry *
glibtop_get_proc_map_l (glibtop *server, glibtop_proc_map *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_MAP
void glibtop_init_proc_map_p (glibtop *server);

glibtop_map_entry *
glibtop_get_proc_map_p (glibtop *server, glibtop_proc_map *buf, pid_t pid);
#else
void glibtop_init_proc_map_s (glibtop *server);

glibtop_map_entry *
glibtop_get_proc_map_s (glibtop *server, glibtop_proc_map *buf, pid_t pid);
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_proc_map [];
extern const unsigned glibtop_types_proc_map [];
extern const char *glibtop_labels_proc_map [];
extern const char *glibtop_descriptions_proc_map [];

#endif

__END_DECLS

#endif
