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

#ifndef __GLIBTOP_COMPAT10_H__
#define __GLIBTOP_COMPAT10_H__

#include <glibtop.h>
#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

/* glibtop_cpu */

#define GLIBTOP_XCPU_TOTAL		GLIBTOP_CPU_XCPU_TOTAL
#define GLIBTOP_XCPU_USER		GLIBTOP_CPU_XCPU_USER
#define GLIBTOP_XCPU_NICE		GLIBTOP_CPU_XCPU_NICE
#define GLIBTOP_XCPU_SYS		GLIBTOP_CPU_XCPU_SYS
#define GLIBTOP_XCPU_IDLE		GLIBTOP_CPU_XCPU_IDLE
#define GLIBTOP_XCPU_FLAGS		GLIBTOP_CPU_XCPU_FLAGS

/* glibtop_proclist */

#define GLIBTOP_PROCLIST_NUMBER		GLIBTOP_ARRAY_NUMBER
#define GLIBTOP_PROCLIST_SIZE		GLIBTOP_ARRAY_SIZE
#define GLIBTOP_PROCLIST_TOTAL		GLIBTOP_ARRAY_TOTAL

#define GLIBTOP_MAX_PROCLIST		GLIBTOP_MAX_ARRAY

typedef struct _glibtop_array		glibtop_proclist;

/* glibtop_mountlist */

#define GLIBTOP_MOUNTLIST_NUMBER	GLIBTOP_ARRAY_NUMBER
#define GLIBTOP_MOUNTLIST_SIZE		GLIBTOP_ARRAY_SIZE
#define GLIBTOP_MOUNTLIST_TOTAL		GLIBTOP_ARRAY_TOTAL

#define GLIBTOP_MAX_MOUNTLIST		GLIBTOP_MAX_ARRAY

typedef struct _glibtop_array		glibtop_mountlist;

/* glibtop_proc_args */

#define GLIBTOP_PROC_ARGS_SIZE		0

#define GLIBTOP_MAX_PROC_ARGS		1

typedef struct _glibtop_array		glibtop_proc_args;

/* glibtop_proc_map */

#define GLIBTOP_PROC_MAP_NUMBER		GLIBTOP_ARRAY_NUMBER
#define GLIBTOP_PROC_MAP_SIZE		GLIBTOP_ARRAY_SIZE
#define GLIBTOP_PROC_MAP_TOTAL		GLIBTOP_ARRAY_TOTAL

#define GLIBTOP_MAX_PROC_MAP		GLIBTOP_MAX_ARRAY

typedef struct _glibtop_array		glibtop_proc_map;

/* glibtop_interface_names */

#define GLIBTOP_INTERFACE_NAMES_NUMBER	GLIBTOP_ARRAY_NUMBER
#define GLIBTOP_INTERFACE_NAMES_SIZE	GLIBTOP_ARRAY_SIZE
#define GLIBTOP_INTERFACE_NAMES_TOTAL	GLIBTOP_ARRAY_TOTAL

#define GLIBTOP_MAX_INTERFACE_NAMES	GLIBTOP_MAX_ARRAY

typedef struct _glibtop_array		glibtop_interface_names;

#endif
