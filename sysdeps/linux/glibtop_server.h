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

#ifndef __GLIBTOP_SERVER_H__
#define __GLIBTOP_SERVER_H__

#include <fcntl.h>
#include <ctype.h>

G_BEGIN_DECLS

#ifdef _IN_LIBGTOP

#define LINUX_VERSION(x,y,z)   (0x10000*(x) + 0x100*(y) + z)

static inline char *
skip_token (const char *p)
{
        while (isspace(*p)) p++;
        while (*p && !isspace(*p)) p++;
	return (char *)p;
}

static inline char *
skip_multiple_token (const char *p, size_t count)
{
	while(count--)
		p = skip_token (p);

	return (char *)p;
}

static inline char *
skip_line (const char *p)
{
	while (*p && *p != '\n') p++;
	return (char *) (*p ? p+1 : p);
}

static inline unsigned long long
get_scaled(const char *buffer, const char *key)
{
  const char    *ptr;
  char          *next;
  const size_t len = strlen(key);
  unsigned long long value = 0;

  if ((ptr = strstr(buffer, key)))
    {
      ptr += len;
      value = strtoull(ptr, &next, 0);
      if (strchr(next, 'k'))
	value *= 1024;
      else if (strchr(next, 'M'))
	value *= 1024 * 1024;
    }
  return value;
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
	close (fd);

	if (len < 0)
	    return -1;

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
#define GLIBTOP_SUID_PROC_ARGS		0
#define GLIBTOP_SUID_PROC_MAP		0
#define GLIBTOP_SUID_NETLOAD		0
#define GLIBTOP_SUID_PPP		0

G_END_DECLS

#endif
