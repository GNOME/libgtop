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

#include <config.h>
#include <glibtop/xmalloc.h>
#include <glibtop/proclist.h>

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>

static const unsigned long _glibtop_sysdeps_proclist =
(1 << GLIBTOP_PROCLIST_TOTAL) + (1 << GLIBTOP_PROCLIST_NUMBER) +
(1 << GLIBTOP_PROCLIST_SIZE);

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
glibtop_get_proclist_s (glibtop *server, glibtop_proclist *buf)
{
	DIR *proc;
	struct dirent *entry;
	char buffer [BUFSIZ];
	unsigned count, total, pid;
	unsigned pids [BLOCK_COUNT], *pids_chain = NULL;
	unsigned pids_size = 0, pids_offset = 0, new_size;
	struct stat statb;
	int len, i, ok;

	glibtop_init_s (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_proclist));

	proc = opendir ("/proc");
	if (!proc) return NULL;

	/* read every every entry in /proc */

	for (count = total = 0, entry = readdir (proc); entry; entry = readdir (proc)) {
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
		
		pids [count++] = pid;

		total++;
	}
	
	closedir (proc);

	/* count is only zero if an error occured (one a running Linux system, we
	 * only have at least one single process). */

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
