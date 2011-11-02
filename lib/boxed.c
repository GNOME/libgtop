/* Copyright (C) 2011 Red Hat
   This file is part of LibGTop 1.0.

   Contributed by Jasper St. Pierre <jstpierre@mecheye.net>, November 2011

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
#include <glibtop/mountlist.h>
#include <glibtop/procmap.h>
#include <glibtop/procopenfiles.h>

#define DEFINE_BOXED(type)                              \
    static type *                                       \
    type##_copy (type *p)                               \
    {                                                   \
        type *n = g_new (type, 1);                      \
        memcpy (n, p, sizeof (type));                   \
        return n;                                       \
    }                                                   \
                                                        \
    G_DEFINE_BOXED_TYPE (type,                          \
                         type,                          \
                         type##_copy,                   \
                         g_free)

DEFINE_BOXED (glibtop);
DEFINE_BOXED (glibtop_map_entry);
DEFINE_BOXED (glibtop_proc_map);
DEFINE_BOXED (glibtop_open_files_entry);
DEFINE_BOXED (glibtop_proc_open_files);
DEFINE_BOXED (glibtop_mountentry);
DEFINE_BOXED (glibtop_mountlist);
