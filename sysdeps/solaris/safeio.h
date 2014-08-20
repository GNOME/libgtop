/* Copyright (C) 1999 Drazen Kacar
   This file is part of LibGTop 1.0.

   Contributed by Drazen Kacar <dave@srce.hr>, May 1999.

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

#ifndef __GLIBTOP_SAFEIO_H__
#define __GLIBTOP_SAFEIO_H__

#include <glib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

G_BEGIN_DECLS

int
s_open(const char *, int);

int
s_stat(const char *, struct stat *);

int
s_close(int);

ssize_t
s_pread(int, void *, size_t, off_t);

int
s_closedir(DIR *);

G_END_DECLS

#endif /* __GLIBTOP_SAFEIO_H__ */
