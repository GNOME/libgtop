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
#include <glibtop/proclist.h>

#include "kernel.h"

static const unsigned long _glibtop_sysdeps_proclist =
(1 << GLIBTOP_PROCLIST_TOTAL) + (1 << GLIBTOP_PROCLIST_NUMBER) +
(1 << GLIBTOP_PROCLIST_SIZE);

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
	union table tbl;
	unsigned *pids_chain;
	struct proclist_args args;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROCLIST, 0);

	memset (buf, 0, sizeof (glibtop_proclist));

	args.which = which;
	args.arg = arg;

	if (table (TABLE_PROCLIST, &tbl, &args))
		glibtop_error_io_r (server, "table(TABLE_PROCLIST)");

	buf->size = sizeof (unsigned);
	buf->number = tbl.proclist.nr_tasks;

	buf->total = buf->number * sizeof (unsigned);

	pids_chain = g_malloc (server, buf->total);

	memcpy (pids_chain, tbl.proclist.pids, buf->total);

	return pids_chain;
}
