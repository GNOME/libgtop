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

#ifndef __GLIBTOP_ERROR_H__
#define __GLIBTOP_ERROR_H__

#include <glibtop.h>

G_BEGIN_DECLS

void glibtop_error_vr (glibtop *server, char *format, va_list args);
void glibtop_warn_vr (glibtop *server, char *format, va_list args);

void glibtop_error_io_vr (glibtop *server, char *format, int, va_list args);
void glibtop_warn_io_vr (glibtop *server, char *format, int, va_list args);

void G_GNUC_UNUSED glibtop_error_r (glibtop *server, char *format, ...);
void G_GNUC_UNUSED glibtop_warn_r (glibtop *server, char *format, ...);
void G_GNUC_UNUSED glibtop_error_io_r (glibtop *server, char *format, ...);
void G_GNUC_UNUSED glibtop_warn_io_r (glibtop *server, char *format, ...);


#ifdef  __GNUC__

#define glibtop_error(p1, args...)	glibtop_error_r(glibtop_global_server , p1 , ## args)
#define glibtop_warn(p1, args...)	glibtop_warn_r(glibtop_global_server , p1 , ## args)

#define glibtop_error_io(p1, args...)	glibtop_error_io_r(glibtop_global_server , p1 , ## args)
#define glibtop_warn_io(p1, args...)	glibtop_warn_io_r(glibtop_global_server , p1 , ## args)

#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L

#define glibtop_error(...)	glibtop_error_r(glibtop_global_server , __VA_ARGS__)
#define glibtop_warn(...)	glibtop_warn_r(glibtop_global_server , __VA_ARGS__)

#define glibtop_error_io(...)	glibtop_error_io_r(glibtop_global_server , __VA_ARGS__)
#define glibtop_warn_io(...)	glibtop_warn_io_r(glibtop_global_server , __VA_ARGS__)

#else /* no __GNUC__, no C99*/

void glibtop_error (char *format, ...);
void glibtop_warn (char *format, ...);
void glibtop_error_io (char *format, ...);
void glibtop_warn_io (char *format, ...);

#endif /* no __GNUC__ */

G_END_DECLS

#endif
