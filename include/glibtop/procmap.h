/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/* $Id$ */

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

#include <glibtop/compat_10.h>
#include <glibtop/array.h>

BEGIN_LIBGTOP_DECLS

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

struct _glibtop_map_entry
{
    u_int64_t flags, start, end, offset, perm, inode, device;
    char filename [GLIBTOP_MAP_FILENAME_LEN+1];
};

#if GLIBTOP_SUID_PROC_MAP
#define glibtop_get_proc_map_r		glibtop_get_proc_map_p
#else
#define glibtop_get_proc_map_r		glibtop_get_proc_map_s
#endif

glibtop_map_entry *
glibtop_get_proc_map_l (glibtop_client *client, glibtop_array *array, pid_t pid);

#if GLIBTOP_SUID_PROC_MAP
int glibtop_init_proc_map_p (glibtop_server *server, glibtop_closure *closure);

glibtop_map_entry *
glibtop_get_proc_map_p (glibtop_server *server, glibtop_closure *closure, glibtop_array *array, pid_t pid);
#else
int glibtop_init_proc_map_s (glibtop_server *server, glibtop_closure *closure);

glibtop_map_entry *
glibtop_get_proc_map_s (glibtop_server *server, glibtop_closure *closure, glibtop_array *array, pid_t pid);
#endif

#ifdef GLIBTOP_NAMES

/* You need to link with -lgtop_names to get this stuff here. */

extern const char *glibtop_names_proc_map [];
extern const unsigned glibtop_types_proc_map [];
extern const char *glibtop_labels_proc_map [];
extern const char *glibtop_descriptions_proc_map [];

#endif

END_LIBGTOP_DECLS

#endif
