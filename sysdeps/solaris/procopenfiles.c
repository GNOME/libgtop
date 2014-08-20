/* Copyright (C) 2006 Henry Zhang
   This file is part of LibGTop 2.14.

   Contributed by Henry Zhang <hua.zhang@sun.com>, July 2006.

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
#include <glibtop/procopenfiles.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

#include "glibtop_private.h"

static const unsigned long _glibtop_sysdeps_proc_open_files =
(1L << GLIBTOP_PROC_OPEN_FILES_NUMBER)|
(1L << GLIBTOP_PROC_OPEN_FILES_TOTAL)|
(1L << GLIBTOP_PROC_OPEN_FILES_SIZE);

/* Init function. */

void
_glibtop_init_proc_open_files_s (glibtop *server)
{
	server->sysdeps.proc_open_files = _glibtop_sysdeps_proc_open_files;
}



/* Provides detailed information about a process' open files */

glibtop_open_files_entry *
glibtop_get_proc_open_files_s (glibtop *server, glibtop_proc_open_files *buf,	pid_t pid)
{
	char filename [BUFSIZ];
	GArray *entries;
	struct dirent *direntry;
	DIR *dir;
	int errno;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_OPEN_FILES, 0);

	memset (buf, 0, sizeof (glibtop_proc_open_files));

	sprintf (filename, "/proc/%d/fd", pid);

	dir = opendir (filename);
	if (!dir) return NULL;

	entries = g_array_new(FALSE, FALSE, sizeof(glibtop_open_files_entry));

	while((direntry = readdir(dir))) {
		char tgt [BUFSIZ];
		int rv;
		glibtop_open_files_entry entry = {0};
		struct stat statbuf;

		if(direntry->d_name[0] == '.')
			continue;

		if ((entry.fd = (int) g_ascii_strtoull(direntry->d_name, NULL, 10)) == 0)
			continue;
					
		/* from /path get object name */
		g_snprintf(filename, sizeof filename, "/proc/%d/path/%s",
			   pid, direntry->d_name);

		rv = readlink(filename, tgt, sizeof(tgt) - 1);
		/* read object, if not have, set it as NULL, but this fd still need to insert into the array */
		if(rv < 0) 
			rv = 0;
		tgt[rv] = '\0';
		
		/* from /fd get the stat data */
		g_snprintf(filename, sizeof filename, "/proc/%d/fd/%s",
			   pid, direntry->d_name);

		if(stat (filename, &statbuf))
			statbuf.st_mode = 0;		

		switch (statbuf.st_mode & S_IFMT) {
			case S_IFIFO:    /* pipe */
				entry.type = GLIBTOP_FILE_TYPE_PIPE;
				break;
			case S_IFSOCK:	/* socket */
			/* at solaris, now a little difficult to tell the Socket type, so here I 
			   give the type 0, it will not impact the existed code. Later will provide 
			   a patch to tell the type, and get the object name */
				entry.type = 0;
				break;
			default:
				entry.type = GLIBTOP_FILE_TYPE_FILE;
		}
				
		g_strlcpy(entry.info.file.name, tgt, sizeof entry.info.file.name);

		g_array_append_val(entries, entry);
	}

	closedir (dir);

	buf->flags = _glibtop_sysdeps_proc_open_files;
	buf->number = entries->len;
	buf->size = sizeof(glibtop_open_files_entry);
	buf->total = buf->number * buf->size;

	return (glibtop_open_files_entry*)g_array_free(entries, FALSE);
}
