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
#include <glibtop/procstate.h>
#include <glibtop/command.h>

#if GLIBTOP_SUID_PROC_STATE

/* Provides detailed information about a process. */

void
glibtop_get_proc_state__l (glibtop *server, glibtop_proc_state *buf,
			   pid_t pid)
{
	glibtop_init__r (&server);
	glibtop_call__l (server, GLIBTOP_CMND_PROC_STATE, sizeof (pid_t),
			 &pid, sizeof (glibtop_proc_state), buf);
}

#endif
