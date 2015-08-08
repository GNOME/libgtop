/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, October 1998.

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
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/netload.h>

#include "utils.h"

static const unsigned long _glibtop_sysdeps_netload =
(1 << GLIBTOP_NETLOAD_IF_FLAGS) + (1 << GLIBTOP_NETLOAD_MTU) +
(1 << GLIBTOP_NETLOAD_SUBNET) + (1 << GLIBTOP_NETLOAD_ADDRESS) +
(1 << GLIBTOP_NETLOAD_PACKETS_IN) + (1 << GLIBTOP_NETLOAD_PACKETS_OUT) +
(1 << GLIBTOP_NETLOAD_PACKETS_TOTAL) + (1 << GLIBTOP_NETLOAD_BYTES_IN) +
(1 << GLIBTOP_NETLOAD_BYTES_OUT) + (1 << GLIBTOP_NETLOAD_BYTES_TOTAL) +
(1 << GLIBTOP_NETLOAD_ERRORS_IN) + (1 << GLIBTOP_NETLOAD_ERRORS_OUT) +
(1 << GLIBTOP_NETLOAD_ERRORS_TOTAL) + (1 << GLIBTOP_NETLOAD_COLLISIONS);

static void get_ifaddr(glibtop* server, struct ifaddr* next, long* addr, long* mask)
{
	struct ifaddr ifaddr;
	struct sockaddr_in sockaddr;

	*addr = 0;
	*mask = 0;

	while (next)
	{
		/* get interface ifaddr structure */

		_glibtop_get_kmem_info(server, (off_t)next, &ifaddr, sizeof(ifaddr));

		/* get socket addr information */

		_glibtop_get_kmem_info( server, (off_t)ifaddr.ifa_addr
				     , &sockaddr, sizeof(sockaddr));

		/* if valid family : UDP, TCP, ...*/

		if (sockaddr.sin_family == AF_INET)
		{
			*addr = sockaddr.sin_addr.s_addr;

			_glibtop_get_kmem_info( server, (off_t)ifaddr.ifa_netmask
					     , &sockaddr, sizeof(sockaddr));

			*mask = sockaddr.sin_addr.s_addr;

			/* address found */

			return;
		}

		next = ifaddr.ifa_next;
	}
}

/* Init function. */

void
_glibtop_init_netload_p (glibtop *server)
{
	off_t result;
	off_t addr;

	result = _glibtop_get_kmem_offset(server, "ifnet");
	if (result <= 0)
	{
		server->sysdeps.netload = 0;
		return;
	}

	result = _glibtop_get_kmem_info(server, result, &addr, sizeof(addr));
	if (result <= 0)
	{
		server->sysdeps.netload = 0;
		return;
	}

	server->machine->ifnet_offset = addr;

	server->sysdeps.netload = _glibtop_sysdeps_netload;
}

/* Provides network statistics. */

void
glibtop_get_netload_p (glibtop *server, glibtop_netload *buf,
		       const char *interface)
{
	int result;
	off_t offset;
	struct ifnet ifnet;
	long addr;
	long mask;
	char name[16];

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_UPTIME), 0);

	memset (buf, 0, sizeof (glibtop_netload));

	for ( offset = server->machine->ifnet_offset;
	      offset != 0;
	      offset = (off_t)ifnet.if_next
	    )
	{
		result = _glibtop_get_kmem_info(server, offset,
						&ifnet, sizeof(ifnet));
		if (result <= 0)
		{
			glibtop_error_io_r (server, "Cannot read ifnet");

			return;
		}

		result = _glibtop_get_kmem_info(server, (off_t)ifnet.if_name,
						name, sizeof(name));
		if (result <= 0)
		{
			glibtop_error_io_r (server, "Cannot read if_name");

			return;
		}

		sprintf(name+strlen(name), "%d", ifnet.if_unit);
		if (strcmp(name, interface) != 0)
		{
			continue;
		}

		if (ifnet.if_flags & IFF_UP)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_UP);
		if (ifnet.if_flags & IFF_BROADCAST)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_BROADCAST);
		if (ifnet.if_flags & IFF_LOOPBACK)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_LOOPBACK);
		if (ifnet.if_flags & IFF_POINTOPOINT)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_POINTOPOINT);
		if (ifnet.if_flags & IFF_RUNNING)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_RUNNING);
		if (ifnet.if_flags & IFF_NOARP)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_NOARP);
		if (ifnet.if_flags & IFF_PROMISC)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_PROMISC);
		if (ifnet.if_flags & IFF_ALLMULTI)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_ALLMULTI);
		if (ifnet.if_flags & IFF_OACTIVE)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_OACTIVE);
		if (ifnet.if_flags & IFF_SIMPLEX)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_SIMPLEX);
		if (ifnet.if_flags & IFF_LINK0)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_LINK0);
		if (ifnet.if_flags & IFF_LINK1)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_LINK1);
		if (ifnet.if_flags & IFF_LINK2)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_LINK2);
		if (ifnet.if_flags & IFF_LINK2)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_LINK2);
		if (ifnet.if_flags & 0x80000)
			buf->if_flags |= (1 << GLIBTOP_IF_FLAGS_MULTICAST);

		buf->mtu = ifnet.if_mtu;

		get_ifaddr(server, ifnet.if_addrlist, &addr, &mask);

		buf->subnet = addr & mask;
		buf->address = addr;

		buf->packets_in = ifnet.if_ipackets;
		buf->packets_out = ifnet.if_opackets;
		buf->packets_total = buf->packets_in + buf->packets_out;

		buf->bytes_in = ifnet.if_ibytes;
		buf->bytes_out = ifnet.if_obytes;
		buf->bytes_total = buf->bytes_in + buf->bytes_out;

		buf->errors_in = ifnet.if_ierrors;
		buf->errors_out = ifnet.if_oerrors;
		buf->errors_total = ifnet.if_ierrors + ifnet.if_oerrors;

		buf->collisions = ifnet.if_collisions;
	}

	buf->flags = _glibtop_sysdeps_netload;
}
