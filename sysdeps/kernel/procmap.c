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
#include <glibtop/xmalloc.h>
#include <glibtop/procmap.h>

#include <glibtop_private.h>

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
    glibtop_map_entry *retval = NULL;
    libgtop_proc_maps_t *maps;
    size_t count, max_len, i;
    int ret;

    glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_MAP, 0);
	
    memset (buf, 0, sizeof (glibtop_proc_map));

    /* Get number of map entries. */
    count = glibtop_get_proc_data_proc_maps_s (server, pid, NULL, 0);

    /* Allocate memory. */
    maps = glibtop_calloc_r (server, count, sizeof (libgtop_proc_maps_t));
    max_len = count * sizeof (libgtop_proc_maps_t);

    ret = glibtop_get_proc_data_proc_maps_s (server, pid, maps, max_len);
    if (ret < 0) {
	glibtop_free_r (server, maps);
	return NULL;
    }

    /* Calculate number of map entries. */
    count = ret / sizeof (libgtop_proc_maps_t);

    /* Allocate memory for the result. */
    retval = glibtop_calloc_r (server, count, sizeof (glibtop_map_entry));

    for (i = 0; i < count; i++) {
	char *filename;

	retval [i].start = maps [i].header.start;
	retval [i].end = maps [i].header.end;
	retval [i].offset = maps [i].header.offset;

	if (maps [i].header.perm & LIBGTOP_VM_READ)
	    retval [i].perm |= GLIBTOP_MAP_PERM_READ;
	if (maps [i].header.perm & LIBGTOP_VM_WRITE)
	    retval [i].perm |= GLIBTOP_MAP_PERM_WRITE;
	if (maps [i].header.perm & LIBGTOP_VM_EXEC)
	    retval [i].perm |= GLIBTOP_MAP_PERM_EXECUTE;
	if (maps [i].header.perm & LIBGTOP_VM_SHARED)
	    retval [i].perm |= GLIBTOP_MAP_PERM_SHARED;
	if (!(maps [i].header.perm & LIBGTOP_VM_MAYSHARE))
	    retval [i].perm |= GLIBTOP_MAP_PERM_PRIVATE;

	retval [i].device = maps [i].header.device;
	retval [i].inode = maps [i].header.inode;

	filename = maps [i].filename;
	filename += maps [i].header.filename_offset;

	strncpy (retval [i].filename, filename, GLIBTOP_MAP_FILENAME_LEN);
	retval [i].filename [GLIBTOP_MAP_FILENAME_LEN-1] = '\0';

	retval [i].flags = _glibtop_sysdeps_map_entry;
    }

    /* Free map entries. */
    glibtop_free_r (server, maps);

    /* Write retval. */
    buf->number = count;
    buf->size = sizeof (glibtop_map_entry);
    buf->total = buf->number * sizeof (glibtop_map_entry);

    buf->flags = _glibtop_sysdeps_proc_map;

    return retval;
}
