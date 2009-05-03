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

#include "procmap_smaps.c"

#define  MAPS_FILE "/proc/%u/maps"
#define SMAPS_FILE "/proc/%u/smaps"


#define PROC_MAPS_FORMAT "%16" G_GINT64_MODIFIER "x-%16" G_GINT64_MODIFIER "x %4c %16" G_GINT64_MODIFIER "x %02hx:%02hx %" G_GINT64_MODIFIER "u%*[ ]%n"


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
_glibtop_init_proc_map_s (glibtop *server)
{
	server->sysdeps.proc_map = _glibtop_sysdeps_proc_map;
}

/* Provides detailed information about a process. */


static const char*
is_smap_value(const char* s)
{
	for ( ; *s; ++s) {

		if (isspace(*s))
			return NULL;

		if (*s == ':')
			return s;
	}

	return NULL;
}


/*
  Returns whether line is a 'value' line
  and add if we know its meaning
*/
static gboolean
parse_smaps(glibtop_map_entry *entry, const char* line)
{

	const struct smap_value* smap;
	size_t len;
	const char* colon;

	if ((colon = is_smap_value(line)) == NULL)
		return FALSE;

	len = colon - line + 1;
	smap = _glibtop_find_smap(line, len);

//		g_debug("smap %s -> %p", line, smap);

	if (smap) {
		char *offset;
		guint64 *value;

		offset = (void*) entry;
		offset += smap->offset;
		value = (void*) offset;

		*value = get_scaled(line + len, NULL);
	}

	return TRUE;
}


/*
  sscanf is too slow
  and system-monitor calls procmap for each pid every second

  manual parsing is faster

  error checking is weaker
*/

static gboolean
parse_line(char* line,
	   guint64* start, guint64* end, char flags[4], guint64* offset,
	   gushort* dev_major, gushort* dev_minor, guint64* inode,
	   char** filename)
{
	/* %16llx-%16llx %4c %16llx %02hx:%02hx %llu%*[ ]%n */

	char *p, *next;

	p = line;

	*start = strtoull(p, &p, 16);

	if (G_UNLIKELY(*p != '-'))
		return FALSE;
	p++;

	*end = strtoull(p, &p, 16);

	p = next_token(p);

	memcpy(flags, p, 4);
	p += 4;

	*offset = strtoull(p, &p, 16);

	*dev_major = strtoul(p, &p, 16);

	if (G_UNLIKELY(*p != ':'))
		return FALSE;
	p++;

	*dev_minor = strtoul(p, &p, 16);

	*inode = strtoull(p, &p, 10);

	p = next_token(p);

	*filename = p;

	for ( ; *p; p++) {
		if (*p == '\n') {
			*p = '\0';
			break;
		}
	}

	return TRUE;
}





glibtop_map_entry *
glibtop_get_proc_map_s (glibtop *server, glibtop_proc_map *buf,	pid_t pid)
{
	char procfilename[GLIBTOP_MAP_FILENAME_LEN+1];

	/*
	  default size of 100 maybe inaccurate.
	  It's the average number of entry per process on my laptop
	*/

	size_t added = 0, entry_list_capacity = 100;
	GArray *entry_list = g_array_sized_new(FALSE, FALSE,
					       sizeof(glibtop_map_entry),
					       entry_list_capacity);
	FILE *maps;
	const char *filename;
	gboolean has_smaps;
	char *line = NULL;
	size_t line_size = 0;

	memset (buf, 0, sizeof (glibtop_proc_map));

	has_smaps = server->os_version_code >= LINUX_VERSION_CODE(2, 6, 14);

	if (has_smaps)
		filename = SMAPS_FILE;
	else
		filename = MAPS_FILE;

	snprintf (procfilename, sizeof procfilename, filename, (unsigned)pid);

	if((maps = fopen (procfilename, "r")) == NULL) {
	  return (glibtop_map_entry*) g_array_free(entry_list, TRUE);
	}

	while(TRUE)
	{
		unsigned long perm;
		/* int line_end; */

		unsigned short dev_major, dev_minor;
		guint64 start, end, offset, inode;
		char flags[4];
		char *filename;

		glibtop_map_entry *entry;

		if (getline(&line, &line_size, maps) == -1)
			break;

	new_entry_line:

		if (!parse_line(line,
				&start, &end, flags, &offset,
				&dev_major, &dev_minor, &inode, &filename))
			continue;

		/*
		if (sscanf(line, PROC_MAPS_FORMAT,
			   &start, &end, flags, &offset,
			   &dev_major, &dev_minor, &inode, &line_end) != 7)
			continue;

		filename = line + line_end;
		g_strstrip(filename);
		*/

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

		if (G_UNLIKELY(added >= entry_list_capacity)) {
			entry_list_capacity *= 2;
			g_array_set_size(entry_list, entry_list_capacity);
		}

		entry = &g_array_index(entry_list, glibtop_map_entry, added++);

		entry->flags = _glibtop_sysdeps_map_entry;
		entry->start = start;
		entry->end = end;
		entry->offset = offset;
		entry->perm = perm;
		entry->device = MKDEV(dev_major, dev_minor);
		entry->inode = inode;
		g_strlcpy(entry->filename, filename, sizeof entry->filename);

		if (has_smaps) {
			ssize_t ret;
			entry->flags |= _glibtop_sysdeps_map_entry_smaps;

			while ((ret = getline(&line, &line_size, maps)) != -1) {
				if (!parse_smaps(entry, line))
					goto new_entry_line;
			}

			if (ret == -1)
				goto eof;
		}
	}

eof:

	g_array_set_size(entry_list, added);
	free(line);
	fclose (maps);

	buf->flags = _glibtop_sysdeps_proc_map;

	buf->number = added;
	buf->size = sizeof (glibtop_map_entry);
	buf->total = buf->number * buf->size;

	return (glibtop_map_entry*) g_array_free(entry_list, FALSE);
}
