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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/msg_limits.h>

static const unsigned long _glibtop_sysdeps_msg_limits =
(1 << GLIBTOP_IPC_MSGMAP) + (1 << GLIBTOP_IPC_MSGMAX) +
(1 << GLIBTOP_IPC_MSGMNB) + (1 << GLIBTOP_IPC_MSGMNI) + 
(1 << GLIBTOP_IPC_MSGTQL);

/* Provides information about sysv ipc limits. */

void
glibtop_get_msg_limits__s (glibtop *server, glibtop_msg_limits *buf)
{
	int ret, value;

	glibtop_init ();

	memset (buf, 0, sizeof (glibtop_msg_limits));
	
	ret = table (TBL_MSGINFO, MSGINFO_MAX, (char *) &value, 1,
		     sizeof (value)); 

	if (ret != 1) return;
	
	buf->flags += (1 << GLIBTOP_IPC_MSGMAX);

	buf->msgmax = value;
	
	ret = table (TBL_MSGINFO, MSGINFO_MNB, (char *) &value, 1,
		     sizeof (value)); 

	if (ret != 1) return;

	buf->flags += (1 << GLIBTOP_IPC_MSGMNB);

	buf->msgmnb = value;
	
	ret = table (TBL_MSGINFO, MSGINFO_MNI, (char *) &value, 1,
		     sizeof (value)); 

	if (ret != 1) return;

	buf->flags += (1 << GLIBTOP_IPC_MSGMNI);

	buf->msgmni = value;
	
	ret = table (TBL_MSGINFO, MSGINFO_TQL, (char *) &value, 1,
		     sizeof (value)); 

	if (ret != 1) return;

	buf->flags += (1 << GLIBTOP_IPC_MSGTQL);

	buf->msgtql = value;
}
