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
#include <glibtop/error.h>
#include <glibtop/proclist.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_proclist =
(1L << GLIBTOP_PROCLIST_TOTAL) + (1L << GLIBTOP_PROCLIST_NUMBER) +
(1L << GLIBTOP_PROCLIST_SIZE);

/* Init function. */

void
_glibtop_init_proclist_p (glibtop *server)
{
	server->sysdeps.proclist = _glibtop_sysdeps_proclist;
}

/* How many elements are there per proctable entry? */

#define ELEMENTS_PER_ENTRY	8

/* We have a buffer for BLOCK_COUNT pids; when it's full, it is copied
 * to a newly realloc()ed area. */

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

/* !!! THIS FUNCTION RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

unsigned *
glibtop_get_proclist_p (glibtop *server, glibtop_proclist *buf,
			gint64 which, gint64 arg)
{
	unsigned count = 0, total = 0;
	unsigned pids [BLOCK_COUNT], *pids_chain = NULL;
	unsigned pids_size = 0, pids_offset = 0, new_size;
	struct tbl_procinfo procinfo [8];
	int entry, max_elements, k;

	glibtop_init_p (server, GLIBTOP_SYSDEPS_PROCLIST, 0);

	memset (buf, 0, sizeof (glibtop_proclist));

	for (entry = 0; entry < server->machine->proctable_entries;
	     entry += ELEMENTS_PER_ENTRY)
	{
		/* !!! THE FOLLOWING CODE RUNS SUID ROOT -
		 *     CHANGE WITH CAUTION !!! */

		glibtop_suid_enter (server);

		max_elements = table
			(TBL_PROCINFO, entry, (char *) &procinfo,
			 ELEMENTS_PER_ENTRY, sizeof (struct tbl_procinfo));

		glibtop_suid_leave (server);

		/* !!! END OF SUID ROOT PART !!! */

		for (k = 0; k < max_elements; k++)
		{
			/* Does this entry contain a real process? */

			if (procinfo [k].pi_status == 0)
				continue;

			/* Fine. Now we first try to store it in pids.
			 * If this buffer is full, we copy it to the
			 * pids_chain. */

			if (count >= BLOCK_COUNT) {

				/* The following call to g_realloc ()
				 * will be equivalent to g_malloc ()
				 * if `pids_chain' is NULL. We just calculate
				 * the new size and copy `pids' to the
				 * beginning of the newly allocated block. */

				new_size = pids_size + BLOCK_SIZE;

				pids_chain = g_realloc
					(server, pids_chain, new_size);

				memcpy (pids_chain + pids_offset,
					pids, BLOCK_SIZE);

				pids_size = new_size;

				pids_offset += BLOCK_COUNT;

				count = 0;
			}

			/* pids is now big enough to hold at least
			 * one single pid. */

			pids [count++] = procinfo [k].pi_pid;

			total++;
		}
	}

	/* count is only zero if an error occured
	 * (eg. the server is not suid root). */

	if (!count) return NULL;

	/* The following call to g_realloc () will be equivalent to
	 * g_malloc () if `pids_chain' is NULL. We just calculate the
	 * new size and copy `pids' to the beginning of the newly allocated
	 * block. */

	new_size = pids_size + count * sizeof (unsigned);

	pids_chain = g_realloc (server, pids_chain, new_size);

	memcpy (pids_chain + pids_offset, pids, count * sizeof (unsigned));

	pids_size = new_size;

	pids_offset += BLOCK_COUNT;

	/* Since everything is ok now, we can set buf->flags, fill in the
	 * remaining fields and return `pids_chain'. */

	buf->flags = _glibtop_sysdeps_proclist;

	buf->size = sizeof (unsigned);
	buf->number = total;

	buf->total = total * sizeof (unsigned);

	return pids_chain;
}
