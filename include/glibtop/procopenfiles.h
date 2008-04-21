/* Copyright (C) 1998-99 Martin Baulig
   Copyright (C) 2004 Nicolás Lichtmaier
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   Modified by Nicolás Lichtmaier to give open process files.

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

#ifndef __GLIBTOP_PROC_OPEN_FILES_H__
#define __GLIBTOP_PROC_OPEN_FILES_H__

#include <glibtop.h>
#include <glibtop/global.h>


G_BEGIN_DECLS

#define GLIBTOP_PROC_OPEN_FILES_NUMBER		0
#define GLIBTOP_PROC_OPEN_FILES_TOTAL		1
#define GLIBTOP_PROC_OPEN_FILES_SIZE		2

#define GLIBTOP_MAX_PROC_OPEN_FILES		3

#define GLIBTOP_FILE_ENTRY_FD				0
#define GLIBTOP_FILE_ENTRY_NAME				1
#define GLIBTOP_FILE_ENTRY_TYPE				2
#define GLIBTOP_FILE_ENTRY_INETSOCKET_DST_HOST		3
#define GLIBTOP_FILE_ENTRY_INETSOCKET_DST_PORT		4

#define GLIBTOP_MAX_OPEN_FILE_ENTRY		5

#define GLIBTOP_OPEN_FILENAME_LEN	215
 /* ready for IPv6 */
#define GLIBTOP_OPEN_DEST_HOST_LEN	46

enum glibtop_file_type {
	GLIBTOP_FILE_TYPE_FILE		= 1,
	GLIBTOP_FILE_TYPE_PIPE		= 2,
	GLIBTOP_FILE_TYPE_INETSOCKET	= 4,
	GLIBTOP_FILE_TYPE_LOCALSOCKET	= 8,
	GLIBTOP_FILE_TYPE_INET6SOCKET	= 16
};

typedef struct _glibtop_open_files_entry glibtop_open_files_entry;

typedef struct _glibtop_proc_open_files	glibtop_proc_open_files;

struct _glibtop_open_files_entry
{
	int fd;
	guint16 type; /* An "enum glibtop_file_type" value. */
	union {
		/* When type == GLIBTOP_FILE_TYPE_INETSOCKET or
		 * when type == GLIBTOP_FILE_TYPE_INET6SOCKET */
		struct {
			char dest_host[GLIBTOP_OPEN_DEST_HOST_LEN+1];
			int dest_port;
		} sock;

		/* When type == GLIBTOP_FILE_TYPE_FILE */
		struct {
			char name[GLIBTOP_OPEN_FILENAME_LEN+1];
		} file;

		/* When type == GLIBTOP_FILE_TYPE_LOCALSOCKET */
		struct {
			char name[GLIBTOP_OPEN_FILENAME_LEN+1];
		} localsock;

	} info;
};

struct _glibtop_proc_open_files
{
	guint64	flags;
	guint64 number;			/* GLIBTOP_PROC_OPEN_FILES_NUMBER */
	guint64 total;			/* GLIBTOP_PROC_OPEN_FILES_TOTAL  */
	guint64 size;			/* GLIBTOP_PROC_OPEN_FILES_SIZE   */
};

glibtop_open_files_entry *
glibtop_get_proc_open_files(glibtop_proc_open_files *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_FILE
#define glibtop_get_proc_open_files_r		glibtop_get_proc_open_files_p
#else
#define glibtop_get_proc_open_files_r		glibtop_get_proc_open_files_s
#endif

glibtop_open_files_entry *
glibtop_get_proc_open_files_l (glibtop *server, glibtop_proc_open_files *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_FILE
void _glibtop_init_proc_open_files_p (glibtop *server);

glibtop_open_files_entry *
glibtop_get_proc_open_files_p (glibtop *server, glibtop_proc_open_files *buf, pid_t pid);
#else
void _glibtop_init_proc_open_files_s (glibtop *server);

glibtop_open_files_entry *
glibtop_get_proc_open_files_s (glibtop *server, glibtop_proc_open_files *buf, pid_t pid);
#endif

G_END_DECLS

#endif
