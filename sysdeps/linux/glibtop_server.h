/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef __GLIBTOP_SERVER_H__
#define __GLIBTOP_SERVER_H__

#include <fcntl.h>
#include <ctype.h>

__BEGIN_DECLS

#ifdef _IN_LIBGTOP

static inline char *
skip_token (const char *p)
{
        while (isspace(*p)) p++;
        while (*p && !isspace(*p)) p++;
	return (char *)p;
}

static inline char *
skip_multiple_token (const char *p, int count)
{
	int i;

	for (i = 0; i < count; i++)
		p = skip_token (p);

	return (char *)p;
}

static inline char *
skip_line (const char *p)
{
	while (*p != '\n') p++;
	return (char *) ++p;
}

static inline int
proc_file_to_buffer (char *buffer, const char *fmt, pid_t pid)
{
	char filename [BUFSIZ];
	int fd, len;

	sprintf (filename, fmt, pid);

	fd = open (filename, O_RDONLY);
	if (fd < 0) return -1;

	len = read (fd, buffer, BUFSIZ-1);
	if (len < 0) return -1;

	close (fd);

	buffer [len] = '\0';

	return 0;
}

static inline int
proc_stat_to_buffer (char *buffer, pid_t pid)
{
	return proc_file_to_buffer (buffer, "/proc/%d/stat", pid);
}

static inline int
proc_status_to_buffer (char *buffer, pid_t pid)
{
	return proc_file_to_buffer (buffer, "/proc/%d/status", pid);
}

static inline int
proc_statm_to_buffer (char *buffer, pid_t pid)
{
	return proc_file_to_buffer (buffer, "/proc/%d/statm", pid);
}

static inline char *
proc_stat_after_cmd (char *p)
{
	p = strrchr (p, ')');
	if (!p) return p;

	*p++ = '\0';
	return p;
}

#endif

#define GLIBTOP_SUID_CPU		0
#define GLIBTOP_SUID_MEM		0
#define GLIBTOP_SUID_SWAP		0
#define GLIBTOP_SUID_UPTIME		0
#define GLIBTOP_SUID_LOADAVG		0
#define GLIBTOP_SUID_SHM_LIMITS		0
#define GLIBTOP_SUID_MSG_LIMITS		0
#define GLIBTOP_SUID_SEM_LIMITS		0
#define GLIBTOP_SUID_PROCLIST		0
#define GLIBTOP_SUID_PROC_STATE		0
#define GLIBTOP_SUID_PROC_UID		0
#define GLIBTOP_SUID_PROC_MEM		0
#define GLIBTOP_SUID_PROC_TIME		0
#define GLIBTOP_SUID_PROC_SIGNAL	0
#define GLIBTOP_SUID_PROC_KERNEL	0
#define GLIBTOP_SUID_PROC_SEGMENT	0
#define GLIBTOP_SUID_PROC_MAP		0

__END_DECLS

#endif
