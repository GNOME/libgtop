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
#include <glibtop/inodedb.h>

#include <pwd.h>
#include <dirent.h>

#include <sys/stat.h>

int
main (int argc, const char *argv [])
{
    glibtop_inodedb *inodedb;
    const char *filename;
    unsigned device, inode;

    if (argc != 3) {
	fprintf (stderr, "Usage: %s device inode\n", argv [0]);
	exit (1);
    }

    if (sscanf (argv [1], "%d", &device) != 1) {
	fprintf (stderr, "Usage: %s device inode\n", argv [0]);
	exit (1);
    }

    if (sscanf (argv [2], "%d", &inode) != 1) {
	fprintf (stderr, "Usage: %s device inode\n", argv [0]);
	exit (1);
    }

    inodedb = glibtop_inodedb_open (0, 0);
    if (!inodedb) exit (1);

    filename = glibtop_inodedb_lookup (inodedb, device, inode);
    if (!filename) exit (2);

    fprintf (stderr, "FILENAME: %d - %d - '%s'\n",
	     (int) device, (int) inode, filename);

    exit (0);
}
