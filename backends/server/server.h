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

#ifndef __GLIBTOP_DAEMON_H__
#define __GLIBTOP_DAEMON_H__

#include <glibtop.h>
#include <glibtop/error.h>

#include <glibtop/open.h>
#include <glibtop/union.h>
#include <glibtop/xmalloc.h>
#include <glibtop/parameter.h>

#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <syslog.h>

#include <glibtop/backend.h>
#include <glibtop-backend-private.h>

#include "command.h"

BEGIN_LIBGTOP_DECLS

/* Some don't have LOG_PERROR */
#ifndef LOG_PERROR
#define LOG_PERROR 0
#endif

#define LIBGTOP_VERSION_STRING "Libgtop %s server version %s (%u,%u,%u,%u)."

#if defined(HAVE_GETDTABLESIZE)
#define GET_MAX_FDS() getdtablesize()
#else
/* Fallthrough case - please add other #elif cases above
   for different OS's as necessary */
#define GET_MAX_FDS() 256
#endif

void
do_output (int s, glibtop_response *resp, const void *recv_ptr,
	   const void *data_ptr);

int
do_read (int s, void *ptr, size_t total_size);

void
handle_slave_connection (int input, int output);

int
glibtop_demarshal_func_i (glibtop *server, glibtop_backend *backend, unsigned command, const void *send_ptr, size_t send_size, void *data_ptr, size_t data_size, void **recv_buf_ptr, size_t *recv_size_ptr, void **recv_data_ptr, size_t *recv_data_size_ptr, int *retval_ptr);

void
glibtop_send_version_i (glibtop *server, int fd);

extern int enable_debug;
extern int verbose_output;

END_LIBGTOP_DECLS

#endif
