/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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
#include <glibtop/procsignal.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_proc_signal =
(1 << GLIBTOP_PROC_SIGNAL_SIGNAL) +
(1 << GLIBTOP_PROC_SIGNAL_BLOCKED) +
(1 << GLIBTOP_PROC_SIGNAL_SIGIGNORE) +
(1 << GLIBTOP_PROC_SIGNAL_SIGCATCH);

/* Init function. */

void
glibtop_init_proc_signal_p (glibtop *server)
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

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_SIGNAL), 0);
	
	memset (buf, 0, sizeof (glibtop_proc_signal));

	/* It does not work for the swapper task. */
	if (pid == 0) return;
	
	/* Get the process information */
	pinfo = kvm_getprocs (server->machine.kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1)) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		return;
	}

	/* signal: mask of pending signals.
	 *         pinfo [0].kp_proc.p_siglist
	 */
	buf->signal [0] = pinfo [0].kp_proc.p_siglist;

	/* blocked: mask of blocked signals.
	 *          pinfo [0].kp_proc.p_sigmask
	 */
	buf->blocked [0] = pinfo [0].kp_proc.p_sigmask;
	
	/* sigignore: mask of ignored signals.
	 *            pinfo [0].kp_proc.p_sigignore
	*/
	buf->sigignore [0] = pinfo [0].kp_proc.p_sigignore;
	
	/* sigcatch: mask of caught signals.
	 *           pinfo [0].kp_proc.p_sigcatch
	*/
	buf->sigcatch [0] = pinfo [0].kp_proc.p_sigcatch;

	buf->flags = _glibtop_sysdeps_proc_signal;
}
