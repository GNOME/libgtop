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

#include <config.h>
#include <glibtop/xmalloc.h>
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
glibtop_init_proclist_s (glibtop *server)
{
	server->sysdeps.proclist = _glibtop_sysdeps_proclist;
}

#define BLOCK_COUNT	256
#define BLOCK_SIZE	(BLOCK_COUNT * sizeof (unsigned))

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

unsigned *
glibtop_get_proclist_s (glibtop *server, glibtop_proclist *buf,
			int64_t which, int64_t arg)
{
	DIR *proc;
	struct dirent *entry;
	char buffer [BUFSIZ];
	unsigned count, total, pid;
	unsigned pids [BLOCK_COUNT], *pids_chain = NULL;
	unsigned pids_size = 0, pids_offset = 0, new_size;
	struct stat statb;
	int len, i, ok;

	glibtop_proc_uid procuid;
	glibtop_proc_state procstate;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROCLIST, 0);

	memset (buf, 0, sizeof (glibtop_proclist));

	proc = opendir ("/proc");
	if (!proc) return NULL;

	/* read every every entry in /proc */

	for (count = total = 0, entry = readdir (proc);
	     entry; entry = readdir (proc)) {
		ok = 1; len = strlen (entry->d_name);

		/* does it consist entirely of digits? */
		
		for (i = 0; i < len; i++)
			if (!isdigit (entry->d_name [i])) ok = 0;
		if (!ok) continue;

		/* convert it in a number */

		if (sscanf (entry->d_name, "%u", &pid) != 1) continue;

		/* is it really a directory? */

		sprintf (buffer, "/proc/%d", pid);
		
		if (stat (buffer, &statb)) continue;

		if (!S_ISDIR (statb.st_mode)) continue;

		switch (which & GLIBTOP_KERN_PROC_MASK) {
		case GLIBTOP_KERN_PROC_ALL:
			break;
		case GLIBTOP_KERN_PROC_PID:
			if ((unsigned) arg != pid)
				continue;
			break;
		case GLIBTOP_KERN_PROC_UID:
			if ((uid_t) arg != statb.st_uid)
				continue;
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
				if (procstate.state != 'R') continue;
		}

		if (which & GLIBTOP_EXCLUDE_SYSTEM) {
			glibtop_get_proc_uid_s (server, &procuid, pid);
			if (procuid.flags & (1L << GLIBTOP_PROC_UID_UID))
				if (procuid.uid == 0) continue;
		}

		/* Fine. Now we first try to store it in pids. If this buffer is
		 * full, we copy it to the pids_chain. */

		if (count >= BLOCK_COUNT) {
			/* The following call to g_realloc will be
			 * equivalent to g_malloc () if `pids_chain' is
			 * NULL. We just calculate the new size and copy `pids'
			 * to the beginning of the newly allocated block. */

			new_size = pids_size + BLOCK_SIZE;

			pids_chain = g_realloc (pids_chain, new_size);

			memcpy (pids_chain + pids_offset, pids, BLOCK_SIZE);

			pids_size = new_size;

			pids_offset += BLOCK_COUNT;

			count = 0;
		}

		/* pids is now big enough to hold at least one single pid. */
		
		pids [count++] = pid;

		total++;
	}
	
	closedir (proc);

	/* count is only zero if an error occured (one a running Linux system,
	 * we have at least one single process). */

	if (!count) return NULL;

	/* The following call to g_realloc will be equivalent to
	 * g_malloc if pids_chain is NULL. We just calculate the
	 * new size and copy pids to the beginning of the newly allocated
	 * block. */
	
	new_size = pids_size + count * sizeof (unsigned);
	
	pids_chain = g_realloc (pids_chain, new_size);
	
	memcpy (pids_chain + pids_offset, pids, count * sizeof (unsigned));
	
	pids_size = new_size;
	
	pids_offset += BLOCK_COUNT;

	/* Since everything is ok now, we can set buf->flags, fill in the
	 * remaining fields and return the `pids_chain'. */

	buf->flags = _glibtop_sysdeps_proclist;

	buf->size = sizeof (unsigned);
	buf->number = total;

	buf->total = total * sizeof (unsigned);

	return pids_chain;
}
