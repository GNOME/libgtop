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

#include "glibtop_private.h"

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



struct SocketEndPoint
{
	char host[GLIBTOP_OPEN_DEST_HOST_LEN + 1];
	int port;
	int sock;
};



static GArray* get_all_sockets()
{
	GArray *socks;
	FILE *tcp;
	char line[1024];

	socks = g_array_new(FALSE, FALSE, sizeof(struct SocketEndPoint));

	g_return_val_if_fail((tcp = fopen("/proc/net/tcp", "r")), socks);

	if(!fgets(line, sizeof line, tcp)) goto eof;

	while(fgets(line, sizeof line, tcp))
	{
		struct SocketEndPoint sep;
		unsigned addr;

		if(sscanf(line, "%*d: %*x:%*x %8x:%4x %*x %*x:%*x %*x:%*x %*d %*d %*d %d",
			  &addr, &sep.port, &sep.sock) != 3)
			continue;

		if(!inet_ntop(AF_INET, &addr, sep.host, sizeof sep.host))
			continue;

		g_array_append_val(socks, sep);
	}

 eof:
	fclose(tcp);
	return socks;
}


/* Provides detailed information about a process' open files */

glibtop_open_files_entry *
glibtop_get_proc_open_files_s (glibtop *server, glibtop_proc_open_files *buf,	pid_t pid)
{
	char fn [BUFSIZ];
	GArray *entries;
	GArray *socks = NULL;
	struct dirent *direntry;
	DIR *dir;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_OPEN_FILES, 0);

	memset (buf, 0, sizeof (glibtop_proc_open_files));

	sprintf (fn, "/proc/%d/fd", pid);

	dir = opendir (fn);
	if (!dir) return NULL;

	entries = g_array_new(FALSE, FALSE, sizeof(glibtop_open_files_entry));

	while((direntry = readdir(dir))) {
		char tgt [BUFSIZ];
		int rv;
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
			unsigned i;
			int sockfd;

			if(!socks)
				socks = get_all_sockets();

			entry.type = GLIBTOP_FILE_TYPE_INETSOCKET;

			sockfd = atoi(tgt + 8);

			for(i = 0; i < socks->len; ++i)
			{
				const struct SocketEndPoint *sep;

				sep = & g_array_index(socks,
						      struct SocketEndPoint,
						      i);

				if(sep->sock == sockfd)
				{
					entry.info.sock.dest_port = sep->port;
					memcpy(entry.info.sock.dest_host,
					       sep->host,
					       sizeof sep->host);
					break;
				}
			}
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

	if(socks)
		g_array_free(socks, TRUE);

	buf->flags = _glibtop_sysdeps_proc_open_files;
	buf->number = entries->len;
	buf->size = sizeof(glibtop_open_files_entry);
	buf->total = buf->number * buf->size;

	return (glibtop_open_files_entry*)g_array_free(entries, FALSE);
}
