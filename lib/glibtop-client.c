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

#include <gobject/gsignal.h>
#include <gobject/gvaluetypes.h>

static void glibtop_client_class_init    (glibtop_client_class   *klass);
static void glibtop_client_init          (glibtop_client         *client);
static void glibtop_client_finalize      (GObject                *object);

static gpointer parent_class = NULL;


/* Internal data */

struct _glibtop_client_private
{
};

enum {
    GLIBTOP_CLIENT_SIGNAL_ERROR,
    GLIBTOP_CLIENT_SIGNAL_WARNING,
    LAST_SIGNAL
};

static guint glibtop_client_signals [LAST_SIGNAL] = { 0 };

GType
glibtop_client_get_type (void)
{
    static GType glibtop_client_type = 0;
  
    if (!glibtop_client_type) {
	static const GTypeInfo glibtop_client_info = {
	    sizeof (glibtop_client_class),
	    NULL,		/* base_class_init */
	    NULL,		/* base_class_finalize */
	    (GClassInitFunc) glibtop_client_class_init,
	    NULL,		/* class_finalize */
	    NULL,		/* class_data */
	    sizeof (glibtop_client),
	    16,			/* n_preallocs */
	    (GInstanceInitFunc) glibtop_client_init,
	};
      
	glibtop_client_type = g_type_register_static
	    (G_TYPE_OBJECT, "glibtop_client", &glibtop_client_info, 0);
    }

    return glibtop_client_type;
}

static void
glibtop_client_error_handler (glibtop_client *client, GError *error)
{
    g_return_if_fail (GLIBTOP_IS_CLIENT (client));

    if (error == NULL)
	return;

    g_error ("%s (%d): %s", g_quark_to_string (error->domain),
	     error->code, error->message);
}

static void
glibtop_client_warning_handler (glibtop_client *client, GError *error)
{
    g_return_if_fail (GLIBTOP_IS_CLIENT (client));

    if (error == NULL)
	return;

    g_warning ("%s (%d): %s", g_quark_to_string (error->domain),
	       error->code, error->message);

}

void
glibtop_client_marshal_VOID__POINTER (GClosure     *closure,
				      GValue       *return_value,
				      guint         n_param_values,
				      const GValue *param_values,
				      gpointer      invocation_hint,
				      gpointer      marshal_data)
{
    typedef void (*GSignalFunc_VOID__POINTER) (gpointer     data1,
					       gpointer     arg_1,
					       gpointer     data2);
    register GSignalFunc_VOID__POINTER callback;
    register GCClosure *cc = (GCClosure*) closure;
    register gpointer data1, data2;

    g_return_if_fail (n_param_values >= 2);

    if (G_CCLOSURE_SWAP_DATA (closure))
	{
	    data1 = closure->data;
	    data2 = g_value_get_as_pointer (param_values + 0);
	}
    else
	{
	    data1 = g_value_get_as_pointer (param_values + 0);
	    data2 = closure->data;
	}
    callback = (GSignalFunc_VOID__POINTER) (marshal_data ? marshal_data : cc->callback);

    callback (data1,
	      g_value_get_as_pointer (param_values + 1),
	      data2);
}

static void
glibtop_client_class_init (glibtop_client_class *class)
{
    GObjectClass *gobject_class;
    GType *param_types;
    GClosure *closure;

    gobject_class = (GObjectClass *) class;
  
    parent_class = g_type_class_ref (G_TYPE_OBJECT);

    closure = g_signal_type_closure_new (G_TYPE_FROM_CLASS (class),
					 G_STRUCT_OFFSET (glibtop_client_class,
							  error));

    param_types = g_new0 (GType, 1);
    param_types [0] = G_TYPE_POINTER;
  
    glibtop_client_signals [GLIBTOP_CLIENT_SIGNAL_ERROR] =
	g_signal_newv ("error", G_TYPE_FROM_CLASS (class),
		       G_SIGNAL_RUN_LAST, closure, NULL,
		       glibtop_client_marshal_VOID__POINTER,
		       G_TYPE_NONE, 1, param_types);

    closure = g_signal_type_closure_new (G_TYPE_FROM_CLASS (class),
					 G_STRUCT_OFFSET (glibtop_client_class,
							  warning));
  
    glibtop_client_signals [GLIBTOP_CLIENT_SIGNAL_WARNING] =
	g_signal_newv ("warning", G_TYPE_FROM_CLASS (class),
		       G_SIGNAL_RUN_LAST, closure, NULL,
		       glibtop_client_marshal_VOID__POINTER,
		       G_TYPE_NONE, 1, param_types);

    g_free (param_types);

    class->error = glibtop_client_error_handler;
    class->warning = glibtop_client_warning_handler;
  
    gobject_class->finalize = glibtop_client_finalize;
}

static void
glibtop_client_init (glibtop_client *glibtop)
{
    glibtop_client_private *priv;
  
    priv = g_new0 (glibtop_client_private, 1);
    glibtop->_priv = priv;
}

static void
glibtop_client_finalize (GObject *object)
{
    glibtop_client *glibtop;
    glibtop_client_private *priv = NULL;
  
    glibtop = GLIBTOP_CLIENT (object);
    priv = glibtop->_priv;
  
    g_free (priv);
  
    if (G_OBJECT_CLASS (parent_class)->finalize)
	G_OBJECT_CLASS (parent_class)->finalize (object);
}

glibtop_client *
glibtop_client_new (void)
{
    return g_object_new (GLIBTOP_TYPE_CLIENT, NULL);
}

void
glibtop_client_propagate_error (glibtop_client *client, GError *error)
{
    GValue params [2] = { { 0, }, { 0, } };

    g_return_if_fail (GLIBTOP_IS_CLIENT (client));

    if (error == NULL)
	return;

    g_value_init (params, GLIBTOP_CLIENT_TYPE (client));
    g_value_set_object (params, G_OBJECT (client));

    g_value_init (params + 1, G_TYPE_POINTER);
    g_value_set_pointer (params + 1, error);

    g_signal_emitv (params,
		    glibtop_client_signals [GLIBTOP_CLIENT_SIGNAL_ERROR],
		    0, NULL);
  
    g_value_unset (params + 1);
    g_value_unset (params + 0);
}

void
glibtop_client_propagate_warning (glibtop_client *client, GError *error)
{
    GValue params [2] = { { 0, }, { 0, } };

    g_return_if_fail (GLIBTOP_IS_CLIENT (client));

    if (error == NULL)
	return;

    g_value_init (params, GLIBTOP_CLIENT_TYPE (client));
    g_value_set_object (params, G_OBJECT (client));

    g_value_init (params + 1, G_TYPE_POINTER);
    g_value_set_pointer (params + 1, error);

    g_signal_emitv (params,
		    glibtop_client_signals [GLIBTOP_CLIENT_SIGNAL_WARNING],
		    0, NULL);
  
    g_value_unset (params + 1);
    g_value_unset (params + 0);
}
