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

#ifndef __GLIBTOP_H__
#define __GLIBTOP_H__

#ifndef _IN_LIBGTOP
#include <glibtop-config.h>
#endif

#include <glibtop/global.h>
#include <glibtop/limits.h>
#include <glibtop/types.h>

#include <glibtop/glibtop-client.h>

BEGIN_LIBGTOP_DECLS

typedef struct _glibtop			glibtop;

#include <glibtop/sysdeps.h>
#include <glibtop/errors.h>

#include <glibtop/glibtop-server.h>

#ifdef _IN_LIBGTOP
#include <glibtop-server-private.h>
#endif

glibtop *
glibtop_init_r (glibtop **server_ptr,
		unsigned long features,
		unsigned flags);

void
glibtop_init_s (glibtop_server *server_ptr,
		unsigned long features,
		unsigned flags);

END_LIBGTOP_DECLS

#endif
