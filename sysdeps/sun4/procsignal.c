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
#include <glibtop/procsignal.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_proc_signal =
(1 << GLIBTOP_PROC_SIGNAL_SIGNAL) + (1 << GLIBTOP_PROC_SIGNAL_BLOCKED) +
(1 << GLIBTOP_PROC_SIGNAL_SIGIGNORE) + (1 << GLIBTOP_PROC_SIGNAL_SIGCATCH);

/* Provides detailed information about a process. */

void
glibtop_get_proc_signal_p (glibtop *server, glibtop_proc_signal *buf,
			   pid_t pid)
{
	struct proc *pp;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_SIGNAL), 0);

	memset (buf, 0, sizeof (glibtop_proc_signal));

	/* Read process table from kernel. */	

	_glibtop_read_proc_table (server);

	/* Find the pid in the process table. */

	pp = _glibtop_find_pid (server, pid);

	if (pp == NULL)	return;

	/* Fill in data fields. */

	buf->signal = pp->p_sig;
	buf->blocked = pp->p_sigmask;
	buf->sigignore = pp->p_sigignore;
	buf->sigcatch = pp->p_sigcatch;
	
	buf->flags = _glibtop_sysdeps_proc_signal;
}
