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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef __GLIBTOP_GLOBAL_H__
#define __GLIBTOP_GLOBAL_H__

#include <glib.h>

#ifdef _IN_LIBGTOP

#endif /* _IN_LIBGTOP */

#if HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdarg.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#include <string.h>

#ifdef _IN_LIBGTOP

#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>

#endif /* _IN_LIBGTOP */

#include <signal.h>
#include <errno.h>

#ifdef _IN_LIBGTOP

G_BEGIN_DECLS

#ifndef HAVE_STRERROR
char *strerror (int errno);
#endif

G_END_DECLS

#endif /* _IN_LIBGTOP */

#endif
