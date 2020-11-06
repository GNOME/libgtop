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
#include <glibtop/error.h>
#include <glibtop/disk.h>

#include "glibtop_private.h"

static const unsigned long _glibtop_sysdeps_disk =
(1L << GLIBTOP_XDISK_SECTORS_READ) + (1L << GLIBTOP_XDISK_TIME_READ) +
(1L << GLIBTOP_XDISK_SECTORS_WRITE) + (1L << GLIBTOP_XDISK_TIME_WRITE);

/* Init function. */

void
_glibtop_init_disk_s (glibtop *server)
{
	server->sysdeps.disk = _glibtop_sysdeps_disk;
}

/* Provides information about disk usage. */

#define FILENAME	"/proc/diskstats" //kernel reports sectors by 512 bytes even for AF 4kn
#define STAT_BUFSIZ     81920

void
glibtop_get_disk_s (glibtop *server, glibtop_disk *buf)
{
	char buffer [STAT_BUFSIZ], *p;
	int i;

	memset (buf, 0, sizeof (glibtop_disk));

	file_to_buffer(server, buffer, sizeof buffer, FILENAME);

	/*
	 * GLOBAL
	 */

	p = buffer;	/* "disk" */

	/*
	 * PER DISK
	 */

	for (i = 0; i <= server->ndisk; i++) {

        p = skip_multiple_token(p,2);

        // skip if disk is the raw device
        if(!check_alphanumeric_word(p)){

		    p = skip_line(p); /* move to ^ */
            p = skip_multiple_token(p,2);

        }

		if (!check_disk_line_warn(server, p, i))
			break;

        p = skip_token(p); /* prev xdisk_name */
        p = skip_token(p); /* prev xdisk_reads_completed */
        p = skip_token(p); /* prev xdisk_reads_merged */

		buf->xdisk_sectors_read [i] = strtoull (p, &p, 0);
		buf->xdisk_time_read [i] = strtoull (p, &p, 0);

        p = skip_token(p); /* prev xdisk_writes_completed */
        p = skip_token(p); /* prev xdisk_writes_merged */

		buf->xdisk_sectors_write  [i] = strtoull (p, &p, 0);
		buf->xdisk_time_write [i] = strtoull (p, &p, 0);

        p = skip_line(p); /* move to ^ */

	}
}
