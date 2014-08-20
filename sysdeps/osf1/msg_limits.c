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
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/msg_limits.h>

static const unsigned long _glibtop_sysdeps_msg_limits =
(1L << GLIBTOP_IPC_MSGMAP) + (1L << GLIBTOP_IPC_MSGMAX) +
(1L << GLIBTOP_IPC_MSGMNB) + (1L << GLIBTOP_IPC_MSGMNI) +
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
	int ret, value;

	glibtop_init_s (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_msg_limits));

	ret = table (TBL_MSGINFO, MSGINFO_MAX, (char *) &value, 1,
		     sizeof (value));

	if (ret != 1) return;

	buf->flags += (1L << GLIBTOP_IPC_MSGMAX);

	buf->msgmax = value;

	ret = table (TBL_MSGINFO, MSGINFO_MNB, (char *) &value, 1,
		     sizeof (value));

	if (ret != 1) return;

	buf->flags += (1L << GLIBTOP_IPC_MSGMNB);

	buf->msgmnb = value;

	ret = table (TBL_MSGINFO, MSGINFO_MNI, (char *) &value, 1,
		     sizeof (value));

	if (ret != 1) return;

	buf->flags += (1L << GLIBTOP_IPC_MSGMNI);

	buf->msgmni = value;

	ret = table (TBL_MSGINFO, MSGINFO_TQL, (char *) &value, 1,
		     sizeof (value));

	if (ret != 1) return;

	buf->flags += (1L << GLIBTOP_IPC_MSGTQL);

	buf->msgtql = value;
}
