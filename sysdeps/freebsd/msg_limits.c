/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, August 1998.

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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/msg_limits.h>

#include <glibtop_suid.h>

/* #define KERNEL to get declaration of `struct msginfo'. */

#define KERNEL

#include <sys/ipc.h>
#include <sys/msg.h>

static const unsigned long _glibtop_sysdeps_msg_limits =
(1 << GLIBTOP_IPC_MSGMAX) + (1 << GLIBTOP_IPC_MSGMNI) +
(1 << GLIBTOP_IPC_MSGMNB) + (1 << GLIBTOP_IPC_MSGTQL) +
(1 << GLIBTOP_IPC_MSGSSZ);

/* The values in this structure never change at runtime, so we only
 * read it once during initialization. We have to use the name `_msginfo'
 * since `msginfo' is already declared external in <sys/msg.h>. */
static struct msginfo _msginfo;
  
/* nlist structure for kernel access */
static struct nlist nlst [] = {
	{ "_msginfo" },
	{ 0 }
};

/* Init function. */

void
glibtop_init_msg_limits_p (glibtop *server)
{
	if (kvm_nlist (server->machine.kd, nlst) != 0) {
		glibtop_warn_io_r (server, "kvm_nlist (msg_limits)");
		return;
	}
	
	if (kvm_read (server->machine.kd, nlst [0].n_value,
		      &_msginfo, sizeof (_msginfo)) != sizeof (_msginfo)) {
		glibtop_warn_io_r (server, "kvm_read (msginfo)");
		return;
	}

	server->sysdeps.msg_limits = _glibtop_sysdeps_msg_limits;
}

/* Provides information about sysv ipc limits. */

void
glibtop_get_msg_limits_p (glibtop *server, glibtop_msg_limits *buf)
{
	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_MSG_LIMITS), 0);
	
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
