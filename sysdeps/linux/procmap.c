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

static const unsigned long _glibtop_sysdeps_proc_map = 0;

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
		unsigned long start, end, offset, inode;
		char flags [5];
		size_t size;

		rv = fscanf (maps,
			     "%08lx-%08lx %4c\n %08lx %02hx:%02hx %ld",
			     &start, &end, flags, &offset,
			     &dev_major, &dev_minor, &inode);

		flags [4] = 0;

		fn [0] = fgetc (maps);
		/* printf ("fn [0] '%c' %x\n", fn [0], fn [0]); */

		if (fn [0] != '\n' && fn [0] != EOF) {
			
			fscanf (maps, "%*[ ]%[^\n]\n", fn);
			/* row->filename = glibtop_strdup_r (server, fn); */
			
		} /* else row->filename = NULL; */

		/* if (rv != EOF)
		   printf ("%08lx-%08lx %s\n",
		   row->VMstart, row->VMend,
		   row->filename); */
		
		n++;

		size = n * sizeof (glibtop_map_entry);

		entry_list = glibtop_realloc_r (server, entry_list, size);

		entry_list [n-1].start = (u_int64_t) start;
		entry_list [n-1].end = (u_int64_t) end;
		entry_list [n-1].offset = (u_int64_t) offset;
		entry_list [n-1].device = (u_int64_t) (dev_major << 8) +
			(u_int64_t) dev_minor;
		entry_list [n-1].inode = (u_int64_t) inode;
		
	} while (rv != EOF && rv && fn [0] != EOF);
	
	/* printf ("allocated: %d\n", i); */

	fclose (maps);

	buf->number = n;
	buf->size = sizeof (glibtop_map_entry);
	buf->total = n * sizeof (glibtop_map_entry);

	return entry_list;
}
