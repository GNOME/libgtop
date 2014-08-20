/*
   This file is part of LibGTop 2.0.

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
#include <unistd.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procsignal.h>

#include <glibtop_suid.h>

#include <sys/param.h>
#include <sys/sysctl.h>

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

/* Provides detailed information about a process. */

void
glibtop_get_proc_signal_p (glibtop *server, glibtop_proc_signal *buf,
			   pid_t pid)
{
	int mib[4];
	size_t length;
	struct kinfo_proc kinfo;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_SIGNAL), 0);
	
	memset (buf, 0, sizeof (glibtop_proc_signal));

	mib [0] = CTL_KERN;
	mib [1] = KERN_PROC;      
	mib [2] = KERN_PROC_PID;
	mib [3] = pid;

	length = sizeof (struct kinfo_proc);
	if (sysctl (mib, 4, &kinfo, &length, NULL, 0) < 0) {
		glibtop_warn_io_r (server, "sysctl (procuid)");
		return;
	}

	buf->signal[0]    = kinfo.kp_proc.p_siglist;
	buf->blocked[0]   = kinfo.kp_proc.p_sigmask;
	buf->sigignore[0] = kinfo.kp_proc.p_sigignore;
	buf->sigcatch[0]  = kinfo.kp_proc.p_sigcatch;
	buf->flags = _glibtop_sysdeps_proc_signal;
}
