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

#include <glibtop_suid.h>

/* Define the appropriate macro (if any) to get declaration of `struct
 * msginfo'.  Needed on, at least, FreeBSD. */
#if defined (STRUCT_MSGINFO_NEEDS_KERNEL)
#define KERNEL 1
#elif defined (STRUCT_MSGINFO_NEEDS__KERNEL)
#define _KERNEL 1
#endif

#include <sys/ipc.h>
#include <sys/msg.h>

static const unsigned long _glibtop_sysdeps_msg_limits =
(1L << GLIBTOP_IPC_MSGMAX) + (1L << GLIBTOP_IPC_MSGMNI) +
(1L << GLIBTOP_IPC_MSGMNB) + (1L << GLIBTOP_IPC_MSGTQL) +
(1L << GLIBTOP_IPC_MSGSSZ);

/* The values in this structure never change at runtime, so we only
 * read it once during initialization. We have to use the name `_msginfo'
 * since `msginfo' is already declared external in <sys/msg.h>. */
static struct msginfo _msginfo;

/* Init function. */

void
_glibtop_init_msg_limits_p (glibtop *server)
{
	int mib[3];
	struct msg_sysctl_info *msgsi;

	size_t len = sizeof(struct msginfo);

	mib[0] = CTL_KERN;
	mib[1] = KERN_SYSVIPC_INFO;
	mib[2] = KERN_SYSVIPC_MSG_INFO;

	if ((msgsi = malloc(len)) == NULL) {
		glibtop_warn_io_r (server, "malloc (msg_limits)");
		return;
	}

	if (sysctl(mib, 3, msgsi, &len, NULL, 0) < 0) {
		glibtop_warn_io_r (server, "sysctl (msg_limits)");
		return;
	}

	_msginfo = msgsi->msginfo;

	free (msgsi);

	server->sysdeps.msg_limits = _glibtop_sysdeps_msg_limits;
}

/* Provides information about sysv ipc limits. */

void
glibtop_get_msg_limits_p (glibtop *server, glibtop_msg_limits *buf)
{
	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_MSG_LIMITS), 0);

	memset (buf, 0, sizeof (glibtop_msg_limits));

	if (server->sysdeps.msg_limits == 0)
		return;

	buf->msgmax = _msginfo.msgmax;
	buf->msgmni = _msginfo.msgmni;
	buf->msgmnb = _msginfo.msgmnb;
	buf->msgtql = _msginfo.msgtql;
	buf->msgssz = _msginfo.msgtql;

	buf->flags = _glibtop_sysdeps_msg_limits;
}
