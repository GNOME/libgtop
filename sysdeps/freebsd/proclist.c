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
#include <glibtop/proclist.h>
#include <glibtop/procstate.h>

#include <glibtop_suid.h>

#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/user.h>

static const unsigned long _glibtop_sysdeps_proclist =
(1L << GLIBTOP_PROCLIST_TOTAL) + (1L << GLIBTOP_PROCLIST_NUMBER) +
(1L << GLIBTOP_PROCLIST_SIZE);

/* Fetch list of currently running processes.
 * The interface of this function is a little bit different from the others:
 * buf->flags is only set if the call succeeded, in this case pids_chain,
 * a list of the pids of all currently running processes is returned,
 * buf->number is the number of elements of this list and buf->size is
 * the size of one single element (sizeof (pid_t)). The total size is
 * stored in buf->total.
 *
 * The calling function has to free the memory to which a pointer is returned.
 *
 * IMPORTANT NOTE:
 *   On error, this function MUST return NULL and set buf->flags to zero !
 *   On success, it returnes a pointer to a list of buf->number elements
 *   each buf->size big. The total size is stored in buf->total.
 * The calling function has to free the memory to which a pointer is returned.
 *
 * On error, NULL is returned and buf->flags is zero. */

/* Init function. */

void
_glibtop_init_proclist_p (glibtop *server)
{
	server->sysdeps.proclist = _glibtop_sysdeps_proclist;
}

pid_t *
glibtop_get_proclist_p (glibtop *server, glibtop_proclist *buf,
			gint64 which, gint64 arg)
{
	struct kinfo_proc *pinfo;
	GArray *pids;
	glibtop_proc_state procstate;
	size_t len;
	unsigned int i;
	pid_t prev;

	memset (buf, 0, sizeof (glibtop_proclist));

	if (sysctlbyname ("kern.proc.all", NULL, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.proc.all)");
		return NULL;
	}

	pinfo = (struct kinfo_proc *) g_malloc0 (len);

	if (sysctlbyname ("kern.proc.all", pinfo, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.proc.all)");
		g_free (pinfo);
		return NULL;
	}

	len /= sizeof (struct kinfo_proc);

	pids = g_array_sized_new (FALSE, FALSE, sizeof (pid_t), len);

	for (i = 0; i < len; i++) {
		pid_t pid;

		pid = (pid_t) pinfo[i].ki_pid;

		/* If a process has many threads, kern.proc.all reports multiple
		   times the same pid. So don't look twice at the same pid.
		   FIXME?: not sure that kern.proc.all reports a partially sorted
		   list (all pid/threads grouped).
		*/
		if (i > 0 && pid == prev) {
			continue;
		}

		prev = pid;

		switch (which & GLIBTOP_KERN_PROC_MASK) {
			case GLIBTOP_KERN_PROC_ALL:
				break;
			case GLIBTOP_KERN_PROC_PID:
				if ((pid_t) arg != pid)
					continue;
				break;
			case GLIBTOP_KERN_PROC_UID:
				if ((uid_t) arg != pinfo[i].ki_ruid)
					continue;
				break;
			case GLIBTOP_KERN_PROC_PGRP:
				if ((pid_t) arg != pinfo[i].ki_pgid)
					continue;
				break;
			case GLIBTOP_KERN_PROC_SESSION:
				if ((pid_t) arg != pinfo[i].ki_sid)
					continue;
				break;
			case GLIBTOP_KERN_PROC_TTY:
				if ((dev_t) arg != pinfo[i].ki_tdev)
					continue;
				break;
			case GLIBTOP_KERN_PROC_RUID:
				if ((uid_t) arg != pinfo[i].ki_ruid)
					continue;
				break;
			}

		if (which & GLIBTOP_EXCLUDE_NOTTY)
			if (pinfo[i].ki_tdev == (dev_t) -1) continue;

		if (which & GLIBTOP_EXCLUDE_IDLE) {
			glibtop_get_proc_state_p (server, &procstate, pid);
			if (procstate.flags & (1L << GLIBTOP_PROC_STATE_STATE))
				if (procstate.state != GLIBTOP_PROCESS_RUNNING) continue;

			if (pinfo[i].ki_ppid == 0 && !strcmp(pinfo[i].ki_comm, "idle"))
				continue;
		}

		if (which & GLIBTOP_EXCLUDE_SYSTEM)
			if (pinfo[i].ki_ruid == (uid_t) 0) continue;

		g_array_append_val (pids, pid);
	}

	g_free (pinfo);

	buf->flags = _glibtop_sysdeps_proclist;
	buf->size = sizeof (pid_t);
	buf->number = pids->len;
	buf->total = buf->number * buf->size;

	return (pid_t *) g_array_free (pids, FALSE);
}
