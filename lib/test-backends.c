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

#include <glibtop/cpu.h>

int
main (int argc, char *argv [])
{
    glibtop_client *client;
    glibtop_backend *backend_common;
    glibtop_backend *backend_sysdeps;
    GError *error = NULL;
    glibtop_cpu cpu;

    g_type_init (G_TYPE_DEBUG_NONE);

    client = glibtop_client_new ();

    backend_common = glibtop_backend_get ("glibtop-backend-common",
					  0, NULL, &error);

    if (error != NULL) {
	glibtop_client_propagate_warning (client, error);
	g_error_free (error);
	error = NULL;
    }

    g_message (G_STRLOC ": backend = %p", backend_common);

    if (backend_common != NULL)
	glibtop_client_add_backend (client, backend_common);

    backend_sysdeps = glibtop_backend_get ("glibtop-backend-sysdeps",
					   0, NULL, &error);

    if (error != NULL) {
	glibtop_client_propagate_warning (client, error);
	g_error_free (error);
	error = NULL;
    }

    g_message (G_STRLOC ": backend = %p", backend_sysdeps);

    if (backend_sysdeps != NULL)
	glibtop_client_add_backend (client, backend_sysdeps);

    glibtop_get_cpu_l (client, &cpu);

    exit (0);
}
