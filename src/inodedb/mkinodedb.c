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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/inodedb.h>

#include <pwd.h>
#include <dirent.h>

#include <sys/stat.h>

int
main (int argc, char *argv [])
{
	GDBM_FILE dbf;
	char dirname [BUFSIZ];
	FILE *f;

	if (argc != 3) {
		fprintf (stderr, "Usage: %s database filename\n", argv [0]);
		exit (1);
	}

	f = fopen (argv [2], "rt");
	if (!f)
		glibtop_error_io ("fopen (%s)", argv [2]);

	dbf = gdbm_open (argv [1], 512, GDBM_WRCREAT, 0600, 0);
	if (!dbf)
		glibtop_error_io ("gdbm_open (%s)", argv [1]);

	while (fgets (dirname, BUFSIZ-1, f)) {
		struct dirent *entry;
		struct stat statb;
		DIR *directory;
		size_t len;

		len = strlen (dirname);
		if (!len) continue;

		if (dirname [len-1] == '\n')
			dirname [len-1] = 0;
		
		if (stat (dirname, &statb))
			continue;

		if (S_ISREG (statb.st_mode)) {
			glibtop_inodedb_key key;
			datum d_key, d_content;
			
			d_key.dptr = (void *) &key;
			d_key.dsize = sizeof (key);

			d_content.dptr = dirname;
			d_content.dsize = strlen (dirname) + 1;
			
			key.device = (u_int64_t) statb.st_dev;
			key.inode = (u_int64_t) statb.st_ino;
			
			if (gdbm_store (dbf, d_key, d_content, GDBM_REPLACE))
				glibtop_error_io ("gdbm_store (%s)", dirname);

			printf ("%-52s - %8lu - %8lu\n",
				dirname, (unsigned long) statb.st_dev,
				(unsigned long) statb.st_ino);

			continue;
		}

		if (!S_ISDIR (statb.st_mode))
			continue;

		directory = opendir (dirname);
		if (!directory) continue;

		while ((entry = readdir (directory))) {
			glibtop_inodedb_key key;
			char filename [BUFSIZ];
			datum d_key, d_content;
			
			sprintf (filename, "%s/%s", dirname, entry->d_name);
			
			if (stat (filename, &statb))
				continue;

			if (!S_ISREG (statb.st_mode))
				continue;

			d_key.dptr = (void *) &key;
			d_key.dsize = sizeof (key);

			d_content.dptr = filename;
			d_content.dsize = strlen (filename) + 1;
			
			key.device = (u_int64_t) statb.st_dev;
			key.inode = (u_int64_t) statb.st_ino;
			
			if (gdbm_store (dbf, d_key, d_content, GDBM_REPLACE))
				glibtop_error_io ("gdbm_store (%s)", filename);

			printf ("%-52s - %8lu - %8lu\n",
				filename, (unsigned long) statb.st_dev,
				(unsigned long) statb.st_ino);
		}
		
		closedir (directory);
	}

	gdbm_close (dbf);

	fclose (f);

	exit (0);
}
