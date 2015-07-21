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
#include <glibtop/procsegment.h>

#include <glibtop_suid.h>

#include <kvm.h>
#include <sys/param.h>
#include <sys/sysctl.h>

static const unsigned long _glibtop_sysdeps_proc_segment =
(1L << GLIBTOP_PROC_SEGMENT_TEXT_RSS) +
(1L << GLIBTOP_PROC_SEGMENT_DATA_RSS);

static int pagesize;

/* Init function. */

void
_glibtop_init_proc_segment_p (glibtop *server)
{
	pagesize = getpagesize ();
	server->sysdeps.proc_segment = _glibtop_sysdeps_proc_segment;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_segment_p (glibtop *server,
			    glibtop_proc_segment *buf,
			    pid_t pid)
{
	struct kinfo_proc *pinfo;
	int count = 0;

	memset (buf, 0, sizeof (glibtop_proc_segment));

	/* It does not work for the swapper task. */
	if (pid == 0) return;

	/* Get the process info from the kernel */
	pinfo = kvm_getprocs (server->machine->kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1)) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		return;
	}

	buf->text_rss = pinfo[0].ki_tsize * pagesize;
	buf->data_rss = pinfo[0].ki_dsize * pagesize;

	buf->flags = _glibtop_sysdeps_proc_segment;
}
