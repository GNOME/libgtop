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

#include <config.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#include "safeio.h"

int
s_open(const char *file, int mode)
{
   int fd;

   do {
      fd = open(file, mode);
   } while(fd < 0 && errno == EINTR);
   return fd;
}

int
s_stat(const char *path, struct stat *buf)
{
   int status;

   do {
      status = stat(path, buf);
   } while(status < 0 && errno == EINTR);
   return status;
}

int
s_close(int fd)
{
   int status;

   do {
      status = close(fd);
   } while(status < 0 && errno == EINTR);
   return status;
}

ssize_t
s_pread(int fd, void *buf, size_t nbytes, off_t offset)
{
   ssize_t len;

   /* Now, why doesn't the pread(2) man page say anything about pread()
      return values? Can it read less bytes than requested? */

   do {
      len = pread(fd, buf, nbytes, offset);
   } while(len < 0 && errno == EINTR);
   return len;
}

int s_closedir(DIR *dirp)
{
   int status;

   do {
      status = closedir(dirp);
   } while(status < 0 && errno == EINTR);
   return status;
}
