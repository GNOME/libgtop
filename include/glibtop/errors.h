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

#ifndef __GLIBTOP_ERRORS_H__
#define __GLIBTOP_ERRORS_H__

#include <glibtop.h>

#include <glib/gerror.h>

G_BEGIN_DECLS

#define GLIBTOP_ERROR glibtop_error_quark ()

GQuark glibtop_error_quark (void) G_GNUC_CONST;

typedef enum {
    GLIBTOP_ERROR_NO_ERROR = 0,
    GLIBTOP_ERROR_UNKNOWN,
    GLIBTOP_ERROR_INVALID_ARGUMENT,
    GLIBTOP_ERROR_NO_SUCH_PARAMETER,
    GLIBTOP_ERROR_READONLY_VALUE,
    GLIBTOP_ERROR_SIZE_MISMATCH,

    GLIBTOP_ERROR_SERVER_COMM_FAILURE,

    GLIBTOP_ERROR_NO_SUCH_PROCESS,

    GLIBTOP_ERROR_NO_KERNEL_SUPPORT,
    GLIBTOP_ERROR_INCOMPATIBLE_KERNEL,

    GLIBTOP_ERROR_NO_SUCH_BACKEND,
    GLIBTOP_ERROR_NOT_IMPLEMENTED,
    GLIBTOP_ERROR_NO_BACKEND_OPENED,

    GLIBTOP_ERROR_DEMARSHAL_ERROR
} glibtop_error;

void
glibtop_error_vl (glibtop_client *client, glibtop_error code,
		  const char *format, va_list args);

void
glibtop_error_io_vl (glibtop_client *client, glibtop_error code,
		     int io_errno, const char *format, va_list args);

void
glibtop_warn_vl (glibtop_client *client, glibtop_error code,
		 const char *format, va_list args);

void
glibtop_warn_io_vl (glibtop_client *client, glibtop_error code,
		    int io_errno, const char *format, va_list args);

void
glibtop_error_l (glibtop_client *client, glibtop_error code,
		 char *format, ...);

void
glibtop_warn_l (glibtop_client *client, glibtop_error code,
		char *format, ...);

void
glibtop_error_io_l (glibtop_client *client, glibtop_error code,
		    char *format, ...);

void
glibtop_warn_io_l (glibtop_client *client, glibtop_error code,
		   char *format, ...);

G_END_DECLS

#endif
