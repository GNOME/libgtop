/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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
#include <glibtop/procmap.h>

static const unsigned long _glibtop_sysdeps_proc_map =
(1 << GLIBTOP_PROC_MAP_NUMBER) + (1 << GLIBTOP_PROC_MAP_TOTAL) +
(1 << GLIBTOP_PROC_MAP_SIZE);

static const unsigned long _glibtop_sysdeps_map_entry =
(1 << GLIBTOP_MAP_ENTRY_START) + (1 << GLIBTOP_MAP_ENTRY_END) +
(1 << GLIBTOP_MAP_ENTRY_OFFSET) + (1 << GLIBTOP_MAP_ENTRY_PERM) +
(1 << GLIBTOP_MAP_ENTRY_INODE) + (1 << GLIBTOP_MAP_ENTRY_DEVICE) +
(1 << GLIBTOP_MAP_ENTRY_FILENAME);

/* Init function. */

void
glibtop_init_proc_map_s (glibtop *server)
{
	server->sysdeps.proc_map = _glibtop_sysdeps_proc_map;
}

/* Provides detailed information about a process. */

glibtop_map_entry *
glibtop_get_proc_map_s (glibtop *server, glibtop_proc_map *buf,	pid_t pid)
{
	char fn [BUFSIZ];
	glibtop_map_entry *entry_list = NULL;
	int rv, n = 0;
	FILE *maps;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_MAP, 0);
	
	memset (buf, 0, sizeof (glibtop_proc_map));

	sprintf (fn, "/proc/%d/maps", pid);

	maps = fopen (fn, "r");
	if (!maps) return NULL;

	do {
		short dev_major, dev_minor;
		unsigned long start, end, offset, inode, perm;
		char flags [5];
		size_t size;

		rv = fscanf (maps,
			     "%08lx-%08lx %4c\n %08lx %02hx:%02hx %ld",
			     &start, &end, flags, &offset,
			     &dev_major, &dev_minor, &inode);

		flags [4] = 0;

		/* Compute access permissions. */

		perm = 0;

		if (flags [0] == 'r')
			perm |= GLIBTOP_MAP_PERM_READ;
		if (flags [1] == 'w')
			perm |= GLIBTOP_MAP_PERM_WRITE;
		if (flags [2] == 'x')
			perm |= GLIBTOP_MAP_PERM_EXECUTE;
		if (flags [3] == 's')
			perm |= GLIBTOP_MAP_PERM_SHARED;
		if (flags [3] == 'p')
			perm |= GLIBTOP_MAP_PERM_PRIVATE;

		/* Read filename. */

		fn [0] = fgetc (maps);

		if (fn [0] != '\n' && fn [0] != EOF) {
			
			fscanf (maps, "%*[ ]%[^\n]\n", fn);
			
		} else fn [0] = 0;

		size = (n+1) * sizeof (glibtop_map_entry);

		entry_list = glibtop_realloc_r (server, entry_list, size);

		memset (&(entry_list [n]), 0, sizeof (glibtop_map_entry));

		entry_list [n].flags = _glibtop_sysdeps_map_entry;

		entry_list [n].start = (u_int64_t) start;
		entry_list [n].end = (u_int64_t) end;
		entry_list [n].offset = (u_int64_t) offset;
		entry_list [n].perm = (u_int64_t) perm;
		entry_list [n].device = (u_int64_t) (dev_major << 8) +
			(u_int64_t) dev_minor;
		entry_list [n].inode = (u_int64_t) inode;

		strncpy (entry_list [n].filename, fn, GLIBTOP_MAP_FILENAME_LEN);
		entry_list [n].filename [GLIBTOP_MAP_FILENAME_LEN] = 0;
		
		n++;

	} while (rv != EOF && rv && fn [0] != EOF);
	
	fclose (maps);

	buf->number = n;
	buf->size = sizeof (glibtop_map_entry);
	buf->total = n * sizeof (glibtop_map_entry);

	return entry_list;
}
