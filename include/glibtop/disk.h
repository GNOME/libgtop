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

#ifndef __GLIBTOP_DISK_H__
#define __GLIBTOP_DISK_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_XDISK_SECTORS_READ	0
#define GLIBTOP_XDISK_TIME_READ		1
#define GLIBTOP_XDISK_SECTORS_WRITE	2
#define GLIBTOP_XDISK_TIME_WRITE		3

#define GLIBTOP_MAX_DISK				4

/* Nobody should really be using more than 4 disk.
   Yes we are :)
   Nobody should really be using more than 32 disk.
*/
#define GLIBTOP_NDISK					1024

typedef struct _glibtop_disk	glibtop_disk;

struct _partition_info
{
	char name[256];
	char type[256];
	char raid_num[256];
	int max;
};

typedef struct _partition_info partition_info;

struct _glibtop_disk
{
	guint64 flags;														/* NOT USED YET		*/
	guint64 xdisk_sectors_read [GLIBTOP_NDISK];				/* GLIBTOP_XDISK_SECTORS_READ		*/
	guint64 xdisk_time_read [GLIBTOP_NDISK];					/* GLIBTOP_XDISK_TIME_READ		*/
	guint64 xdisk_sectors_write [GLIBTOP_NDISK];				/* GLIBTOP_XDISK_SECTORS_WRITE		*/
	guint64 xdisk_time_write [GLIBTOP_NDISK];					/* GLIBTOP_XDISK_TIME_WRITE		*/
	guint64 xdisk_flags;
};

void glibtop_get_disk (glibtop_disk *buf);

#if GLIBTOP_SUID_DISK
#define glibtop_get_disk_r	glibtop_get_disk_p
#else
#define glibtop_get_disk_r	glibtop_get_disk_s
#endif

void glibtop_get_disk_l (glibtop *server, glibtop_disk *buf);

#if GLIBTOP_SUID_DISK
void _glibtop_init_disk_p (glibtop *server);
void glibtop_get_disk_p (glibtop *server, glibtop_disk *buf);
#else
void _glibtop_init_disk_s (glibtop *server);
void glibtop_get_disk_s (glibtop *server, glibtop_disk *buf);
#endif


G_END_DECLS

#endif
