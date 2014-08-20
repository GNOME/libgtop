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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <config.h>
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
_glibtop_init_proc_open_files_s (glibtop *server)
{
	server->sysdeps.proc_open_files = _glibtop_sysdeps_proc_open_files;
}



typedef void (*LineParser)(GHashTable *dict, const char *line);


static void
parse_file(const char *filename, LineParser parser, GHashTable *dict)
{
	FILE *f;
	char *line = NULL;
	size_t size = 0;

	f = fopen(filename, "r");

	if(!f) {
		g_warning("Cannot open '%s'", filename);
		return;
	}


	/* skip the first line */
	if (getline(&line, &size, f) == -1)
		goto eof;

	while (getline(&line, &size, f) != -1)
		parser(dict, line);

 eof:
	free(line);
	fclose(f);
}


static GHashTable*
get_all(const char *filename, LineParser parser)
{
	GHashTable *dict;

	dict = g_hash_table_new_full(g_direct_hash, g_direct_equal,
				     NULL, g_free);

	parse_file(filename, parser, dict);

	return dict;
}





struct Inet6SocketEntry
{
	char host[GLIBTOP_OPEN_DEST_HOST_LEN + 1];
	int port;
};


static void
inet6_socket_parser(GHashTable *dict, const char* line)
{
	struct Inet6SocketEntry *se;
	int sock;
	struct in6_addr addr;

	se = g_malloc0(sizeof *se);

	if(sscanf(line, "%*d: %*s %8x%8x%8x%8x:%4x %*x %*x:%*x %*x:%*x %*d %*d %*d %d",
		  &addr.s6_addr32[0], &addr.s6_addr32[1], &addr.s6_addr32[2],
		  &addr.s6_addr32[3], &se->port, &sock) != 6)
		goto error;

	if(!inet_ntop(AF_INET6, &addr, se->host, sizeof se->host))
		goto error;

	g_hash_table_insert(dict, GINT_TO_POINTER(sock), se);
	return;

 error:
	g_free(se);
}


static inline GHashTable *
get_all_inet6_sockets()
{
	return get_all("/proc/net/tcp6", inet6_socket_parser);
}





struct InetSocketEntry
{
	char host[GLIBTOP_OPEN_DEST_HOST_LEN + 1];
	int port;
};


static void
inet_socket_parser(GHashTable *dict, const char* line)
{
	struct InetSocketEntry *se;
	int sock;
	unsigned addr;

	se = g_malloc0(sizeof *se);

	if(sscanf(line, "%*d: %*x:%*x %8x:%4x %*x %*x:%*x %*x:%*x %*d %*d %*d %d",
		  &addr, &se->port, &sock) != 3)
		goto error;

	if(!inet_ntop(AF_INET, &addr, se->host, sizeof se->host))
		goto error;

	g_hash_table_insert(dict, GINT_TO_POINTER(sock), se);
	return;

 error:
	g_free(se);
}


static inline GHashTable *
get_all_inet_sockets()
{
	return get_all("/proc/net/tcp", inet_socket_parser);
}





struct LocalSocketEntry
{
	char name[GLIBTOP_OPEN_DEST_HOST_LEN + 1];
};


static void
local_socket_parser(GHashTable *dict, const char *line)
{
	int sock;
	struct LocalSocketEntry *use;
	char *p;

	use = g_malloc0(sizeof *use);

	/* dfaf1640: 00000003 00000000 00000000 0001 03  6457 /dev/log */
	p = skip_multiple_token(line, 6);

	sock = strtoul(p, &p, 10);
	g_strlcpy(use->name, p, sizeof use->name);
	g_strstrip(use->name);
	g_hash_table_insert(dict, GINT_TO_POINTER(sock), use);
}


static inline GHashTable *
get_all_local_sockets()
{
	return get_all("/proc/net/unix", local_socket_parser);
}



/* Provides detailed information about a process' open files */

glibtop_open_files_entry *
glibtop_get_proc_open_files_s (glibtop *server, glibtop_proc_open_files *buf,	pid_t pid)
{
	char fn [BUFSIZ];
	GArray *entries;
	GHashTable *inet6_sockets = NULL, *inet_sockets = NULL, *local_sockets = NULL;
	struct dirent *direntry;
	DIR *dir;

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

		if (!safe_readlink(fn, tgt, sizeof tgt))
			continue;

		entry.fd = atoi(direntry->d_name);

		if(g_str_has_prefix(tgt, "socket:["))
		{
			int sockfd;
			struct Inet6SocketEntry *i6se;
			struct InetSocketEntry *ise;
			struct LocalSocketEntry *lse;

			if(!inet6_sockets) inet6_sockets = get_all_inet6_sockets();
			if(!inet_sockets) inet_sockets = get_all_inet_sockets();
			if(!local_sockets) local_sockets = get_all_local_sockets();

			sockfd = atoi(tgt + 8);

			i6se = g_hash_table_lookup(inet6_sockets,
						 GINT_TO_POINTER(sockfd));

			if(i6se) {
				entry.type = GLIBTOP_FILE_TYPE_INET6SOCKET;
				entry.info.sock.dest_port = i6se->port;
				g_strlcpy(entry.info.sock.dest_host, i6se->host,
					  sizeof entry.info.sock.dest_host);
				goto found;
			}

			ise = g_hash_table_lookup(inet_sockets,
						 GINT_TO_POINTER(sockfd));

			if(ise) {
				entry.type = GLIBTOP_FILE_TYPE_INETSOCKET;
				entry.info.sock.dest_port = ise->port;
				g_strlcpy(entry.info.sock.dest_host, ise->host,
					  sizeof entry.info.sock.dest_host);
				goto found;
			}

			lse = g_hash_table_lookup(local_sockets,
						  GINT_TO_POINTER(sockfd));

			if(lse) {
				entry.type = GLIBTOP_FILE_TYPE_LOCALSOCKET;
				g_strlcpy(entry.info.localsock.name, lse->name,
					  sizeof entry.info.localsock.name);
				goto found;
			}

		found:
			(void)0; /* kills warning */
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

	if(inet_sockets) g_hash_table_destroy(inet_sockets);
	if(inet6_sockets) g_hash_table_destroy(inet6_sockets);
	if(local_sockets) g_hash_table_destroy(local_sockets);

	buf->flags = _glibtop_sysdeps_proc_open_files;
	buf->number = entries->len;
	buf->size = sizeof(glibtop_open_files_entry);
	buf->total = buf->number * buf->size;

	return (glibtop_open_files_entry*)g_array_free(entries, FALSE);
}
