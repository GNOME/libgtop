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

#include <errno.h>
#include <alloca.h>

static const unsigned long _glibtop_sysdeps_proc_map =
(1L << GLIBTOP_PROC_MAP_NUMBER) + (1L << GLIBTOP_PROC_MAP_TOTAL) +
(1L << GLIBTOP_PROC_MAP_SIZE);
static const unsigned long _glibtop_sysdeps_map_entry =
(1L << GLIBTOP_MAP_ENTRY_START) + (1L << GLIBTOP_MAP_ENTRY_END) +
(1L << GLIBTOP_MAP_ENTRY_OFFSET) + (1L << GLIBTOP_MAP_ENTRY_PERM);


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
   	int fd, i, nmaps;
	prmap_t *maps;
	glibtop_map_entry *entry;
	struct stat inode;
	char buffer[BUFSIZ];
	
	memset (buf, 0, sizeof (glibtop_proc_map));

	sprintf(buffer, "/proc/%d/map", (int)pid);
	if((fd = open(buffer, O_RDONLY)) < 0)
	{
	   	if(errno != EPERM && errno != EACCES)
		   	glibtop_warn_io_r(server, "open (%s)", buffer);
		return;
	}
	if(fstat(fd, &inode) < 0)
	{
	   	if(errno != EOVERFLOW)
		   	glibtop_warn_io_r(server, "fstat (%s)", buffer);
		/* else call daemon for 64-bit support */
		close(fd);
		return;
	}
	maps = alloca(inode.st_size);
	nmaps = inode.st_size / sizeof(prmap_t);
	if(pread(fd, maps, inode.st_size, 0) != inode.st_size)
	{
	   	glibtop_warn_io_r(server, "pread (%s)", buffer);
		close(fd);
		return;
	}
	close(fd);
	if(!(entry = glibtop_malloc_r(server, nmaps * sizeof(glibtop_map_entry))))
	   	return;

	buf->number = nmaps;
	buf->size = sizeof(glibtop_map_entry);
	buf->total = nmaps * sizeof(glibtop_map_entry);

	memset(entry, 0, nmaps * sizeof(glibtop_map_entry));
	for(i = 0; i < nmaps; ++i)
	{
	   	entry[i].start = maps[i].pr_vaddr;
		entry[i].end = maps[i].pr_vaddr + maps[i].pr_size;
		entry[i].offset = maps[i].pr_offset;
		if(maps[i].pr_mflags & MA_READ)
		   	entry[i].perm |= GLIBTOP_MAP_PERM_READ;
		if(maps[i].pr_mflags & MA_WRITE)
		   	entry[i].perm |= GLIBTOP_MAP_PERM_WRITE;
		if(maps[i].pr_mflags & MA_EXEC)
		   	entry[i].perm |= GLIBTOP_MAP_PERM_EXECUTE;
		if(maps[i].pr_mflags & MA_SHARED)
		   	entry[i].perm |= GLIBTOP_MAP_PERM_SHARED;
		else
		   	entry[i].perm |= GLIBTOP_MAP_PERM_PRIVATE;
		entry[i].flags = _glibtop_sysdeps_map_entry;
	}
	
	buf->flags = _glibtop_sysdeps_proc_map;
	return entry;
}
