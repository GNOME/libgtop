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
#include <glibtop/error.h>
#include <glibtop/procmap.h>


#define PROC_MAPS_FORMAT ((sizeof(void*) == 8) \
? "%16lx-%16lx %4c %16lx %02hx:%02hx %lu%*[ ]%[^\n]\n" \
: "%08lx-%08lx %4c %08lx %02hx:%02hx %lu%*[ ]%[^\n]\n")


static const unsigned long _glibtop_sysdeps_proc_map =
(1L << GLIBTOP_PROC_MAP_NUMBER) + (1L << GLIBTOP_PROC_MAP_TOTAL) +
(1L << GLIBTOP_PROC_MAP_SIZE);

static const unsigned long _glibtop_sysdeps_map_entry =
(1L << GLIBTOP_MAP_ENTRY_START) + (1L << GLIBTOP_MAP_ENTRY_END) +
(1L << GLIBTOP_MAP_ENTRY_OFFSET) + (1L << GLIBTOP_MAP_ENTRY_PERM) +
(1L << GLIBTOP_MAP_ENTRY_INODE) + (1L << GLIBTOP_MAP_ENTRY_DEVICE) +
(1L << GLIBTOP_MAP_ENTRY_FILENAME);

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
	char filename [GLIBTOP_MAP_FILENAME_LEN+1];
	glibtop_map_entry *entry_list;
	gsize allocated;
	FILE *maps;

	/* fscanf args */
	unsigned short dev_major, dev_minor;
	unsigned long start, end, offset, inode;
	char flags[4];
	/* filename is the 8th argument */

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_MAP, 0);

	memset (buf, 0, sizeof (glibtop_proc_map));

	sprintf (filename, "/proc/%d/maps", pid);

	maps = fopen (filename, "r");
	if (!maps) return NULL;

	allocated = 32; /* magic */
	entry_list = g_new(glibtop_map_entry, allocated);

	for(buf->number = 0; TRUE; buf->number++)
	{
		unsigned long perm = 0;

		int rv;

		/* 8 arguments */
		rv = fscanf (maps, PROC_MAPS_FORMAT,
			     &start, &end, flags, &offset,
			     &dev_major, &dev_minor, &inode, filename);

		if(rv == EOF || rv < 7)
		  break;

		if(rv == 7) /* no filename */
		  filename[0] = '\0';

		/* Compute access permissions. */

		if (flags [0] == 'r')
			perm |= GLIBTOP_MAP_PERM_READ;

		if (flags [1] == 'w')
			perm |= GLIBTOP_MAP_PERM_WRITE;

		if (flags [2] == 'x')
			perm |= GLIBTOP_MAP_PERM_EXECUTE;

		if (flags [3] == 's')
			perm |= GLIBTOP_MAP_PERM_SHARED;
		else if (flags [3] == 'p')
			perm |= GLIBTOP_MAP_PERM_PRIVATE;


		if(buf->number == allocated) {
			/* grow by 2 and blank the newly allocated entries */
			entry_list = g_renew (glibtop_map_entry, entry_list, allocated * 2);
			allocated *= 2;
		}

		entry_list [buf->number].flags = _glibtop_sysdeps_map_entry;

		entry_list [buf->number].start = (guint64) start;
		entry_list [buf->number].end = (guint64) end;
		entry_list [buf->number].offset = (guint64) offset;
		entry_list [buf->number].perm = (guint64) perm;
		entry_list [buf->number].device = (guint64) (dev_major << 8) +
			(guint64) dev_minor;
		entry_list [buf->number].inode = (guint64) inode;

		g_strlcpy (entry_list [buf->number].filename, filename,
			   sizeof entry_list [buf->number].filename);
	}

	fclose (maps);

	buf->flags = _glibtop_sysdeps_proc_map;
	buf->size = sizeof (glibtop_map_entry);
	buf->total = buf->number * buf->size;

	g_renew(glibtop_map_entry, entry_list, buf->number);

	return entry_list;
}
