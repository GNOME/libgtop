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

#ifndef __GLIBTOP_LIMITS_H__
#define __GLIBTOP_LIMITS_H__

#include <glibtop/global.h>

BEGIN_LIBGTOP_DECLS

/* Nobody should really be using more than 64 processors. */
#define GLIBTOP_NCPU		64

/* Maximum number of process groups in glibtop_proc_uid. */
#define GLIBTOP_MAX_GROUPS	32

/* Maximum length of a mountpoint/device name in glibtop_mountlist. */
#define GLIBTOP_MOUNTENTRY_LEN	79

/* Maximum length of a network interface name. */
#define GLIBTOP_INTERFACE_LEN	32

/* This is ((u_int64_t)-1) */
#define GLIBTOP_UNLIMITED	(~(u_int64_t)0)

END_LIBGTOP_DECLS

#endif
