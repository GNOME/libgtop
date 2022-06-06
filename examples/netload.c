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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

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


static char* hwaddress_format_for_display(glibtop_netload *buf)
{
	unsigned i;
	GString *repr = g_string_new("");
	char *str;

	for(i = 0; i < sizeof buf->hwaddress; ++i)
		g_string_append_printf(repr, "%02X:",
				       ((unsigned char*)buf->hwaddress)[i]);

	repr->str [ repr->len - 1] = ' ';
	str = g_string_free(repr, FALSE);
	g_strstrip(str);
	return str;
}


int
main (int argc, char *argv [])
{
	glibtop_netload netload;
	unsigned method, count, port;
	struct in_addr addr, subnet;
	char address_string[INET_ADDRSTRLEN], subnet_string[INET_ADDRSTRLEN];
	char address6_string[INET6_ADDRSTRLEN], prefix6_string[INET6_ADDRSTRLEN];
	char *hwaddress_string;
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
		g_error ("Usage: %s interface", argv [0]);

	glibtop_get_netload (&netload, argv [1]);

	addr.s_addr = netload.address;
	subnet.s_addr = netload.subnet;

	inet_ntop (AF_INET, &addr,   address_string, INET_ADDRSTRLEN);
	inet_ntop (AF_INET, &subnet, subnet_string,  INET_ADDRSTRLEN);
	inet_ntop (AF_INET6, netload.address6, address6_string, INET6_ADDRSTRLEN);
	inet_ntop (AF_INET6, netload.prefix6,  prefix6_string,  INET6_ADDRSTRLEN);

	hwaddress_string = hwaddress_format_for_display(&netload);

	printf ("Network Load (0x%016llx):\n\n"
		"\tInterface Flags:\t0x%016llx\n"
		"\tAddress:\t\t0x%08x - %s\n"
		"\tSubnet:\t\t\t0x%08x - %s\n\n"
		"\tMTU:\t\t\t%d\n"
		"\tCollisions:\t\t%" G_GUINT64_FORMAT "\n\n"
		"\tPackets In:\t\t%" G_GUINT64_FORMAT "\n"
		"\tPackets Out:\t\t%" G_GUINT64_FORMAT "\n"
		"\tPackets Total:\t\t%" G_GUINT64_FORMAT "\n\n"
		"\tBytes In:\t\t%" G_GUINT64_FORMAT "\n"
		"\tBytes Out:\t\t%" G_GUINT64_FORMAT "\n"
		"\tBytes Total:\t\t%" G_GUINT64_FORMAT "\n\n"
		"\tErrors In:\t\t%" G_GUINT64_FORMAT "\n"
		"\tErrors Out:\t\t%" G_GUINT64_FORMAT "\n"
		"\tErrors Total:\t\t%" G_GUINT64_FORMAT "\n\n"
		"\tAddress6:\t\t%s\n"
		"\tPrefix6:\t\t%s\n"
		"\tScope6:\t\t\t%#03x\n\n"
		"\tHarware Address:\t%s\n\n",
		(unsigned long long)netload.flags,
		(unsigned long long)netload.if_flags,
		(guint32) netload.address, address_string,
		(guint32) netload.subnet,  subnet_string,
		netload.mtu,
		netload.collisions,
		netload.packets_in,
		netload.packets_out,
		netload.packets_total,
		netload.bytes_in,
		netload.bytes_out,
		netload.bytes_total,
		netload.errors_in,
		netload.errors_out,
		netload.errors_total,
		address6_string,
		prefix6_string,
		(int) netload.scope6,
		hwaddress_string);


	glibtop_close ();

	exit (0);
}
