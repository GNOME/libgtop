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
#include <glibtop/proclist.h>

#include <glibtop/procuid.h>
#include <glibtop/procstate.h>

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>

static const unsigned long _glibtop_sysdeps_proclist =
(1L << GLIBTOP_PROCLIST_TOTAL) + (1L << GLIBTOP_PROCLIST_NUMBER) +
(1L << GLIBTOP_PROCLIST_SIZE);

/* Init function. */

void
_glibtop_init_proclist_s (glibtop *server)
{
	server->sysdeps.proclist = _glibtop_sysdeps_proclist;
}

/* Fetch list of currently running processes.
 *
 * The interface of this function is a little bit different from the others:
 * buf->flags is only set if the call succeeded, in this case pids_chain,
 * a list of the pids of all currently running processes is returned,
 * buf->number is the number of elements of this list and buf->size is
 * the size of one single element (sizeof (unsigned)). The total size is
 * stored in buf->total.
 *
 * The calling function has to free the memory to which a pointer is returned.
 *
 * On error, NULL is returned and buf->flags is zero. */

pid_t*
glibtop_get_proclist_s (glibtop *server, glibtop_proclist *buf,
			gint64 which, gint64 arg)
{
	DIR *proc;
	struct dirent *entry;
	GArray *pids;
	glibtop_proc_uid procuid;
	glibtop_proc_state procstate;
	struct stat statb;

	memset (buf, 0, sizeof (glibtop_proclist));

	proc = opendir ("/proc");
	if (!proc) return NULL;

	if(fstat(dirfd(proc), &statb) != 0) return NULL;

	pids = g_array_sized_new(FALSE, FALSE, sizeof(pid_t), statb.st_nlink);

	/* read every every entry in /proc */

	while((entry = readdir (proc))) {
		pid_t pid;

		if (entry->d_type != DT_DIR)
			continue;

		if (!(pid = strtoul(entry->d_name, NULL, 10)))
			continue;

		switch (which & GLIBTOP_KERN_PROC_MASK) {
		case GLIBTOP_KERN_PROC_ALL:
			break;
		case GLIBTOP_KERN_PROC_PID:
			if ((pid_t) arg != pid)
				continue;
			break;
		case GLIBTOP_KERN_PROC_UID:
		{
			char path[32];
			struct stat path_stat;

			snprintf(path, sizeof path, "/proc/%u", (unsigned)pid);

			if (stat(path, &path_stat))
				continue;

			if ((uid_t) arg != path_stat.st_uid)
				continue;
		}
			break;
		case GLIBTOP_KERN_PROC_PGRP:
			/* Do you really, really need this ? */
			glibtop_get_proc_uid_s (server, &procuid, pid);
			if (procuid.flags & (1L << GLIBTOP_PROC_UID_PGRP))
				if ((int) arg != procuid.pgrp)
					continue;
			break;
		case GLIBTOP_KERN_PROC_SESSION:
			/* Do you really, really need this ? */
			glibtop_get_proc_uid_s (server, &procuid, pid);
			if (procuid.flags & (1L << GLIBTOP_PROC_UID_SESSION))
				if ((int) arg != procuid.session)
					continue;
			break;
		case GLIBTOP_KERN_PROC_TTY:
			/* Do you really, really need this ? */
			glibtop_get_proc_uid_s (server, &procuid, pid);
			if (procuid.flags & (1L << GLIBTOP_PROC_UID_TTY))
				if ((int) arg != procuid.tty)
					continue;
			break;
		case GLIBTOP_KERN_PROC_RUID:
			/* Do you really, really need this ? */
			glibtop_get_proc_uid_s (server, &procuid, pid);
			if (procuid.flags & (1L << GLIBTOP_PROC_UID_EUID))
				if ((int) arg != procuid.euid)
					continue;
			break;
		}

		if (which & GLIBTOP_EXCLUDE_NOTTY) {
			glibtop_get_proc_uid_s (server, &procuid, pid);
			if (procuid.flags & (1L << GLIBTOP_PROC_UID_TTY))
				if (procuid.tty == -1) continue;
		}

		if (which & GLIBTOP_EXCLUDE_IDLE) {
			glibtop_get_proc_state_s (server, &procstate, pid);
			if (procstate.flags & (1L << GLIBTOP_PROC_STATE_STATE))
				if (procstate.state != GLIBTOP_PROCESS_RUNNING) continue;
		}

		if (which & GLIBTOP_EXCLUDE_SYSTEM) {
			glibtop_get_proc_uid_s (server, &procuid, pid);
			if (procuid.flags & (1L << GLIBTOP_PROC_UID_UID))
				if (procuid.uid == 0) continue;
		}

		g_array_append_val(pids, pid);
	}

	closedir (proc);

	buf->flags = _glibtop_sysdeps_proclist;
	buf->size = sizeof(pid_t);
	buf->number = pids->len;
	buf->total = buf->number * buf->size;

	return (pid_t*)g_array_free(pids, FALSE);
}
