/* Copyright (C) 2004 Benoît Dejean
   This file is part of LibGTop 2.0.

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

#ifndef __CYGWIN__GLIBTOP_PRIVATE_H__
#define __CYGWIN__GLIBTOP_PRIVATE_H__

#include <glibtop.h>
#include <glibtop/error.h>

#include <glib.h>

#include <fcntl.h>
#include <ctype.h>
#include <string.h>

G_BEGIN_DECLS


static inline char*
next_token(const char *p)
{
	while (isspace(*p)) p++;
	return (char*) p;
}

char *
skip_token (const char *p);

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

/*
 * Smart strtoul which handles binary suffixes
 * e.g: get_scaled("Size: 32 kB", "Size:") == 32768
 * key can be NULL if there's not prefix to strip (or prefix size is known
 */
unsigned long long
get_scaled(const char *buffer, const char *key);


/* aborts on error */
void
file_to_buffer(glibtop *server, char *buffer, size_t bufsiz, const char *filename);

/* return < 0 on error, otherwise 0 on success */
int
try_file_to_buffer(char *buffer, size_t bufsiz, const char *format, ...) G_GNUC_PRINTF(3, 4);


/* some inline functions that wrap proc path
 * as fast as macros :)
 */

static inline int
proc_file_to_buffer (char *buffer, size_t bufsiz, const char *fmt, pid_t pid)
{
	return try_file_to_buffer(buffer, bufsiz, fmt, pid);
}

static inline int
proc_stat_to_buffer (char *buffer, size_t bufsiz, pid_t pid)
{
	return proc_file_to_buffer(buffer, bufsiz, "/proc/%d/stat", pid);
}

static inline int
proc_status_to_buffer (char *buffer, size_t bufsiz, pid_t pid)
{
	return proc_file_to_buffer(buffer, bufsiz, "/proc/%d/status", pid);
}

static inline int
proc_statm_to_buffer (char *buffer, size_t bufsiz, pid_t pid)
{
	return proc_file_to_buffer(buffer, bufsiz, "/proc/%d/statm", pid);
}


static inline char *
proc_stat_after_cmd (char *p)
{
	p = strrchr (p, ')');
	if (G_LIKELY(p))
		*p++ = '\0';
	return p;
}


unsigned long
get_boot_time(glibtop *server);


size_t
get_page_size(void);


gboolean
check_cpu_line(glibtop *server, const char *line, unsigned n);


static inline gboolean
check_cpu_line_warn(glibtop *server, const char *line, unsigned i)
{
	gboolean ret;

	ret = check_cpu_line(server, line, i);

	if (G_UNLIKELY(!ret))
		glibtop_warn_io_r(server,
				  "'%s' does not start with 'cpu%u'",
				  line, i);

	return ret;
}


gboolean safe_readlink(const char *path, char *buf, size_t bufsiz);


G_END_DECLS

#endif /* __CYGWIN__GLIBTOP_PRIVATE_H__ */
