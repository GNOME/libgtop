/* Copyright (C) 2017 Robert Roth
   This file is part of LibGTop.

   Contributed by Robert Roth <robert.roth.off@gmail.com>, February 2017.

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

#ifndef __GLIBTOP_PROCDISKIO_H__
#define __GLIBTOP_PROCDISKIO_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_PROC_DISKIO_RCHAR	0
#define GLIBTOP_PROC_DISKIO_WCHAR	1
#define GLIBTOP_PROC_DISKIO_RBYTES	2
#define GLIBTOP_PROC_DISKIO_WBYTES	3

#define GLIBTOP_MAX_PROC_TIME		3

typedef struct _glibtop_proc_diskio	glibtop_proc_diskio;

/* Time section */

/**
 * glibtop_proc_diskio:
 * @rchar: The number of bytes which this task has caused to be read from storage. This
 * is simply the sum of bytes which this process passed to read() and pread(), also including tty IO,
 * and it is unaffected by whether or not actual physical disk IO was required.
 * @wchar: The number of bytes which this task has caused, or shall cause to be written
 * to disk. Similar caveats apply here as with rchar.
 * @rbytes: Attempt to count the number of bytes which this process really did cause to
 * be fetched from the storage layer. Done at the submit_bio() level, so it is
 * accurate for block-backed filesystems.
 * @wbytes: Attempt to count the number of bytes which this process caused to be sent to
 * the storage layer. This is done at page-dirtying time.
 *
 * Process disk io data filled by glibtop_get_proc_diskio().
 *
 */
struct _glibtop_proc_diskio
{
    /*< private >*/
	guint64	flags;
	/*< public >*/
	guint64 rchar;
	guint64 wchar;
	guint64 rbytes;
	guint64 wbytes;
};


void glibtop_get_proc_diskio (glibtop_proc_diskio *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_DISKIO
#define glibtop_get_proc_diskio_r	glibtop_get_proc_diskio_p
#else
#define glibtop_get_proc_diskio_r	glibtop_get_proc_diskio_s
#endif

void glibtop_get_proc_diskio_l (glibtop *server, glibtop_proc_diskio *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_DISKIO
void _glibtop_init_proc_diskio_p (glibtop *server);
void glibtop_get_proc_diskio_p (glibtop *server, glibtop_proc_diskio *buf, pid_t pid);
#else
void _glibtop_init_proc_diskio_s (glibtop *server);
void glibtop_get_proc_diskio_s (glibtop *server, glibtop_proc_diskio *buf, pid_t pid);
#endif


G_END_DECLS

#endif
