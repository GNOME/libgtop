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

#include <glibtop/msg_limits.h>

#include <sys/ipc.h>
#include <sys/msg.h>

static const unsigned long _glibtop_sysdeps_msg_limits =
(1 << GLIBTOP_IPC_MSGPOOL) + (1 << GLIBTOP_IPC_MSGMAP) +
(1 << GLIBTOP_IPC_MSGMAX) + (1 << GLIBTOP_IPC_MSGMNB) +
(1 << GLIBTOP_IPC_MSGMNI) + (1 << GLIBTOP_IPC_MSGSSZ) +
(1 << GLIBTOP_IPC_MSGTQL);

/* Provides information about sysv ipc limits. */

void
glibtop_get_msg_limits__s (glibtop *server, glibtop_msg_limits *buf)
{
	struct msginfo	msginfo;
  
	glibtop_init__r (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_msg_limits));
  
	buf->flags = _glibtop_sysdeps_msg_limits;
  
	msgctl (0, IPC_INFO, (struct msqid_ds *) &msginfo);
  
	buf->msgpool = msginfo.msgpool;
	buf->msgmap = msginfo.msgmap;
	buf->msgmax = msginfo.msgmax;
	buf->msgmnb = msginfo.msgmnb;
	buf->msgmni = msginfo.msgmni;
	buf->msgssz = msginfo.msgssz;
	buf->msgtql = msginfo.msgtql;
}
