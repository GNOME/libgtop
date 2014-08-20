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
#include <glib.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procmap.h>

#include <stddef.h>

#include "glibtop_private.h"

#define MKDEV(ma,mi)    (((ma) << 20) | (mi))

#define  MAPS_FILE "/proc/%u/maps"


#define PROC_MAPS_FORMAT "%16" G_GINT64_MODIFIER "x-%16" G_GINT64_MODIFIER "x %4c %16" G_GINT64_MODIFIER "x %02hx:%02hx %" G_GINT64_MODIFIER "u%*[ ]%n"


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
_glibtop_init_proc_map_s (glibtop *server)
{
	server->sysdeps.proc_map = _glibtop_sysdeps_proc_map;
}


glibtop_map_entry *
glibtop_get_proc_map_s (glibtop *server, glibtop_proc_map *buf,	pid_t pid)
{
	char procfilename[GLIBTOP_MAP_FILENAME_LEN+1];

	/*
	  default size of 100 maybe inaccurate.
	  It's the average number of entry per process on my laptop
	*/

	GArray *entry_list = g_array_sized_new(FALSE, FALSE,
					       sizeof(glibtop_map_entry),
					       100);
	FILE *maps;
	const char *filename;
	char *line = NULL;
	size_t line_size = 0;

	memset (buf, 0, sizeof (glibtop_proc_map));

	filename = MAPS_FILE;

	snprintf (procfilename, sizeof procfilename, filename, (unsigned)pid);

	if((maps = fopen (procfilename, "r")) == NULL) {
	  return (glibtop_map_entry*) g_array_free(entry_list, TRUE);
	}

	while(TRUE)
	{
		unsigned long perm;
		guint len;
		int line_end;

		unsigned short dev_major, dev_minor;
		guint64 start, end, offset, inode;
		char flags[4];
		char *filename;

		glibtop_map_entry *entry;

		if (getline(&line, &line_size, maps) == -1)
			break;

	new_entry_line:

		if (sscanf(line, PROC_MAPS_FORMAT,
			   &start, &end, flags, &offset,
			   &dev_major, &dev_minor, &inode, &line_end) != 7)
			continue;

		filename = line + line_end;
		g_strstrip(filename);

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
		else if (flags [3] == 'p')
			perm |= GLIBTOP_MAP_PERM_PRIVATE;

		/*
		  avoid copying the entry, grow by 1 and point to the last
		  element.
		*/
		len = entry_list->len;
		g_array_set_size(entry_list, len + 1);
		entry = &g_array_index(entry_list, glibtop_map_entry, len);

		entry->flags = _glibtop_sysdeps_map_entry;
		entry->start = start;
		entry->end = end;
		entry->offset = offset;
		entry->perm = perm;
		entry->device = MKDEV(dev_major, dev_minor);
		entry->inode = inode;
		g_strlcpy(entry->filename, filename, sizeof entry->filename);
	}

eof:

	free(line);
	fclose (maps);

	buf->flags = _glibtop_sysdeps_proc_map;

	buf->number = entry_list->len;
	buf->size = sizeof (glibtop_map_entry);
	buf->total = buf->number * buf->size;

	return (glibtop_map_entry*) g_array_free(entry_list, FALSE);
}
