/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Dr. James Dominic P. Guana <jdpguana@histark.com>, August 2021.

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

#include <locale.h>
#include <libintl.h>
#include <math.h>
#include <stdio.h>

#include <glibtop.h>
#include <glibtop/sysinfo.h>

int
main (int argc, char *argv [])
{
   char *model;
   const glibtop_sysinfo * sysinfo;

   glibtop_init();
   sysinfo = glibtop_get_sysinfo ();
   model = g_hash_table_lookup (sysinfo->cpuinfo [1].values, "model name");

   printf ("\nProcessor Model: %s\n", g_strdup (model));
   printf ("Number of Cores: %" G_GUINT64_FORMAT "\n\n", sysinfo->ncpu);

   glibtop_close ();

   exit (0);
}
