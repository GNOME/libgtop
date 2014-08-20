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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef __GLIBTOP_DAEMON_H__
#define __GLIBTOP_DAEMON_H__

#include <glibtop.h>
#include <glibtop/gnuserv.h>

#include <glibtop/open.h>
#include <glibtop/union.h>
#include <glibtop/version.h>
#include <glibtop/command.h>
#include <glibtop/parameter.h>

#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <syslog.h>

G_BEGIN_DECLS

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

void syslog_message (int priority, const char *format, ...) G_GNUC_PRINTF(2, 3);
void syslog_io_message (int priority, const char *format, ...) G_GNUC_PRINTF(2, 3);

extern gboolean enable_debug;
extern gboolean verbose_output;

G_END_DECLS

#endif
