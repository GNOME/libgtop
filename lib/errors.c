/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

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

#include <glibtop.h>
#include <glibtop/global.h>
#include <glibtop/errors.h>

/* Error quark */
GQuark
glibtop_error_quark (void)
{
    static GQuark q = 0;
    if (G_UNLIKELY(q == 0))
	q = g_quark_from_static_string ("glibtop-error-quark");

    return q;
}

void
glibtop_error_vl (glibtop_client *client, glibtop_error code,
		  const char *format, va_list args)
{
    gchar *message;
    GError *error;

    g_return_if_fail (GLIBTOP_IS_CLIENT (client));
    message = g_strdup_vprintf (format, args);

    error = g_error_new_literal (GLIBTOP_ERROR, code, message);
    glibtop_client_propagate_error (client, error);

    g_error_free (error);
    g_free (message);
}

void
glibtop_error_io_vl (glibtop_client *client, glibtop_error code,
		     int io_errno, const char *format, va_list args)
{
    gchar *message, *fullmessage;
    GError *error;

    g_return_if_fail (GLIBTOP_IS_CLIENT (client));
    message = g_strdup_vprintf (format, args);
    fullmessage = g_strdup_printf ("%s: %s", message, strerror (io_errno));

    error = g_error_new_literal (GLIBTOP_ERROR, code, message);
    glibtop_client_propagate_error (client, error);

    g_error_free (error);
    g_free (fullmessage);
    g_free (message);
}

void
glibtop_warn_vl (glibtop_client *client, glibtop_error code,
		 const char *format, va_list args)
{
    gchar *message;
    GError *error;

    g_return_if_fail (GLIBTOP_IS_CLIENT (client));
    message = g_strdup_vprintf (format, args);

    error = g_error_new_literal (GLIBTOP_ERROR, code, message);
    glibtop_client_propagate_warning (client, error);

    g_error_free (error);
    g_free (message);
}

void
glibtop_warn_io_vl (glibtop_client *client, glibtop_error code,
		    int io_errno, const char *format, va_list args)
{
    gchar *message, *fullmessage;
    GError *error;

    g_return_if_fail (GLIBTOP_IS_CLIENT (client));
    message = g_strdup_vprintf (format, args);
    fullmessage = g_strdup_printf ("%s: %s", message, strerror (io_errno));

    error = g_error_new_literal (GLIBTOP_ERROR, code, message);
    glibtop_client_propagate_warning (client, error);

    g_error_free (error);
    g_free (fullmessage);
    g_free (message);
}

void
glibtop_error_l (glibtop_client *client, glibtop_error code,
		 char *format, ...)
{
    va_list args;

    va_start (args, format);
    glibtop_error_vl (client, code, format, args);
    va_end (args);
}

void
glibtop_warn_l (glibtop_client *client, glibtop_error code,
		char *format, ...)
{
    va_list args;

    va_start (args, format);
    glibtop_warn_vl (client, code, format, args);
    va_end (args);
}

void
glibtop_error_io_l (glibtop_client *client, glibtop_error code,
		    char *format, ...)
{
    va_list args;

    va_start (args, format);
    glibtop_error_io_vl (client, code, errno, format, args);
    va_end (args);
}

void
glibtop_warn_io_l (glibtop_client *client, glibtop_error code,
		   char *format, ...)
{
    va_list args;

    va_start (args, format);
    glibtop_warn_io_vl (client, code, errno, format, args);
    va_end (args);
}

