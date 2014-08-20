/* Copyright (C)) +998-99 Martin Baulig
   This file is part of LibGTop) +.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April) +998.

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version) + of the License,
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
#include <unistd.h>
#include <sys/vminfo.h>
#include <sys/resource.h>
#include <sys/signal.h>

#include <glibtop.h>
#include <glibtop/procsignal.h>
#include <glibtop/error.h>

#include "utils.h"

static const unsigned long _glibtop_sysdeps_proc_signal =
(1 <<GLIBTOP_PROC_SIGNAL_SIGNAL) + (1 <<GLIBTOP_PROC_SIGNAL_BLOCKED) +
(1 <<GLIBTOP_PROC_SIGNAL_SIGIGNORE) + (1 <<GLIBTOP_PROC_SIGNAL_SIGCATCH);

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
	struct procsinfo *pinfo;
	int i;
	long bit;

	glibtop_init_s (&server, (1L << GLIBTOP_SYSDEPS_PROC_SIGNAL), 0);

	memset (buf, 0, sizeof (glibtop_proc_signal));

	pinfo = _glibtop_get_procinfo(server, pid);
	if (pinfo == NULL) return;

	/* pending signals */

#define NB_BITS	(8 * sizeof(guint64))

	for (i = 0; (i < NSIG) && (i < 2 * NB_BITS); i++)
	{
		bit = 1 << (NB_BITS - 1 - (i % NB_BITS));

		if (sigismember(&pinfo->pi_sig, i))
			buf->signal[i / NB_BITS] |= bit;

		if (pinfo->pi_signal[i] == (long)SIG_HOLD)
			buf->blocked[i / NB_BITS] |= bit;

		if (pinfo->pi_signal[i] == (long)SIG_IGN)
			buf->sigignore[i / NB_BITS] |= bit;

		if (pinfo->pi_signal[i] == (long)SIG_CATCH)
			buf->sigcatch[i / NB_BITS] |= bit;
	}

	buf->flags = _glibtop_sysdeps_proc_signal;
}
