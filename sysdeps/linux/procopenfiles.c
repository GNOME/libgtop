/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   Copyright (C) 2004 Nicolás Lichtmaier
   This file is part of LibGTop 1.0.

   Modified by Nicolás Lichtmaier to give a process open files.

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
#include <glibtop/procopenfiles.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>


static const unsigned long _glibtop_sysdeps_proc_open_files =
(1L << GLIBTOP_PROC_OPEN_FILES_NUMBER)|
(1L << GLIBTOP_PROC_OPEN_FILES_TOTAL)|
(1L << GLIBTOP_PROC_OPEN_FILES_SIZE);

/* Init function. */

void
glibtop_init_proc_open_files_s (glibtop *server)
{
	server->sysdeps.proc_open_files = _glibtop_sysdeps_proc_open_files;
}


static void
get_socket_endpoint(char *buf, int *prmtport, int s)
{
	FILE *f;
	char l[1024];

	buf[0] = '\0';
	prmtport = 0;

	f = fopen("/proc/net/tcp", "r");

	if(!f)
		return;

	while(fgets(l, sizeof l, f))
	{
		unsigned int loc, locport, rmt;
		int sock = -42;
		/* FIXME
		  sscanf(l, "%*d: %8x:%4x %8x:%4x %*x %*x:%*x %*x:%*x %*d %*d %*d %d",
		&loc, &locport, &rmt, prmtport, &sock); */
		if(sock == s)
		{
			inet_ntop(AF_INET, &rmt, buf, GLIBTOP_OPEN_DEST_HOST_LEN);
			break;
		}
	}

	fclose(f);
}

/* Provides detailed information about a process' open files */

glibtop_open_files_entry *
glibtop_get_proc_open_files_s (glibtop *server, glibtop_proc_open_files *buf,	pid_t pid)
{
	char fn [BUFSIZ];
	GArray *entries;
	struct dirent *direntry;
	int rv;
	DIR *dir;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_OPEN_FILES, 0);

	memset (buf, 0, sizeof (glibtop_proc_open_files));

	sprintf (fn, "/proc/%d/fd", pid);

	dir = opendir (fn);
	if (!dir) return NULL;

	entries = g_array_new(FALSE, FALSE, sizeof(glibtop_open_files_entry));

	while((direntry = readdir(dir))) {
		char tgt [BUFSIZ];
		glibtop_open_files_entry entry = {0};


		if(direntry->d_name[0] == '.')
			continue;

		g_snprintf(fn, sizeof fn, "/proc/%d/fd/%s",
			   pid, direntry->d_name);

		rv = readlink(fn, tgt, sizeof(tgt) - 1);
		if(rv < 0) continue;
		tgt[rv] = '\0';

		entry.fd = atoi(direntry->d_name);

		if(g_str_has_prefix(tgt, "socket:["))
		{
			int sockfd;
			entry.type = GLIBTOP_FILE_TYPE_INETSOCKET;
			sockfd = atoi(tgt + 8);
			get_socket_endpoint(entry.info.sock.dest_host,
					    &(entry.info.sock.dest_port),
					    sockfd);
		}
		else if(g_str_has_prefix(tgt, "pipe:["))
		{
			entry.type = GLIBTOP_FILE_TYPE_PIPE;
		}
		else
		{
			entry.type = GLIBTOP_FILE_TYPE_FILE;
			g_strlcpy(entry.info.file.name, tgt, sizeof entry.info.file.name);
		}

		g_array_append_val(entries, entry);
	}

	closedir (dir);

	buf->flags = _glibtop_sysdeps_proc_open_files;
	buf->number = entries->len;
	buf->size = sizeof(glibtop_open_files_entry);
	buf->total = buf->number * buf->size;

	return (glibtop_open_files_entry*)g_array_free(entries, FALSE);
}
