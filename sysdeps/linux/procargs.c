/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/xmalloc.h>
#include <glibtop/procargs.h>

static const unsigned long _glibtop_sysdeps_proc_args =
(1 << GLIBTOP_PROC_ARGS_SIZE);

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
		retval = glibtop_malloc_r (server, max_len+1);

		len = read (cmdline, retval, max_len);
		if (len < 0) {
			glibtop_free_r (server, retval);
			return NULL;
		}
		
		return retval;
	}

	while (1) {
		len = read (cmdline, buffer, BUFSIZ-1);
		if (len < 0) {
			glibtop_free_r (server, retval);
			return NULL;
		}

		if (len == 0)
			break;

		retval = glibtop_realloc_r (server, retval, total+len+1);
		memcpy (retval+total, buffer, len);
		*(retval+total+len) = 0;
		total += len;
	}
	
	close (cmdline);

	buf->size = total;
	buf->flags = _glibtop_sysdeps_proc_args;
	
	return retval;
}
