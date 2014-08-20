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
#include <procinfo.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procargs.h>

#include "utils.h"

static const unsigned long _glibtop_sysdeps_proc_args =
	(1 << GLIBTOP_PROC_ARGS_SIZE);

/* Init function. */

void
_glibtop_init_proc_args_s (glibtop *server)
{
	server->sysdeps.proc_args = _glibtop_sysdeps_proc_args;
}

/* Provides detailed information about a process. */

static void _glibtop_remove_non_ascii(char* string)
{
	for (;*string != 0;string++)
	{
		if (!isascii(*string))
			*string = '?';
	}
}

char *
glibtop_get_proc_args_s (glibtop *server, glibtop_proc_args *buf,
			 pid_t pid, unsigned max_len)
{
	struct procsinfo *pinfo;
	char *args_buffer;
	char* args = NULL;
	int size;
	int result;
	int len;

	glibtop_init_s (&server, (1L << GLIBTOP_SYSDEPS_PROC_ARGS), 0);

	memset (buf, 0, sizeof (glibtop_proc_args));

	pinfo = _glibtop_get_procinfo(server, pid);
	if (pinfo == NULL) return NULL;

	size = max_len != 0 ? max_len : 4096;
	args_buffer = g_malloc (size);

	result = getargs(pinfo, sizeof(struct procsinfo), args_buffer, size);
	if (result == -1)
	{
		glibtop_error_io_r (server, "Cannot malloc getargs");

		g_free(args_buffer);

		return NULL;
	}

	/* look if empty string */

	if (args_buffer[0] == 0)
	{
		g_free(args_buffer);

		return NULL;
	}

	/* compute length of args and realloc */

	len = 0;
	while ((args_buffer[len] != 0) && (len < size))
	{
		_glibtop_remove_non_ascii(args_buffer + len);

		len += strlen(args_buffer + len) + 1;
	}

	args = g_malloc (len);

	memcpy(args, args_buffer, len);
	g_free(args_buffer);

	buf->size = len - 1;

	buf->flags = _glibtop_sysdeps_proc_args;

	return args;
}
