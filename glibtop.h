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

#include <glibtop_server.h>
#include <glibtop/types.h>

BEGIN_LIBGTOP_DECLS

typedef struct _glibtop			glibtop;
typedef struct _glibtop_server_private	glibtop_server_private;

#include <glibtop/sysdeps.h>
#include <glibtop/errors.h>

#include <glibtop/parameter.h>

#ifdef _IN_LIBGTOP
#include <glibtop-server-private.h>
#endif

struct _glibtop
{
    int refcount;		/* Reference count */
    unsigned flags;
    int ncpu;			/* Number of CPUs, zero if single-processor */
    unsigned long os_version_code;	/* Version code of the operating system */
    const char *name;		/* Program name for error messages */
    unsigned long features;	/* Server is required for this features */
    unsigned long server_port;	/* Port on which daemon is listening */
    glibtop_sysdeps sysdeps;	/* Detailed feature list */
    glibtop_sysdeps required;	/* Required feature list */
    glibtop_sysdeps wanted;	/* We only want this features */
    glibtop_parameter _param;	/* Server parameter */
    glibtop_server_private *_priv;	/* Private data */
    unsigned glibtop_errno;
};

extern glibtop *glibtop_global_server;

extern const unsigned long glibtop_server_features;

#define glibtop_init()	glibtop_init_r(&glibtop_global_server, 0, 0);

#define glibtop_close()	glibtop_close_r(glibtop_global_server);

glibtop *
glibtop_init_r (glibtop **server_ptr,
		unsigned long features,
		unsigned flags);

glibtop *
glibtop_init_s (glibtop **server_ptr,
		unsigned long features,
		unsigned flags);

void
glibtop_server_ref (glibtop *server);

void
glibtop_server_unref (glibtop *server);

END_LIBGTOP_DECLS

#endif
