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

#include <glibtop/backend.h>

#include <gmodule.h>

static void
unload_module (gpointer data, gpointer user_data)
{
    g_module_close (data);
}

static int
load_extra_libs (glibtop_client *client, glibtop_backend_entry *entry,
		 GError **error)
{
    GSList *list;
    GSList *loaded_here = NULL;

    for (list = entry->extra_libs; list; list = list->next) {
	const gchar *filename = list->data;
	GModule *module;

	module = g_module_open (filename, G_MODULE_BIND_LAZY);
	if (!module) {
	    g_set_error (error, GLIBTOP_ERROR, GLIBTOP_ERROR_NO_SUCH_BACKEND,
			 "Cannot open extra shared library `%s' "
			 "for backend `%s' (%s)", filename, entry->name,
			 g_module_error ());
	    g_slist_foreach (loaded_here, unload_module, NULL);
	    return -GLIBTOP_ERROR_NO_SUCH_BACKEND;
	}

	loaded_here = g_slist_prepend (loaded_here, module);
    }

    entry->_priv->extra_modules = g_slist_concat
	(loaded_here, entry->_priv->extra_modules);

    return 0;
}

glibtop_backend *
glibtop_open_backend_l (glibtop_client *client, const char *backend_name,
			u_int64_t features, const char **backend_args,
			GError **error)
{
    const glibtop_backend_info *info;
    glibtop_backend_entry *entry;
    glibtop_backend *backend;

    g_return_val_if_fail (GLIBTOP_IS_CLIENT (client), NULL);

    entry = glibtop_backend_by_name (backend_name);
    if (!entry) return NULL;

    if (!entry->_priv) {
	entry->_priv = g_new0 (glibtop_backend_module, 1);

	if (entry->extra_libs) {
	    int retval;

	    retval = load_extra_libs (client, entry, error);
	    if (retval < 0) return NULL;
	}

	entry->_priv->module = g_module_open (entry->shlib_name,
					      G_MODULE_BIND_LAZY);
	if (!entry->_priv->module) {
	    g_set_error (error, GLIBTOP_ERROR, GLIBTOP_ERROR_NO_SUCH_BACKEND,
			 "Cannot open shared library `%s' "
			 "for backend `%s' (%s)", entry->shlib_name,
			 entry->name, g_module_error ());
	    return NULL;
	}

	if (!g_module_symbol (entry->_priv->module,
			      "LibGTopBackendInfo",
			      (gpointer*) &entry->info)) {
	    g_set_error (error, GLIBTOP_ERROR, GLIBTOP_ERROR_NO_SUCH_BACKEND,
			 "Library `%s' is not a valid "
			 "LibGTop Backend library (start symbol not found)",
			 entry->shlib_name);

	    g_module_close (entry->_priv->module);
	    g_free (entry->_priv);
	    entry->_priv = NULL;

	    return NULL;
	}
    }

    entry->_priv->refcount++;

    info = entry->info;
    if (!info) return NULL;

    backend = g_new0 (glibtop_backend, 1);
    backend->_priv_module = entry->_priv;
    backend->info = info;

    backend->server = glibtop_server_new ();

    if (info->open) {
	int retval;

	retval = info->open (backend->server, backend, features, backend_args);
	if (retval) {
	    glibtop_server_unref (backend->server);
	    g_free (backend->_priv);
	    g_free (backend);
	    return NULL;
	}
    }

    return backend;
}
