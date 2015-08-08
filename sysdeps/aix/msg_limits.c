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
#include <sys/msg.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/msg_limits.h>

static const unsigned long _glibtop_sysdeps_msg_limits =
(1 << GLIBTOP_IPC_MSGMAX) + (1 << GLIBTOP_IPC_MSGMNB) +
(1 << GLIBTOP_IPC_MSGMNI) + (1 << GLIBTOP_IPC_MSGTQL);

/* Init function. */

void
_glibtop_init_msg_limits_p (glibtop *server)
{
	off_t result;

	result = _glibtop_get_kmem_offset(server, "msginfo");
	if (result <= 0)
	{
		server->sysdeps.sem_limits = 0;
		return;
	}

	server->machine->msginfo_offset = result;

	server->sysdeps.msg_limits = _glibtop_sysdeps_msg_limits;
}

/* Provides information about sysv ipc limits. */

void
glibtop_get_msg_limits_p (glibtop *server, glibtop_msg_limits *buf)
{
	int result;
	struct msginfo msginfo;

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_MSG_LIMITS), 0);

	memset (buf, 0, sizeof (glibtop_msg_limits));

	result = _glibtop_get_kmem_info(server, server->machine->msginfo_offset,
					&msginfo, sizeof(msginfo));

	if (result <= 0)
	{
		glibtop_error_io_r (server, "Cannot read seminfo");

		return;
	}

	buf->msgmax = msginfo.msgmax;
	buf->msgmnb = msginfo.msgmnb;
	buf->msgmni = msginfo.msgmni;
	buf->msgtql = msginfo.msgmnm;

	buf->flags = _glibtop_sysdeps_msg_limits;
}
