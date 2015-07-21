/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, August 1998.

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
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/msg_limits.h>

#include <sys/types.h>
#include <sys/sysctl.h>

static const unsigned long _glibtop_sysdeps_msg_limits =
(1L << GLIBTOP_IPC_MSGMAX) + (1L << GLIBTOP_IPC_MSGMNI) +
(1L << GLIBTOP_IPC_MSGMNB) + (1L << GLIBTOP_IPC_MSGTQL) +
(1L << GLIBTOP_IPC_MSGSSZ) + (1L << GLIBTOP_IPC_MSGPOOL);

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
	size_t len;
	int msgmax, msgmni, msgmnb, msgtql, msgssz, msgseg;

	memset (buf, 0, sizeof (glibtop_msg_limits));

	if (server->sysdeps.msg_limits == 0)
		return;

	len = sizeof (msgseg);
	if (sysctlbyname ("kern.ipc.msgseg", &msgseg, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.msgseg)");
		return;
	}

	len = sizeof (msgssz);
	if (sysctlbyname ("kern.ipc.msgssz", &msgssz, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.msgssz)");
		return;
	}

	len = sizeof (msgtql);
	if (sysctlbyname ("kern.ipc.msgtql", &msgtql, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.msgtql)");
		return;
	}

	len = sizeof (msgmnb);
	if (sysctlbyname ("kern.ipc.msgmnb", &msgmnb, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.msgmnb)");
		return;
	}

	len = sizeof (msgmni);
	if (sysctlbyname ("kern.ipc.msgmni", &msgmni, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.msgmni)");
		return;
	}

	len = sizeof (msgmax);
	if (sysctlbyname ("kern.ipc.msgmax", &msgmax, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.ipc.msgmax)");
		return;
	}

	buf->msgmax = msgmax;
	buf->msgmni = msgmni;
	buf->msgmnb = msgmnb;
	buf->msgtql = msgtql;
	buf->msgssz = msgssz;
	buf->msgpool = msgseg;

	buf->flags = _glibtop_sysdeps_msg_limits;
}
