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

#include <config.h>
#include <glibtop/msg_limits.h>

#include <sys/ipc.h>
#include <sys/msg.h>

static const unsigned long _glibtop_sysdeps_msg_limits =
(1L << GLIBTOP_IPC_MSGPOOL) + (1L << GLIBTOP_IPC_MSGMAP) +
(1L << GLIBTOP_IPC_MSGMAX) + (1L << GLIBTOP_IPC_MSGMNB) +
(1L << GLIBTOP_IPC_MSGMNI) + (1L << GLIBTOP_IPC_MSGSSZ) +
(1L << GLIBTOP_IPC_MSGTQL);

/* Init function. */

void
_glibtop_init_msg_limits_s (glibtop *server)
{
	server->sysdeps.msg_limits = _glibtop_sysdeps_msg_limits;
}

/* Provides information about sysv ipc limits. */

void
glibtop_get_msg_limits_s (glibtop *server, glibtop_msg_limits *buf)
{
	struct msginfo msginfo;

	memset (buf, 0, sizeof (glibtop_msg_limits));

	msgctl (0, IPC_INFO, (void*) &msginfo);

	buf->msgpool = msginfo.msgpool;
	buf->msgmap = msginfo.msgmap;
	buf->msgmax = msginfo.msgmax;
	buf->msgmnb = msginfo.msgmnb;
	buf->msgmni = msginfo.msgmni;
	buf->msgssz = msginfo.msgssz;
	buf->msgtql = msginfo.msgtql;
	buf->flags = _glibtop_sysdeps_msg_limits;
}
