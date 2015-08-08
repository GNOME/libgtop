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
#include <glibtop/mem.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_mem =
(1 << GLIBTOP_MEM_TOTAL) + (1 << GLIBTOP_MEM_USED) +
(1 << GLIBTOP_MEM_FREE) + (1 << GLIBTOP_MEM_LOCKED);

/* define pagetok in terms of pageshift */

#define pagetok(size) ((size) << server->machine->pageshift)

/* Provides information about memory usage. */

void
glibtop_get_mem_p (glibtop *server, glibtop_mem *buf)
{
	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_MEM), 0);

	memset (buf, 0, sizeof (glibtop_mem));

	/* !!! THE FOLLOWING CODE RUNS SGID KMEM - CHANGE WITH CAUTION !!! */

	glibtop_suid_enter (server);

	/* get the array of physpage descriptors */

	(void) _glibtop_getkval (server, server->machine->pages,
				 (int *) server->machine->physpage,
				 server->machine->bytesize,
				 "array _page");

	glibtop_suid_leave (server);

	/* !!! END OF SGID KMEM PART !!! */


	{	/* sum memory statistics */
		register struct page *pp;
		register int cnt;
		register int inuse;
		register int free;
		register int locked;

		/* bop thru the array counting page types */

		pp = server->machine->physpage;
		inuse = free = locked = 0;
		for (cnt = server->machine->count; --cnt >= 0; pp++) {
			if (pp->p_free)
				free++;
			else if (pp->p_lock || pp->p_keepcnt > 0)
				locked++;
			else
				inuse++;
		}

		/* convert memory stats to Kbytes */

		buf->total  = pagetok (inuse + free);
		buf->used   = pagetok (inuse);
		buf->free   = pagetok (free);
		buf->locked = pagetok (locked);

		buf->flags = _glibtop_sysdeps_mem;
	}
}
