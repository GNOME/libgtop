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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procargs.h>

static const unsigned long _glibtop_sysdeps_proc_args =
(1L << GLIBTOP_PROC_ARGS_SIZE);

/* Init function. */

void
glibtop_init_proc_args_s (glibtop *server)
{
	server->sysdeps.proc_args = _glibtop_sysdeps_proc_args;
}

/* Provides detailed information about a process. */

char *
glibtop_get_proc_args_s (glibtop *server, glibtop_proc_args *buf,
			 pid_t pid, unsigned max_len)
{
	char fn [BUFSIZ], buffer [BUFSIZ];
	int cmdline, len, total = 0;
	char *retval = NULL;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_ARGS, 0);

	memset (buf, 0, sizeof (glibtop_proc_args));

	sprintf (fn, "/proc/%d/cmdline", pid);

	cmdline = open (fn, O_RDONLY);
	if (cmdline < 0) return NULL;

	if (max_len) {
		retval = g_malloc (max_len+1);

		len = read (cmdline, retval, max_len);
		close (cmdline);

		if (len < 0) {
			g_free (retval);
			return NULL;
		}

		*(retval+len) = 0;

		buf->size = len;
		buf->flags = _glibtop_sysdeps_proc_args;

		return retval;
	}

	while (1) {
		len = read (cmdline, buffer, BUFSIZ-1);
		if (len < 0) {
			close (cmdline);
			g_free (retval);
			return NULL;
		}

		if (len == 0)
			break;

		retval = g_realloc (retval, total+len+1);
		memcpy (retval+total, buffer, len);
		*(retval+total+len) = 0;
		total += len;
	}

	close (cmdline);

	buf->size = total;
	buf->flags = _glibtop_sysdeps_proc_args;

	return retval;
}
