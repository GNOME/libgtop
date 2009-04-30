/* This file is part of LibGTop 2.0.

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

#include <config.h>
#include <glibtop.h>
#include <glibtop/netlist.h>

static const unsigned long _glibtop_sysdeps_netlist = 0;

/* Init function. */

void
_glibtop_init_netlist_s (glibtop *server)
{
	server->sysdeps.netlist = _glibtop_sysdeps_netlist;
}

char**
glibtop_get_netlist_s (glibtop *server, glibtop_netlist *buf)
{
	memset (buf, 0, sizeof (glibtop_netlist));

	return NULL;
}
