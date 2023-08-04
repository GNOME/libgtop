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

// Linux kernel reports sectors by 512 bytes even for AF 4kn //

#define FILENAME	"/proc/diskstats" 
#define CMD_PIPE    "lsblk --output NAME,TYPE -i -n | sed 's/`-//'|sed 's/|-//'|sed 's/|//'| sed -e 's/^[ \t]*//'|tr -s ' '"
#define STAT_BUFSIZ     81920

// Handle LVM and RAID //

static void
find_primary_part (partition_info *primary_part, const char *m)
{
	int n = 0, tlvl = 0;
	char name[256]="", type[256]="";

	primary_part->max = 0;

	//scan by tree level
	//0 = disk (to lvl 0)
	//0 = disk, 1 = part (to lvl 1)
	//0 = disk, 1 = part, 2 = lvm or raid (to lvl 2)
	//0 = disk, 1 = part, 2 = raid, 3 = lvm (to lvl 3)

	while (sscanf(m, "%s %s", name, type) == 2)
	{

		if (tlvl == 0) {

			if (strcmp (type, "loop") == 0 || strcmp (type, "rom") == 0 || strcmp (type, "disk") == 0) {

				primary_part->max++;

			}
			else if ((strcmp (type, "part") == 0)){

				tlvl = 1;

			}

		}
		else if(tlvl == 1){

			if (strcmp (type, "loop") == 0 || strcmp (type, "rom") == 0 || strcmp (type, "disk") == 0) {

				n--;
				tlvl = 0;
				primary_part->max++;

			}
			else if ((strcmp (type, "part") == 0)) {

				n--;

			}
			else if ((strcmp (type, "lvm") == 0) || (strncmp (type, "raid", 4) == 0)) {

				tlvl = 2;
				primary_part->max++;

			}

		}
		else if( tlvl == 2){

			if (strcmp (type, "loop") == 0 || strcmp (type, "rom") == 0 || strcmp(type, "disk") == 0) {

				n--;
				tlvl = 0;
				primary_part->max++;

			}
			else if (((strcmp (primary_part[n-1].type, "lvm") == 0) && (strcmp (type, "lvm") == 0)) ||
				((strcmp (primary_part[n-1].type, "raid") == 0) && (strncmp (type, "raid", 4) == 0))) {

				n--;

			}
			else if (((strcmp (primary_part[n-1].type, "lvm") == 0) && (strcmp (type, "part") == 0)) ||
				((strcmp (primary_part[n-1].type, "raid") == 0) && (strcmp (type, "part") == 0))) {

				n--;
				tlvl = 1;

			}
			else if ((strcmp (primary_part[n-1].type, "raid") == 0) && (strcmp (type, "lvm") == 0)){
				
				tlvl = 3;
				primary_part->max++;

			}

		}
		else if (tlvl == 3) {

			if (strcmp (type, "loop") == 0 || strcmp (type, "rom") == 0 || strcmp (type, "disk") == 0) {

				n--;
				tlvl = 0;
				primary_part->max++;

			}
			else if ((strcmp (type, "lvm") == 0)) {

				n--;

			}
			else if ((strncmp (type, "raid", 4) == 0)) {

				n--;
				tlvl = 2;

			}
			else if ((strcmp (type, "part") == 0)) {

				n--;
				tlvl = 1;

			}

		}


		strcpy (primary_part[n].name, name);
		strncpy (primary_part[n].type, type, 4);
		
		if (strcmp (primary_part[n].type, "raid") == 0) {

			strcpy (primary_part[n].raid_num, type + 4);

		}

		m = skip_line (m);
		n++;

	}
}

static int
is_virtual_drive (partition_info *primary_part, const char *p)
{
	int i;
	char name[256];
	int test = 1;

	sscanf (p, "%s", name);

	if (*p) {

		for (i=0; i < primary_part->max; i++) {

			if (strcmp (primary_part[i].name, name) == 0) {

				test = 0;
				break;

			}

		}

	}
	else {

		test = 0;

	}

	return test;
}

static int
max_lines (const char *p)
{
	char temp[10];
	int count = 0;

	while (sscanf (p, "%s", temp) == 1)
	{

		p = skip_line(p);
		count++;

	}

	return count;
}

void
glibtop_get_disk_s (glibtop *server, glibtop_disk *buf)
{
	partition_info primary_part[GLIBTOP_NDISK];
	char buffer [STAT_BUFSIZ], *p, map_buffer [STAT_BUFSIZ], *m;
	int i;

	memset (buf, 0, sizeof (glibtop_disk));

	file_to_buffer (server, buffer, sizeof buffer, FILENAME);

	get_from_pipe (map_buffer, STAT_BUFSIZ, CMD_PIPE);

	/*
	 * GLOBAL
	 */

	p = buffer;	/* "disk" */
	m = map_buffer;

	/*
	 * PER DISK
	 */

	server->ndisk = max_lines(p);

	find_primary_part (primary_part, m);

	for (i = 0; i < server->ndisk; i++) {

		p = skip_multiple_token (p,2);

		// skip if disk is the raw device
		if (!is_virtual_drive (primary_part, p)) {

			server->ndisk--;
			p = skip_line (p); /* move to ^ */
			p = skip_multiple_token (p, 2);

		}

		p = skip_token (p); /* prev xdisk_name */
		p = skip_token (p); /* prev xdisk_reads_completed */
		p = skip_token (p); /* prev xdisk_reads_merged */

		buf->xdisk_sectors_read [i] = strtoull (p, &p, 0);
		buf->xdisk_time_read [i] = strtoull (p, &p, 0);

		p = skip_token (p); /* prev xdisk_writes_completed */
		p = skip_token (p); /* prev xdisk_writes_merged */

		buf->xdisk_sectors_write  [i] = strtoull (p, &p, 0);
		buf->xdisk_time_write [i] = strtoull (p, &p, 0);

		p = skip_line (p); /* move to ^ */

	}
}
