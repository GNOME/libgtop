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
#include <procinfo.h>

#include <glibtop.h>
#include <glibtop/proclist.h>

static const unsigned long _glibtop_sysdeps_proclist =
(1 << GLIBTOP_PROCLIST_NUMBER) + (1 << GLIBTOP_PROCLIST_TOTAL) +
(1 << GLIBTOP_PROCLIST_SIZE);

#define BLOCK_COUNT	256
#define BLOCK_SIZE	(BLOCK_COUNT * sizeof (unsigned int))

/* Init function. */

void
_glibtop_init_proclist_s (glibtop *server)
{
	server->sysdeps.proclist = _glibtop_sysdeps_proclist;
}

/* Fetch list of currently running processes.
 *
 * IMPORTANT NOTE:
 *   On error, this function MUST return NULL and set buf->flags to zero !
 *   On success, it returnes a pointer to a list of buf->number elements
 *   each buf->size big. The total size is stored in buf->total. */

unsigned *
glibtop_get_proclist_s (glibtop *server, glibtop_proclist *buf,
			gint64 which, gint64 arg)
{
	struct procsinfo pinfo;
	int count, total;
	unsigned pids [BLOCK_COUNT], *pids_chain = NULL;
	int pids_size = 0, pids_offset = 0, new_size;
	pid_t current;
	int result;

	glibtop_init_s (&server, (1L << GLIBTOP_SYSDEPS_PROCLIST), 0);

	memset (buf, 0, sizeof (glibtop_proclist));

	for( count = total = 0, current = 0
	   , result = getprocs(&pinfo, sizeof(pinfo), NULL, 0, &current, 1);
	     result == 1;
	     result = getprocs(&pinfo, sizeof(pinfo), NULL, 0, &current, 1))
	{
		if (which & GLIBTOP_EXCLUDE_IDLE)
		{
			if (pinfo.pi_state & SIDL)
			{
				/* exclude idle processes */

				continue;
			}
		}

		if (which & GLIBTOP_EXCLUDE_SYSTEM)
		{
			if (pinfo.pi_flags & SKPROC)
			{
				/* exclude Kernel processes */

				continue;
			}
		}

		if (which & GLIBTOP_EXCLUDE_NOTTY)
		{
			if (!pinfo.pi_ttyp)
			{
				/* exclude processes without tty */

				continue;
			}
		}

		switch(which & GLIBTOP_KERN_PROC_MASK)
		{
			case GLIBTOP_KERN_PROC_ALL:

				/* return information about all processes
				 * so, let's go ahead
				 */

				break;

			case GLIBTOP_KERN_PROC_PID:

				/* return information about all processes with
				 * pid passed in arg
				 */

				if (pinfo.pi_pid != arg)
				{
					continue;
				}

				break;

			case GLIBTOP_KERN_PROC_PGRP:

				/* return information about all processes in
				 * process group passed in arg
				 */

				if (pinfo.pi_pgrp != arg)
				{
					continue;
				}

				break;

			case GLIBTOP_KERN_PROC_SESSION:

				/* return information about all processes in
				 * session passed in arg
				 */

				if (pinfo.pi_sid != arg)
				{
					continue;
				}

				break;

			case GLIBTOP_KERN_PROC_TTY:

				/* return information about all processes with
				 * tty device number passed in arg
				 */

				if (pinfo.pi_ttyd != arg)
				{
					continue;
				}

				break;

			case GLIBTOP_KERN_PROC_UID:

				/* return information about all processes with
				 * effective uid passed in arg
				 */

				if (pinfo.pi_cred.cr_uid != arg)
				{
					continue;
				}

				break;

			case GLIBTOP_KERN_PROC_RUID:

				/* return information about all processes with
				 * real uid passed in arg
				 */

				if (pinfo.pi_cred.cr_ruid != arg)
				{
					continue;
				}

				break;
		}

		if (count >= BLOCK_COUNT)
		{
			/* The following call to g_realloc will be
			 * equivalent to g_malloc () if `pids_chain' is
			 * NULL. We just calculate the new size and copy `pids'
			 * to the beginning of the newly allocated block. */

			new_size = pids_size + BLOCK_SIZE;

			pids_chain = g_realloc
					(pids_chain, new_size);
			memcpy (pids_chain + pids_offset, pids, BLOCK_SIZE);

			pids_size = new_size;
			pids_offset += BLOCK_COUNT;
			count = 0;
		}

		/* pids is now big enough to hold at least one single pid. */

		pids[count++] = pinfo.pi_pid;

		total++;
	}

	if (result == -1)
	{
		glibtop_error_io_r(server, "Cannot read procsinfo");
	}

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

	/* Since everything is ok now, we can set buf->flags, fill in the
	 * remaining fields and return the `pids_chain'. */

	buf->size = sizeof(unsigned int);
	buf->number = total;
	buf->total = total * buf->size;

	buf->flags = _glibtop_sysdeps_proclist;

	return pids_chain;
}
