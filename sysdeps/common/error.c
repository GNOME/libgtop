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

#include <config.h>

#include <glib.h>
#include <errno.h>

#include <glibtop/error.h>

enum MESSAGE_LEVEL {
	MESSAGE_DEBUG,
	MESSAGE_WARNING,
	MESSAGE_ERROR
};

#define DEFAULT_NAME	"LibGTop-Server"

/* Prints error message and exits. */

static void
print_message (const glibtop *server, int message_level, const char *format, int error, va_list args)
{
	const char *level;
	GString* message;

	message = g_string_new(NULL);

	g_string_printf(message, "%s", (server && server->name ? server->name : DEFAULT_NAME));

	if (server && server->pid) {
		g_string_append_printf(message, "(c=%u/s=%u)", getpid(), server->pid);
	}
	else {
		g_string_append_printf(message, "(c=%u)", getpid());
	}

	switch (message_level) {
	case MESSAGE_DEBUG:
		level = "DEBUG";
		break;
	case MESSAGE_WARNING:
		level = "WARNING";
		break;
	case MESSAGE_ERROR:
		level = "ERROR";
		break;
	default:
		level = "UNKNOWN";
	}

	g_string_append_printf(message, ": [%s] ", level);

	g_string_append_vprintf(message, format, args);

	if (error) {
		g_string_append_printf(message, ": %s", g_strerror(error));
	}

	g_string_append_c(message, '\n');

	fputs(message->str, stderr);

	g_string_free(message, TRUE);
}

void
glibtop_error_vr (glibtop *server, const char *format, va_list args)
{
	print_message (server, MESSAGE_ERROR, format, 0, args);

#ifdef LIBGTOP_ENABLE_DEBUG
	abort ();
#else
	exit (1);
#endif
}

void
glibtop_error_io_vr (glibtop *server, const char *format, int error, va_list args)
{
	print_message (server, MESSAGE_ERROR, format, error, args);

#ifdef LIBGTOP_ENABLE_DEBUG
	abort ();
#else
	exit (1);
#endif
}

void
glibtop_warn_vr (glibtop *server, const char *format, va_list args)
{
	print_message (server, MESSAGE_WARNING, format, 0, args);

#ifdef LIBGTOP_FATAL_WARNINGS
	abort ();
#endif
}

void
glibtop_warn_io_vr (glibtop *server, const char *format, int error, va_list args)
{
	print_message (server, MESSAGE_WARNING, format, error, args);

#ifdef LIBGTOP_FATAL_WARNINGS
	abort ();
#endif
}


void
glibtop_error_r (glibtop *server, const char *format, ...)
{
	va_list args;

	va_start (args, format);
	glibtop_error_vr (server, format, args);
	va_end (args);
}


void
glibtop_warn_r (glibtop *server, const char *format, ...)
{
	va_list args;

	va_start (args, format);
	glibtop_warn_vr (server, format, args);
	va_end (args);
}

void
glibtop_error_io_r (glibtop *server, const char *format, ...)
{
	va_list args;

	va_start (args, format);
	glibtop_error_io_vr (server, format, errno, args);
	va_end (args);
}

void
glibtop_warn_io_r (glibtop *server, const char *format, ...)
{
	va_list args;

	va_start (args, format);
	glibtop_warn_io_vr (server, format, errno, args);
	va_end (args);
}


void
glibtop_debug_vr (glibtop *server, const char *format, va_list args)
{
	print_message (server, MESSAGE_DEBUG, format, 0, args);
}

void
glibtop_debug_r_real (glibtop *server, const char *format, ...)
{
	va_list args;

	va_start (args, format);
	glibtop_debug_vr (server, format, args);
	va_end (args);
}


void
glibtop_error (const char *format, ...)
{
	va_list args;
	va_start (args, format);
	glibtop_error_vr (glibtop_global_server, format, args);
	va_end (args);
}

void
glibtop_warn (const char *format, ...)
{
	va_list args;
	va_start (args, format);
	glibtop_warn_vr (glibtop_global_server, format, args);
	va_end (args);
}

void
glibtop_error_io (const char *format, ...)
{
	va_list args;
	va_start (args, format);
	glibtop_error_io_vr (glibtop_global_server, format, errno, args);
	va_end (args);
}

void
glibtop_warn_io (const char *format, ...)
{
	va_list args;
	va_start (args, format);
	glibtop_warn_io_vr (glibtop_global_server, format, errno, args);
	va_end (args);
}
