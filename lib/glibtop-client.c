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

static void glibtop_client_class_init    (glibtop_client_class   *klass);
static void glibtop_client_init          (glibtop_client         *client);
static void glibtop_client_finalize      (GObject                *object);

static gpointer parent_class = NULL;


/* Internal data */

struct _glibtop_client_private
{
};

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
glibtop_client_class_init (glibtop_client_class *class)
{
    GObjectClass *gobject_class;

    gobject_class = (GObjectClass *) class;
  
    parent_class = g_type_class_ref (G_TYPE_OBJECT);
  
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
