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
#include <glibtop/procsignal.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proc_signal =
(1L << GLIBTOP_PROC_SIGNAL_SIGNAL) + (1L << GLIBTOP_PROC_SIGNAL_BLOCKED);

/* Init function. */

void
_glibtop_init_proc_signal_s (glibtop *server)
{
	server->sysdeps.proc_signal = _glibtop_sysdeps_proc_signal;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_signal_s (glibtop *server, glibtop_proc_signal *buf,
			   pid_t pid)
{
#ifdef HAVE_PROCFS_H
   	struct pstatus pstatus;
#else
	struct prstatus pstatus;
#endif
	int size;

	memset (buf, 0, sizeof (glibtop_proc_signal));

	if(glibtop_get_proc_status_s(server, &pstatus, pid))
	   	return;

	if(sizeof(buf->signal) < sizeof(sigset_t))
	   	size = sizeof(buf->signal);
	else
	   	size = sizeof(sigset_t);

	memcpy(buf->signal, &pstatus.pr_sigpend, size);
#ifdef HAVE_PROCFS_H
	memcpy(buf->blocked, &pstatus.pr_lwp.pr_lwphold, size);
#else
	memcpy(buf->blocked, &pstatus.pr_lwppend, size);
#endif

	/* Technically, most of this is meaningless on a process level,
	   but this should be a good enough approximation. */

	buf->flags = _glibtop_sysdeps_proc_signal;
}
