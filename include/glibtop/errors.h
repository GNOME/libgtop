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

BEGIN_LIBGTOP_DECLS

#define GLIBTOP_ERROR_NO_ERROR			0
#define GLIBTOP_ERROR_UNKNOWN			1
#define GLIBTOP_ERROR_INVALID_ARGUMENT		2
#define GLIBTOP_ERROR_NO_SUCH_PARAMETER		3
#define GLIBTOP_ERROR_READONLY_VALUE		4
#define GLIBTOP_ERROR_SIZE_MISMATCH		5

#define GLIBTOP_ERROR_SERVER_COMM_FAILURE	6

#define GLIBTOP_ERROR_NO_SUCH_PROCESS		7

#define GLIBTOP_MAX_ERROR			8

char *
glibtop_get_error_string_l (glibtop *server, unsigned error_number);

unsigned
glibtop_get_errno_l (glibtop *server);

END_LIBGTOP_DECLS

#endif
