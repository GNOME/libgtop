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

#include <glibtop.h>
#include <glibtop/msg_limits.h>

#include <kvm.h>
#include <sys/msg.h>

static struct nlist nlst[] = { {"msginfo"}, {NULL} };
static const unsigned long _glibtop_sysdeps_msg_limits =
(1L << GLIBTOP_IPC_MSGPOOL) + (1L << GLIBTOP_IPC_MSGMAP) +
(1L << GLIBTOP_IPC_MSGMAX) + (1L << GLIBTOP_IPC_MSGMNB) +
(1L << GLIBTOP_IPC_MSGMNI) + (1L << GLIBTOP_IPC_MSGSSZ) +
(1L << GLIBTOP_IPC_MSGTQL);

/* Init function. */

void
glibtop_init_msg_limits_p (glibtop *server)
{
   	kvm_t *kd = server->machine.kd;

	if(kd && !kvm_nlist(kd, nlst))
		server->sysdeps.msg_limits = _glibtop_sysdeps_msg_limits;
	else
	   	server->sysdeps.msg_limits = 0;
}

/* Provides information about sysv ipc limits. */

void
glibtop_get_msg_limits_p (glibtop *server, glibtop_msg_limits *buf)
{
   	kvm_t *kd = server->machine.kd;
	struct msginfo minfo;

	memset (buf, 0, sizeof (glibtop_msg_limits));

	if(!(server->sysdeps.msg_limits))
	   	return;
	if(kvm_read(kd, nlst[0].n_value, (void *)&minfo,
		    sizeof(struct msginfo)) != sizeof(struct msginfo))
	   	return;

	buf->msgmap = minfo.msgmap;
	buf->msgmax = minfo.msgmax;
	buf->msgmnb = minfo.msgmnb;
	buf->msgmni = minfo.msgmni;
	buf->msgssz = minfo.msgssz;
	buf->msgtql = minfo.msgtql;
	buf->msgpool = minfo.msgmni * minfo.msgmnb >> 10;
	buf->flags = _glibtop_sysdeps_msg_limits;
}
