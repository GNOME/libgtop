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
#include <glibtop/xmalloc.h>
#include <glibtop/procargs.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proc_args =
(1L << GLIBTOP_PROC_ARGS_SIZE);

/* Init function. */

int
glibtop_init_proc_args_s (glibtop *server)
{
    server->sysdeps.proc_args = _glibtop_sysdeps_proc_args;
}

/* Provides detailed information about a process. */

char *
glibtop_get_proc_args_s (glibtop *server, glibtop_proc_args *buf,
			 pid_t pid, unsigned max_len)
{
    char buffer [BUFSIZ];
    char *retval = NULL, *ptr;
    size_t total;
    int ret;

    memset (buf, 0, sizeof (glibtop_proc_args));

    if (max_len > BUFSIZ)
	retval = ptr = glibtop_malloc_r (server, max_len+1);
    else
	ptr = buffer;

    if (!max_len)
	max_len = BUFSIZ;

    ret = glibtop_get_proc_data_proc_args_s (server, pid, ptr, max_len);
    if (ret < 0) {
	if (!retval) glibtop_free_r (server, retval);
	return NULL;
    }

    total = ret;

    if (retval) {
	retval = glibtop_realloc_r (server, retval, total+1);
    } else {
	retval = glibtop_malloc_r (server, total+1);
	memcpy (retval, buffer, total);
    }

    retval [total] = 0;

    buf->size = total;
    buf->flags = _glibtop_sysdeps_proc_args;

    return retval;
}
