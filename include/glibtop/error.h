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


/*
 * This is private API
 */

#ifndef __GLIBTOP_ERROR_H__
#define __GLIBTOP_ERROR_H__

#include <glibtop.h>

G_BEGIN_DECLS

void glibtop_error_vr (glibtop *server, const char *format, va_list args) G_GNUC_NORETURN;
void glibtop_warn_vr (glibtop *server, const char *format, va_list args);
void glibtop_error_io_vr (glibtop *server, const char *format, int, va_list args) G_GNUC_NORETURN;
void glibtop_warn_io_vr (glibtop *server, const char *format, int, va_list args);

void glibtop_error_r (glibtop *server, const char *format, ...) G_GNUC_PRINTF(2, 3) G_GNUC_NORETURN;
void glibtop_warn_r (glibtop *server, const char *format, ...) G_GNUC_PRINTF(2, 3);
void glibtop_error_io_r (glibtop *server, const char *format, ...) G_GNUC_PRINTF(2, 3) G_GNUC_NORETURN;
void glibtop_warn_io_r (glibtop *server, const char *format, ...) G_GNUC_PRINTF(2, 3);

void glibtop_error (const char *format, ...) G_GNUC_PRINTF(1, 2) G_GNUC_NORETURN;
void glibtop_warn (const char *format, ...) G_GNUC_PRINTF(1, 2);
void glibtop_error_io (const char *format, ...) G_GNUC_PRINTF(1, 2) G_GNUC_NORETURN;
void glibtop_warn_io (const char *format, ...) G_GNUC_PRINTF(1, 2);

/* FIXME: C99 */
#define glibtop_debug_r(server, fmt, ...)				\
	G_STMT_START {							\
	if (LIBGTOP_ENABLE_DEBUG)					\
		glibtop_debug_r_real(server, "%s:%d %s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} G_STMT_END

#define glibtop_debug(...) glibtop_debug_r(glibtop_global_server, __VA_ARGS__)

void glibtop_debug_vr (glibtop *server, const char *format, va_list args);
void glibtop_debug_r_real (glibtop *server, const char *format, ...) G_GNUC_PRINTF(2, 3);

G_END_DECLS

#endif
