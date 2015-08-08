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
#include <glibtop/error.h>
#include <glibtop/procmap.h>

#include <errno.h>

#include "safeio.h"


static const unsigned long _glibtop_sysdeps_proc_map =
(1L << GLIBTOP_PROC_MAP_NUMBER) + (1L << GLIBTOP_PROC_MAP_TOTAL) +
(1L << GLIBTOP_PROC_MAP_SIZE);
static const unsigned long _glibtop_sysdeps_map_entry =
(1L << GLIBTOP_MAP_ENTRY_START) + (1L << GLIBTOP_MAP_ENTRY_END) +
(1L << GLIBTOP_MAP_ENTRY_OFFSET) + (1L << GLIBTOP_MAP_ENTRY_PERM);
static const unsigned long _glibtop_sysdeps_map_device =
(1L << GLIBTOP_MAP_ENTRY_DEVICE) + (1L << GLIBTOP_MAP_ENTRY_INODE);


/* Init function. */

void
_glibtop_init_proc_map_s (glibtop *server)
{
	server->sysdeps.proc_map = _glibtop_sysdeps_proc_map;
}

/* Provides detailed information about a process. */

glibtop_map_entry *
glibtop_get_proc_map_s (glibtop *server, glibtop_proc_map *buf,	pid_t pid)
{
   	int fd, i, nmaps, pr_err, heap;
#if GLIBTOP_SOLARIS_RELEASE >= 50600
	prxmap_t *maps;
	struct ps_prochandle *Pr = NULL;
#else
	prmap_t *maps;
#endif

	/* A few defines, to make it shorter down there */

#ifdef HAVE_PROCFS_H
# define OFFSET  pr_offset
#else
# define OFFSET  pr_off
#endif

	glibtop_map_entry *entry;
	struct stat inode;
	char buffer[BUFSIZ];

	memset (buf, 0, sizeof (glibtop_proc_map));

#ifdef HAVE_PROCFS_H
	sprintf(buffer, "/proc/%d/xmap", (int)pid);
#else
	sprintf(buffer, "/proc/%d", (int)pid);
#endif
	if((fd = s_open(buffer, O_RDONLY)) < 0)
	{
	   	if (errno != EPERM && errno != EACCES)
		   	glibtop_warn_io_r(server, "open (%s)", buffer);
		return NULL;
	}
#ifdef HAVE_PROCFS_H
	if(fstat(fd, &inode) < 0)
	{
	   	if(errno != EOVERFLOW)
		   	glibtop_warn_io_r(server, "fstat (%s)", buffer);
		/* else call daemon for 64-bit support */
		s_close(fd);
		return NULL;
	}
	maps = g_alloca(inode.st_size);
	nmaps = inode.st_size / sizeof(prxmap_t);
	if(s_pread(fd, maps, inode.st_size, 0) != inode.st_size)
	{
	   	glibtop_warn_io_r(server, "pread (%s)", buffer);
		s_close(fd);
		return NULL;
	}
#else
	if(ioctl(fd, PIOCNMAP, &nmaps) < 0)
	{
	   	glibtop_warn_io_r(server, "ioctl(%s, PIOCNMAP)", buffer);
		s_close(fd);
		return NULL;
	}
	maps = g_alloca((nmaps + 1) * sizeof(prmap_t));
	if(ioctl(fd, PIOCMAP, maps) < 0)
	{
	   	glibtop_warn_io_r(server, "ioctl(%s, PIOCMAP)", buffer);
		s_close(fd);
		return NULL;
	}
#endif
	buf->number = nmaps;
	buf->size = sizeof(glibtop_map_entry);
	buf->total = nmaps * sizeof(glibtop_map_entry);
	entry = g_malloc0(buf->total);

#if GLIBTOP_SOLARIS_RELEASE >= 50600

	if(server->machine->objname && server->machine->pgrab &&
	   server->machine->pfree)
	   Pr = (server->machine->pgrab)(pid, 1, &pr_err);
#endif
	for(heap = 0,i = 0; i < nmaps; ++i)
	{
	   	int len;

	   	entry[i].start = maps[i].pr_vaddr;
		entry[i].end = maps[i].pr_vaddr + maps[i].pr_size;

#if GLIBTOP_SOLARIS_RELEASE >= 50600

		if(maps[i].pr_dev != PRNODEV)
		{
		   entry[i].device = maps[i].pr_dev;
		   entry[i].inode = maps[i].pr_ino;
		   entry[i].flags |= _glibtop_sysdeps_map_device;
		}
#endif
		entry[i].offset = maps[i].OFFSET;
		if(maps[i].pr_mflags & MA_READ)
		   	entry[i].perm |= GLIBTOP_MAP_PERM_READ;
		if(maps[i].pr_mflags & MA_WRITE){
		   	entry[i].perm |= GLIBTOP_MAP_PERM_WRITE;
		   	entry[i].size = maps[i].pr_size;
		}
		if(maps[i].pr_mflags & MA_EXEC)
		   	entry[i].perm |= GLIBTOP_MAP_PERM_EXECUTE;
		if(maps[i].pr_mflags & MA_SHARED)
		   	entry[i].perm |= GLIBTOP_MAP_PERM_SHARED;
		else
		   	entry[i].perm |= GLIBTOP_MAP_PERM_PRIVATE;
		entry[i].flags = _glibtop_sysdeps_map_entry;

#if GLIBTOP_SOLARIS_RELEASE >= 50600

		if(maps[i].pr_mflags & MA_ANON)
		{
		   if(!heap)
		   {
		      ++heap;
		      strcpy(entry[i].filename, "[ heap ]");
		   }
		   else
		      if(i == nmaps - 1)
			 strcpy(entry[i].filename, "[ stack ]");
		      else
			 strcpy(entry[i].filename, "[ anon ]");
		   entry[i].flags |= (1L << GLIBTOP_MAP_ENTRY_FILENAME);
		}
		else
		   if(Pr)
		   {
		      server->machine->objname(Pr, maps[i].pr_vaddr, buffer,
					      BUFSIZ);
		      if((len = resolvepath(buffer, entry[i].filename,
					    GLIBTOP_MAP_FILENAME_LEN)) > 0)
		      {
			 entry[i].filename[len] = 0;
			 entry[i].flags |= (1L << GLIBTOP_MAP_ENTRY_FILENAME);
		      }
		   }
#endif
	}

#if GLIBTOP_SOLARIS_RELEASE >= 50600

	if(Pr)
	   	server->machine->pfree(Pr);
#endif
	buf->flags = _glibtop_sysdeps_proc_map;
	s_close(fd);
	return entry;
}
