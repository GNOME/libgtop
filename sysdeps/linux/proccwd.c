/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

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
#include <glibtop/proccwd.h>

static const unsigned long _glibtop_sysdeps_proc_cwd =
(1L << GLIBTOP_PROC_CWD_SIZE);

static const unsigned long _glibtop_sysdeps_proc_cwd_stat =
(1L << GLIBTOP_PROC_CWD_DEVICE) + (1L << GLIBTOP_PROC_CWD_INODE);

/* Init function. */

int
glibtop_init_proc_cwd_s (glibtop *server)
{
    server->sysdeps.proc_cwd = _glibtop_sysdeps_proc_cwd |
	_glibtop_sysdeps_proc_cwd_stat;

    return 0;
}

/* Provides detailed information about a process. */

char *
glibtop_get_proc_cwd_s (glibtop *server, glibtop_proc_cwd *buf, pid_t pid)
{
    char fn [BUFSIZ], buffer [BUFSIZ], *retval = NULL;
    struct stat statb;
    int len;

    glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_CWD, 0);
	
    memset (buf, 0, sizeof (glibtop_proc_cwd));

    sprintf (fn, "/proc/%d/cwd", pid);
	
    len = readlink (fn, buffer, BUFSIZ-1);

    if (len < 0) {
	glibtop_warn_io_r (server, "readlink (%s)", fn);
	return NULL;
    }

    buffer [len] = '\0';

    retval = glibtop_strdup_r (server, buffer);

    buf->size = len+1;
    buf->flags = _glibtop_sysdeps_proc_cwd;

    if (lstat (fn, &statb)) {
	glibtop_warn_io_r (server, "lstat (%s)", fn);
	return retval;
    }

    buf->device = statb.st_dev;
    buf->inode = statb.st_ino;

    buf->flags |= _glibtop_sysdeps_proc_cwd_stat;

    return retval;
}
