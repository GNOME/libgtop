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
#include <gdbm.h>

#ifndef SYSTEM_INODEDB
#define SYSTEM_INODEDB "/usr/var/libgtop/inodedb.db"
#endif

glibtop_inodedb *
glibtop_inodedb_open_s (glibtop *server, unsigned databases,
			unsigned long cachesize)
{
	glibtop_inodedb *inodedb;
	char filename [BUFSIZ];
	struct passwd *pwd;
	struct stat statb;

	if (!databases)
		databases = GLIBTOP_INODEDB_ALL;

	inodedb = g_malloc (sizeof (glibtop_inodedb));

	if (stat (SYSTEM_INODEDB, &statb))
		databases &= ~GLIBTOP_INODEDB_SYSTEM;

	if (databases & GLIBTOP_INODEDB_SYSTEM) {
		inodedb->system_dbf = gdbm_open
			(SYSTEM_INODEDB, 0, GDBM_READER, 0, 0);
		if (!inodedb->system_dbf)
			glibtop_error_io_r
				(server, "gdbm_open (%s)", SYSTEM_INODEDB);
	}

	pwd = getpwuid (getuid ());
	if (!pwd) glibtop_error_io_r (server, "getpwuid");
	
	sprintf (filename, "%s/var/libgtop/inodedb.db", pwd->pw_dir);
	
	if (stat (filename, &statb))
		databases &= ~GLIBTOP_INODEDB_USER;

	if (databases & GLIBTOP_INODEDB_USER) {
		inodedb->user_dbf = gdbm_open
			(filename, 0, GDBM_READER, 0, 0);
		if (!inodedb->user_dbf)
			glibtop_error_io_r
				(server, "gdbm_open (%s)", filename);
	}

	return inodedb;
}

const char *
glibtop_inodedb_lookup_s (glibtop *server,
			  glibtop_inodedb *inodedb,
			  guint64 device, guint64 inode)
{
	glibtop_inodedb_key key;
	datum d_key, d_content;

	d_key.dptr = (void *) &key;
	d_key.dsize = sizeof (key);

	key.device = device;
	key.inode = inode;

	if (inodedb->system_dbf) {
		d_content = gdbm_fetch (inodedb->system_dbf, d_key);
		if (d_content.dptr) return d_content.dptr;
	}

	if (inodedb->user_dbf) {
		d_content = gdbm_fetch (inodedb->user_dbf, d_key);
		if (d_content.dptr) return d_content.dptr;
	}

	return NULL;
}

void
glibtop_inodedb_close_s (glibtop *server, glibtop_inodedb *inodedb)
{
	if (inodedb->system_dbf)
		gdbm_close (inodedb->system_dbf);
	
	if (inodedb->user_dbf)
		gdbm_close (inodedb->user_dbf);

	g_free (server, inodedb);
}
