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
#include <glibtop/procsignal.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proc_signal =
(1L << GLIBTOP_PROC_SIGNAL_SIGNAL) +
(1L << GLIBTOP_PROC_SIGNAL_BLOCKED) +
(1L << GLIBTOP_PROC_SIGNAL_SIGIGNORE) +
(1L << GLIBTOP_PROC_SIGNAL_SIGCATCH);

/* Init function. */

void
glibtop_init_proc_signal_s (glibtop *server)
{
    server->sysdeps.proc_signal = _glibtop_sysdeps_proc_signal;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_signal_s (glibtop *server, glibtop_proc_signal *buf,
			   pid_t pid)
{
    libgtop_proc_signal_t proc_signal;
    int i;

    memset (buf, 0, sizeof (glibtop_proc_signal));

    if (glibtop_get_proc_data_proc_signal_s (server, &proc_signal, pid))
	return;

    for (i = 0; i < 1; i++) {
	buf->signal [i] = proc_signal.signal [i];
	buf->blocked [i] = proc_signal.blocked [i];
	buf->sigignore [i] = proc_signal.ignore [i];
	buf->sigcatch [i] = proc_signal.catch [i];
    }

    buf->flags = _glibtop_sysdeps_proc_signal;
}
