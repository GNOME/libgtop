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

#ifndef __GLIBTOP_BACKEND_COMMON_H__
#define __GLIBTOP_BACKEND_COMMON_H__

#include <glibtop.h>
#include <glibtop/global.h>

#include <glibtop/open.h>
#include <glibtop/glibtop-backend.h>
#include <glibtop/glibtop-backend-info.h>

typedef struct _backend_server_private backend_server_private;

struct _backend_server_private
{
    u_int64_t flags;
    int input [2];		/* Pipe client <- server */
    int output [2];		/* Pipe client -> server */
    pid_t pid;			/* PID of the server */

    glibtop_server *server;
};

void *
glibtop_call_i (glibtop_server *server, backend_server_private *priv,
		unsigned command, size_t send_size, const void *send_ptr,
		size_t data_size, const void *data_ptr,
		size_t recv_size, void *recv_ptr,
		int *retval_ptr);

void
glibtop_read_i (backend_server_private *priv, size_t size, void *buf);

void *
glibtop_read_data_i (backend_server_private *priv);

void
glibtop_write_i (backend_server_private *priv, size_t size, const void *buf);

#endif

