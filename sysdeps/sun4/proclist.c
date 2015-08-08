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
#include <glibtop/proclist.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_proclist =
(1 << GLIBTOP_PROCLIST_TOTAL) + (1 << GLIBTOP_PROCLIST_NUMBER) +
(1 << GLIBTOP_PROCLIST_SIZE);

/* Fetch list of currently running processes.
 *
 * IMPORTANT NOTE:
 *   On error, this function MUST return NULL and set buf->flags to zero !
 *   On success, it returnes a pointer to a list of buf->number elements
 *   each buf->size big. The total size is stored in buf->total. */

unsigned *
glibtop_get_proclist_p (glibtop *server, glibtop_proclist *buf,
			gint64 which, gint64 arg)
{
	register struct proc *pp;
	register int i, nproc = 0;
	unsigned *proc_list = NULL;
	size_t proc_size;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROCLIST), 0);

	memset (buf, 0, sizeof (glibtop_proclist));

	/* Read process table from kernel. */

	_glibtop_read_proc_table (server);

	/* Count number of processes. */

	for (pp = server->machine->proc_table, i = 0;
	     i < server->machine->nproc; pp++, i++) {
		if (pp->p_stat == 0)
			continue;
		else
			nproc++;
	}

	if (nproc == 0) /* Should never happen. */
		return NULL;

	/* Allocate space for process list. */

	proc_size = nproc * sizeof (unsigned);

	proc_list = g_malloc (proc_size);

	/* Write process list. */

	for (pp = server->machine->proc_table, i = 0, nproc = 0;
	     i < server->machine->nproc; pp++, i++) {
		if (pp->p_stat == 0)
			continue;
		proc_list [nproc++] = pp->p_pid;
	}

	/* Since everything is ok now, we can set buf->flags, fill in the remaining fields
	   and return proc_list. */

	buf->flags = _glibtop_sysdeps_proclist;

	buf->size = sizeof (unsigned);
	buf->number = nproc;

	buf->total = nproc * sizeof (unsigned);

	return proc_list;
}
