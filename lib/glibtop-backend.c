/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 2.0.

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

#include <glib-object.h>

#include <glibtop/glibtop-backend.h>
#include <glibtop/glibtop-backend-info.h>

static void glibtop_backend_class_init    (glibtop_backend_class  *klass);
static void glibtop_backend_init          (glibtop_backend        *backend);
static void glibtop_backend_finalize      (GObject                *object);

static gpointer parent_class = NULL;


/* Internal data */

struct _glibtop_backend_private
{
    const glibtop_backend_info *info;

    glibtop_closure *closure_data;

    glibtop_server *server;
    glibtop_backend_module *module;
};

GType
glibtop_backend_get_type (void)
{
    static GType glibtop_backend_type = 0;
  
    if (!glibtop_backend_type) {
	static const GTypeInfo glibtop_backend_info = {
	    sizeof (glibtop_backend_class),
	    NULL,		/* base_class_init */
	    NULL,		/* base_class_finalize */
	    (GClassInitFunc) glibtop_backend_class_init,
	    NULL,		/* class_finalize */
	    NULL,		/* class_data */
	    sizeof (glibtop_backend),
	    16,			/* n_preallocs */
	    (GInstanceInitFunc) glibtop_backend_init,
	};
      
	glibtop_backend_type = g_type_register_static
	    (G_TYPE_OBJECT, "glibtop_backend", &glibtop_backend_info, 0);
    }

    return glibtop_backend_type;
}

static void
glibtop_backend_class_init (glibtop_backend_class *class)
{
    GObjectClass *gobject_class;

    gobject_class = (GObjectClass *) class;
  
    parent_class = g_type_class_ref (G_TYPE_OBJECT);

    gobject_class->finalize = glibtop_backend_finalize;
}

static void
glibtop_backend_init (glibtop_backend *backend)
{
    glibtop_backend_private *priv;

    priv = g_new0 (glibtop_backend_private, 1);
    backend->_priv = priv;
}

static void
glibtop_backend_finalize (GObject *object)
{
    glibtop_backend *glibtop;
    glibtop_backend_private *priv = NULL;
  
    glibtop = GLIBTOP_BACKEND (object);
    priv = glibtop->_priv;

    g_free (priv);
  
    if (G_OBJECT_CLASS (parent_class)->finalize)
	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
unload_module (gpointer data, gpointer user_data)
{
    g_module_close (data);
}

static int
load_extra_libs (glibtop_backend_entry *entry, GError **error)
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
glibtop_backend_get (const char *backend_name, u_int64_t features,
		     const char **backend_args, GError **error)
{
    const glibtop_backend_info *info;
    glibtop_backend_entry *entry;
    glibtop_backend *backend;

    glibtop_init_backends ();

    entry = glibtop_backend_by_name (backend_name);
    if (!entry) {
	g_set_error (error, GLIBTOP_ERROR, GLIBTOP_ERROR_NO_SUCH_BACKEND,
		     "No backend with this name");
	return NULL;
    }

    if (!entry->_priv) {
	entry->_priv = g_new0 (glibtop_backend_module, 1);

	if (entry->extra_libs) {
	    int retval;

	    retval = load_extra_libs (entry, error);
	    if (retval < 0)
		return NULL;
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

    info = entry->info;
    if (!info) {
	g_set_error (error, GLIBTOP_ERROR, GLIBTOP_ERROR_NO_SUCH_BACKEND,
		     "Can't get backend info");
	return NULL;
    }

    backend = g_object_new (GLIBTOP_TYPE_BACKEND, NULL);

    backend->_priv->module = entry->_priv;
    backend->_priv->info = info;

    backend->_priv->server = glibtop_server_new ();

    if (info->open) {
	int retval;

	retval = info->open (backend->_priv->server, backend, features,
			     backend_args);
	if (retval) {
	    g_set_error (error, GLIBTOP_ERROR, GLIBTOP_ERROR_NO_SUCH_BACKEND,
			 "Backend open function return error condition");

	    g_object_unref (G_OBJECT (backend));
	    return NULL;
	}
    }

    entry->_priv->refcount++;

    return backend;
}

glibtop_call_vector *
glibtop_backend_get_call_vector (glibtop_backend *backend)
{
    g_return_val_if_fail (GLIBTOP_IS_BACKEND (backend), NULL);

    g_assert (backend->_priv->info != NULL);

    return backend->_priv->info->call_vector;
}

glibtop_server *
glibtop_backend_get_server (glibtop_backend *backend)
{
    g_return_val_if_fail (GLIBTOP_IS_BACKEND (backend), NULL);

    glibtop_server_ref (backend->_priv->server);

    return backend->_priv->server;
}

glibtop_closure *
glibtop_backend_get_closure_data (glibtop_backend *backend)
{
    g_return_val_if_fail (GLIBTOP_IS_BACKEND (backend), NULL);

    return backend->_priv->closure_data;
}

void
glibtop_backend_set_closure_data (glibtop_backend *backend,
				  glibtop_closure *closure)
{
    g_return_if_fail (GLIBTOP_IS_BACKEND (backend));
    g_return_if_fail (backend->_priv->closure_data == NULL);

    backend->_priv->closure_data = closure;
}
