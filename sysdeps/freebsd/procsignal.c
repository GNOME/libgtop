/* Copyright (C) 1998 Joshua Sled
   This file is part of LibGTop 1.0.

   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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
#include <glibtop/procsignal.h>

#include <glibtop_suid.h>

#include <sys/param.h>

#include <osreldate.h>

static const unsigned long _glibtop_sysdeps_proc_signal =
(1L << GLIBTOP_PROC_SIGNAL_SIGNAL) +
(1L << GLIBTOP_PROC_SIGNAL_BLOCKED) +
(1L << GLIBTOP_PROC_SIGNAL_SIGIGNORE) +
(1L << GLIBTOP_PROC_SIGNAL_SIGCATCH);

/* Init function. */

void
_glibtop_init_proc_signal_p (glibtop *server)
{
	server->sysdeps.proc_signal = _glibtop_sysdeps_proc_signal;
}

void
glibtop_get_proc_signal_p (glibtop *server,
			   glibtop_proc_signal *buf,
			   pid_t pid)
{
	struct kinfo_proc *pinfo;
	int count = 0;

	memset (buf, 0, sizeof (glibtop_proc_signal));

	/* It does not work for the swapper task. */
	if (pid == 0) return;

	/* Get the process information */
	pinfo = kvm_getprocs (server->machine->kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1)) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		return;
	}

#define	PROC_SIGLIST	ki_siglist
#define	PROC_SIGMASK	ki_sigmask
#define	PROC_SIGIGNORE	ki_sigignore
#define	PROC_SIGCATCH	ki_sigcatch

	/* signal: mask of pending signals.
	 *         pinfo [0].kp_proc.p_siglist
	 */
	buf->signal [0] = pinfo [0].PROC_SIGLIST.__bits[0];

	/* blocked: mask of blocked signals.
	 *          pinfo [0].kp_proc.p_sigmask
	 */
	buf->blocked [0] = pinfo [0].PROC_SIGMASK.__bits[0];

	/* sigignore: mask of ignored signals.
	 *            pinfo [0].kp_proc.p_sigignore
	*/
	buf->sigignore [0] = pinfo [0].PROC_SIGIGNORE.__bits[0];

	/* sigcatch: mask of caught signals.
	 *           pinfo [0].kp_proc.p_sigcatch
	*/
	buf->sigcatch [0] = pinfo [0].PROC_SIGCATCH.__bits[0];

	buf->flags = _glibtop_sysdeps_proc_signal;
}
