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

#ifndef __GLIBTOP_GLIB_ARRAYS_H__
#define __GLIBTOP_GLIB_ARRAYS_H__

#include <glibtop.h>
#include <glibtop/array.h>
#include <glibtop/global.h>
#include <glibtop/xmalloc.h>

#include <glib.h>

BEGIN_LIBGTOP_DECLS

#define glibtop_get_proc_args_as_array(pid) glibtop_get_proc_args_as_array_l(glibtop_global_server, pid)

GPtrArray *
glibtop_get_proc_args_as_array_l (glibtop *server, pid_t pid);

#define glibtop_get_proclist_as_array(which,arg) glibtop_get_proclist_as_array_l(glibtop_global_server, which, arg)

GArray *
glibtop_get_proclist_as_array_l (glibtop *server, int64_t which, int64_t arg);

#define glibtop_get_proc_map_as_array(pid) glibtop_get_proc_map_as_array_l(glibtop_global_server, pid)

GPtrArray *
glibtop_get_proc_map_as_array_l (glibtop *server, pid_t pid);

#define glibtop_get_mountlist_as_array(all_fs) glibtop_get_mountlist_as_array_l(glibtop_global_server, all_fs)

GPtrArray *
glibtop_get_mountlist_as_array_l (glibtop *server, int all_fs);

END_LIBGTOP_DECLS
#endif
