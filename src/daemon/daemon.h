/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef __GLIBTOP_DAEMON_H__
#define __GLIBTOP_DAEMON_H__

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/gnuserv.h>

#include <glibtop/open.h>
#include <glibtop/union.h>
#include <glibtop/xmalloc.h>
#include <glibtop/version.h>
#include <glibtop/command.h>
#include <glibtop/parameter.h>

#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <syslog.h>

BEGIN_LIBGTOP_DECLS

/* Some don't have LOG_PERROR */
#ifndef LOG_PERROR
#define LOG_PERROR 0
#endif

#if defined(HAVE_GETDTABLESIZE)
#define GET_MAX_FDS() getdtablesize()
#else
/* Fallthrough case - please add other #elif cases above
   for different OS's as necessary */
#define GET_MAX_FDS() 256
#endif

#define _offset_union(p)	((char *) &resp->u.p - (char *) resp)
#define _offset_data(p)		_offset_union (data.p)

#define MSG_BUFSZ		sizeof (struct _glibtop_ipc_message)
#define MSG_MSGSZ		(MSG_BUFSZ - sizeof (long))

void handle_parent_connection (int s);
void handle_slave_connection (int input, int output);
void handle_slave_command (glibtop_command *cmnd, glibtop_response *resp,
			   const void *parameter);

void do_output (int s, glibtop_response *resp, off_t offset,
		size_t data_size, const void *data);
int do_read (int s, void *ptr, size_t total_size);

void syslog_message (int priority, char *format, ...);
void syslog_io_message (int priority, char *format, ...);

extern int enable_debug;
extern int verbose_output;

END_LIBGTOP_DECLS

#endif
