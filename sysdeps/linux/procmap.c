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

#include <config.h>
#include <glib.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procmap.h>

#include <linux/kdev_t.h>
#include <stddef.h>

#include "glibtop_private.h"


#define  MAPS_FILE "/proc/%u/maps"
#define SMAPS_FILE "/proc/%u/smaps"


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

static const unsigned long _glibtop_sysdeps_map_entry_smaps =
(1UL << GLIBTOP_MAP_ENTRY_SIZE) + (1UL << GLIBTOP_MAP_ENTRY_RSS) +
(1UL << GLIBTOP_MAP_ENTRY_SHARED_DIRTY) + (1UL << GLIBTOP_MAP_ENTRY_SHARED_CLEAN) +
(1UL << GLIBTOP_MAP_ENTRY_PRIVATE_DIRTY) + (1UL << GLIBTOP_MAP_ENTRY_PRIVATE_CLEAN);


/* Init function. */

void
glibtop_init_proc_map_s (glibtop *server)
{
	server->sysdeps.proc_map = _glibtop_sysdeps_proc_map;
}

/* Provides detailed information about a process. */


static void
add_smaps(glibtop *server, FILE *smaps, glibtop_map_entry *entry)
{
#define SMAP_OFFSET(MEMBER) offsetof(glibtop_map_entry, MEMBER)

	struct smap_value {
		const char *name;
		ptrdiff_t offset;
	};

	const struct smap_value values[] = {
		{ "Size:",		SMAP_OFFSET(size) },
		{ "Rss:",		SMAP_OFFSET(rss) },
		{ "Shared_Clean:",	SMAP_OFFSET(shared_clean) },
		{ "Shared_Dirty:",	SMAP_OFFSET(shared_dirty) },
		{ "Private_Clean:",	SMAP_OFFSET(private_clean) },
		{ "Private_Dirty:",	SMAP_OFFSET(private_dirty) }
	};

	size_t i;

	for (i = 0; i < G_N_ELEMENTS(values); ++i) {
		char line[80];
		char *offset;
		guint64 *value;

		if (!fgets(line, sizeof line, smaps)) {
			glibtop_warn_io_r(server,
					  "Could not read smaps value %s",
					  values[i].name);
			return;
		}

		offset = (void*) entry;
		offset += values[i].offset;
		value = (void*) offset;

		*value = get_scaled(line, values[i].name);
	}

	entry->flags |= _glibtop_sysdeps_map_entry_smaps;

#undef SMAP_OFFSET
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

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_MAP, 0);

	memset (buf, 0, sizeof (glibtop_proc_map));

	if (server->os_version_code >= LINUX_VERSION_CODE(2, 6, 0))
		filename = SMAPS_FILE;
	else
		filename = MAPS_FILE;

	snprintf (procfilename, sizeof procfilename, filename, (unsigned)pid);

	if((maps = fopen (procfilename, "r")) == NULL) {
	  return (glibtop_map_entry*) g_array_free(entry_list, TRUE);
	}

	while(TRUE)
	{
		char line[1024];

		unsigned long perm = 0;
		int rv;
		guint len;

		unsigned short dev_major, dev_minor;
		unsigned long start, end, offset, inode;
		char flags[4];
		char filename [GLIBTOP_MAP_FILENAME_LEN+1];

		glibtop_map_entry *entry;

		if (!fgets(line, sizeof line, maps))
			break;

		/* 8 arguments */
		rv = sscanf(line, PROC_MAPS_FORMAT,
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

		/*
		  avoid copying the entry, grow by 1 and point to the last
		  element.
		*/
		len = entry_list->len;
		g_array_set_size(entry_list, len + 1);
		entry = &g_array_index(entry_list, glibtop_map_entry, len);

		entry->flags = _glibtop_sysdeps_map_entry;
		entry->start = (guint64) start;
		entry->end = (guint64) end;
		entry->offset = (guint64) offset;
		entry->perm = (guint64) perm;
		entry->device = (guint64) MKDEV(dev_major, dev_minor);
		entry->inode = (guint64) inode;
		g_strlcpy(entry->filename, filename, sizeof entry->filename);

		if (server->os_version_code >= LINUX_VERSION_CODE(2, 6, 0))
			add_smaps(server, maps, entry);

	}

	fclose (maps);

	buf->flags = _glibtop_sysdeps_proc_map;

	buf->number = entry_list->len;
	buf->size = sizeof (glibtop_map_entry);
	buf->total = buf->number * buf->size;

	return (glibtop_map_entry*) g_array_free(entry_list, FALSE);
}
