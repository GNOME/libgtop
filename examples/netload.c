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

#include <locale.h>
#include <libintl.h>
#include <stdio.h>

#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/close.h>

#include <glibtop/parameter.h>

#include <glibtop/netload.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef PROFILE_COUNT
#define PROFILE_COUNT	1
#endif

int
main (int argc, char *argv [])
{
	glibtop_netload netload;
	unsigned method, count, port;
	struct in_addr addr, subnet;
	char *address_string, *subnet_string;
	char buffer [BUFSIZ];

	count = PROFILE_COUNT;

	setlocale (LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, GTOPLOCALEDIR);
	textdomain (GETTEXT_PACKAGE);

	glibtop_init_r (&glibtop_global_server, 0, GLIBTOP_INIT_NO_OPEN);

	glibtop_get_parameter (GLIBTOP_PARAM_METHOD, &method, sizeof (method));

	printf ("Method = %d\n", method);

	count = glibtop_get_parameter (GLIBTOP_PARAM_COMMAND, buffer, BUFSIZ);
	buffer [count] = 0;

	printf ("Command = '%s'\n", buffer);

	count = glibtop_get_parameter (GLIBTOP_PARAM_HOST, buffer, BUFSIZ);
	buffer [count] = 0;

	glibtop_get_parameter (GLIBTOP_PARAM_PORT, &port, sizeof (port));

	printf ("Host = '%s' - %u\n\n", buffer, port);

	glibtop_init_r (&glibtop_global_server, 0, 0);

	if (argc != 2)
		glibtop_error ("Usage: %s interface", argv [0]);

	glibtop_get_netload (&netload, argv [1]);

	addr.s_addr = netload.address;
	subnet.s_addr = netload.subnet;

	address_string = g_strdup (inet_ntoa (addr));
	subnet_string  = g_strdup (inet_ntoa (subnet));

	printf ("Network Load (0x%08lx):\n\n"
		"\tInterface Flags:\t0x%08lx\n"
		"\tAddress:\t\t0x%08lx - %s\n"
		"\tSubnet:\t\t\t0x%08lx - %s\n\n"
		"\tMTU:\t\t\t%ld\n"
		"\tCollisions:\t\t%ld\n\n"
		"\tPackets In:\t\t%ld\n"
		"\tPackets Out:\t\t%ld\n"
		"\tPackets Total:\t\t%ld\n\n"
		"\tBytes In:\t\t%ld\n"
		"\tBytes Out:\t\t%ld\n"
		"\tBytes Total:\t\t%ld\n\n"
		"\tErrors In:\t\t%ld\n"
		"\tErrors Out:\t\t%ld\n"
		"\tErrors Total:\t\t%ld\n\n",
		(unsigned long) netload.flags,
		(unsigned long) netload.if_flags,
		(unsigned long) netload.address, address_string,
		(unsigned long) netload.subnet,  subnet_string,
		(unsigned long) netload.mtu,
		(unsigned long) netload.collisions,
		(unsigned long) netload.packets_in,
		(unsigned long) netload.packets_out,
		(unsigned long) netload.packets_total,
		(unsigned long) netload.bytes_in,
		(unsigned long) netload.bytes_out,
		(unsigned long) netload.bytes_total,
		(unsigned long) netload.errors_in,
		(unsigned long) netload.errors_out,
		(unsigned long) netload.errors_total);

	g_free (address_string);
	g_free (subnet_string);

	glibtop_close ();

	exit (0);
}
