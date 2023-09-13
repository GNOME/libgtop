/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by James Dominic P. Guana <guana.histark@gmail.com>, May 2020.

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

#include <locale.h>
#include <libintl.h>
#include <math.h>
#include <stdio.h>

#include <glibtop.h>
#include <glibtop/disk.h>

int
main (int argc, char *argv [])
{
	glibtop_disk disk;
	char separator [BUFSIZ];
	int ndisk, i;

	glibtop_init();

	glibtop_get_disk (&disk);

	ndisk = glibtop_global_server->ndisk ? glibtop_global_server->ndisk : 1;

	memset (separator, '-', 91);
	separator [92] = '\0';

	printf("\n\n");
	printf ("ELAPSE                 ");
	printf ("Read (b)  Time Read (ms)       Write (b) Time Write (ms)\n");
	printf ("%s\n", separator);

	for (i = 0; i < ndisk; i++) {
		printf ("DISK %3d      : %15lu %15lu %15lu %15lu\n", i,
			(unsigned long) disk.xdisk_sectors_read [i] * 512,
			(unsigned long) disk.xdisk_time_read [i],
			(unsigned long) disk.xdisk_sectors_write  [i] * 512,
			(unsigned long) disk.xdisk_time_write   [i]);

	}

	printf ("%s\n\n\n", separator);

	exit (0);
}
