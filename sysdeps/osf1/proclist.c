/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

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
#include <glibtop/xmalloc.h>
#include <glibtop/proclist.h>

static const unsigned long _glibtop_sysdeps_proclist =
(1 << GLIBTOP_PROCLIST_TOTAL) + (1 << GLIBTOP_PROCLIST_NUMBER) +
(1 << GLIBTOP_PROCLIST_SIZE);

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
glibtop_get_proclist_p (glibtop *server, glibtop_proclist *buf)
{
	unsigned count = 0, total = 0;
	unsigned pids [BLOCK_COUNT], *pids_chain = NULL;
	unsigned pids_size = 0, pids_offset = 0, new_size;
	struct tbl_procinfo procinfo [8];
	int entry, max_elements, k;

	glibtop_open_p (server, 0, 0);
	
	memset (buf, 0, sizeof (glibtop_proclist));
	
	for (entry = 0; entry < server->machine.proctable_entries;
	     entry += ELEMENTS_PER_ENTRY)
	{

		/* !!! THE FOLLOWING CODE RUNS SUID ROOT - CHANGE WITH CAUTION !!! */
	
		setreuid (server->machine.uid, server->machine.euid);
	
		max_elements = table (TBL_PROCINFO, entry, (char *) &procinfo,
				      ELEMENTS_PER_ENTRY, sizeof (struct tbl_procinfo));
		
		if (setreuid (server->machine.euid, server->machine.uid))
			_exit (1);
			
		/* !!! END OF SUID ROOT PART !!! */
		
		for (k = 0; k < max_elements; k++)
		{
			/* Does this entry contain a real process? */

			if (procinfo [k].pi_status == 0)
				continue;

			/* The following code is copied from the Linux implementation.
			 * It's safe since we are no longer root at this point here. */
				
			/* Fine. Now we first try to store it in pids. If this buffer is
			 * full, we copy it to the pids_chain. */

			if (count >= BLOCK_COUNT) {
				/* The following call to glibtop_realloc will be equivalent to
				 * glibtop_malloc if pids_chain is NULL. We just calculate the
				 * new size and copy pids to the beginning of the newly allocated
				 * block. */

				new_size = pids_size + BLOCK_SIZE;

				pids_chain = glibtop_realloc_r (server, pids_chain, new_size);

				memcpy (pids_chain + pids_offset, pids, BLOCK_SIZE);

				pids_size = new_size;

				pids_offset += BLOCK_COUNT;

				count = 0;
			}

			/* pids is now big enough to hold at least one single pid. */
		
			pids [count++] = procinfo [k].pi_pid;

			total++;
		}	
	}

	/* count is only zero if an error occured (eg. the server is not suid root). */

	if (!count) return NULL;

	/* The following call to glibtop_realloc will be equivalent to
	 * glibtop_malloc if pids_chain is NULL. We just calculate the
	 * new size and copy pids to the beginning of the newly allocated
	 * block. */
	
	new_size = pids_size + count * sizeof (unsigned);
	
	pids_chain = glibtop_realloc_r (server, pids_chain, new_size);
	
	memcpy (pids_chain + pids_offset, pids, count * sizeof (unsigned));
	
	pids_size = new_size;
	
	pids_offset += BLOCK_COUNT;

	/* Since everything is ok now, we can set buf->flags, fill in the remaining fields
	   and return pids_chain. */

	buf->flags = _glibtop_sysdeps_proclist;

	buf->size = sizeof (unsigned);
	buf->number = total;

	buf->total = total * sizeof (unsigned);

	return pids_chain;
}
