/* Copyright (C) 2004 Benoît Dejean
   This file is part of LibGTop 2.0.


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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>

#include <glibtop.h>
#include <glibtop/netlist.h>


int main(int argc, char *argv [])
{
	glibtop_netlist buf;
	char **devices;
	guint32 i;

	glibtop_init();

	devices = glibtop_get_netlist(&buf);

	for(i = 0; i < buf.number; ++i)
	{
		printf("net device '%s'\n", devices[i]);
	}

	g_strfreev(devices);

	glibtop_close();
	return 0;
}
