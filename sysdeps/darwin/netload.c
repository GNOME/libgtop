/*
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

#include <config.h>
#include <unistd.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/netload.h>

#include <glibtop_suid.h>

#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <net/route.h>

static const unsigned long _glibtop_sysdeps_netload =
(1L << GLIBTOP_NETLOAD_IF_FLAGS) +
(1L << GLIBTOP_NETLOAD_MTU) +
/* (1L << GLIBTOP_NETLOAD_SUBNET) + */
/* (1L << GLIBTOP_NETLOAD_ADDRESS) + */
(1L << GLIBTOP_NETLOAD_PACKETS_IN) +
(1L << GLIBTOP_NETLOAD_PACKETS_OUT) +
(1L << GLIBTOP_NETLOAD_PACKETS_TOTAL) +
(1L << GLIBTOP_NETLOAD_BYTES_IN) +
(1L << GLIBTOP_NETLOAD_BYTES_OUT) +
(1L << GLIBTOP_NETLOAD_BYTES_TOTAL) +
(1L << GLIBTOP_NETLOAD_ERRORS_IN) +
(1L << GLIBTOP_NETLOAD_ERRORS_OUT) +
(1L << GLIBTOP_NETLOAD_ERRORS_TOTAL) +
(1L << GLIBTOP_NETLOAD_COLLISIONS);

/* Init function. */

void
_glibtop_init_netload_p (glibtop *server)
{
	server->sysdeps.netload = _glibtop_sysdeps_netload;
}

/* Provides Network statistics. */

void
glibtop_get_netload_p (glibtop *server, glibtop_netload *buf,
		       const char *interface)
{
	int mib[] = { CTL_NET, PF_ROUTE, 0, 0, NET_RT_IFLIST, 0 };
	size_t bufsize;
	char *rtbuf, *ptr, *eob;
	struct if_msghdr *ifm;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_NETLOAD), 0);

	memset (buf, 0, sizeof (glibtop_netload));

	if (sysctl (mib, 6, NULL, &bufsize, NULL, 0) < 0)
		return;

	rtbuf = (char*)g_malloc (bufsize);
	if (rtbuf == NULL)
		return;

	if (sysctl (mib, 6, rtbuf, &bufsize, NULL, 0) < 0) {
		g_free (rtbuf);
		return;
	}

	eob = rtbuf + bufsize;
	ptr = rtbuf;
	while (ptr < eob) {
		struct sockaddr_dl *sdl;

		ifm = (struct if_msghdr*) ptr;

		if (ifm->ifm_type != RTM_IFINFO)
			break;
		ptr += ifm->ifm_msglen;

		while (ptr < eob) {
			struct if_msghdr *nextifm = (struct if_msghdr*) ptr;

			if (nextifm->ifm_type != RTM_NEWADDR)
				break;
			ptr += nextifm->ifm_msglen;
		}

		sdl = (struct sockaddr_dl*) (ifm + 1);
		if (sdl->sdl_family != AF_LINK)
			continue;
		if (strlen (interface) != sdl->sdl_nlen)
			continue;
		if (strcmp (interface, sdl->sdl_data) == 0)
			goto FOUND;
	}
	g_free (rtbuf);
	return;

FOUND:
	if (ifm->ifm_flags & IFF_UP)
		buf->if_flags |= GLIBTOP_IF_FLAGS_UP;
	if (ifm->ifm_flags & IFF_BROADCAST)
		buf->if_flags |= GLIBTOP_IF_FLAGS_BROADCAST;
	if (ifm->ifm_flags & IFF_DEBUG)
		buf->if_flags |= GLIBTOP_IF_FLAGS_DEBUG;
	if (ifm->ifm_flags & IFF_LOOPBACK)
		buf->if_flags |= GLIBTOP_IF_FLAGS_LOOPBACK;
	if (ifm->ifm_flags & IFF_POINTOPOINT)
		buf->if_flags |= GLIBTOP_IF_FLAGS_POINTOPOINT;
	if (ifm->ifm_flags & IFF_RUNNING)
		buf->if_flags |= GLIBTOP_IF_FLAGS_RUNNING;
	if (ifm->ifm_flags & IFF_NOARP)
		buf->if_flags |= GLIBTOP_IF_FLAGS_NOARP;
	if (ifm->ifm_flags & IFF_NOARP)
		buf->if_flags |= GLIBTOP_IF_FLAGS_PROMISC;
	if (ifm->ifm_flags & IFF_ALLMULTI)
		buf->if_flags |= GLIBTOP_IF_FLAGS_ALLMULTI;
	if (ifm->ifm_flags & IFF_OACTIVE)
		buf->if_flags |= GLIBTOP_IF_FLAGS_OACTIVE;
	if (ifm->ifm_flags & IFF_SIMPLEX)
		buf->if_flags |= GLIBTOP_IF_FLAGS_SIMPLEX;
	if (ifm->ifm_flags & IFF_LINK0)
		buf->if_flags |= GLIBTOP_IF_FLAGS_LINK0;
	if (ifm->ifm_flags & IFF_LINK1)
		buf->if_flags |= GLIBTOP_IF_FLAGS_LINK1;
	if (ifm->ifm_flags & IFF_LINK2)
		buf->if_flags |= GLIBTOP_IF_FLAGS_LINK2;
	if (ifm->ifm_flags & IFF_ALTPHYS)
		buf->if_flags |= GLIBTOP_IF_FLAGS_ALTPHYS;
	if (ifm->ifm_flags & IFF_MULTICAST)
		buf->if_flags |= GLIBTOP_IF_FLAGS_MULTICAST;
	buf->mtu		= ifm->ifm_data.ifi_mtu;
	buf->subnet		= 0; /* FIXME */
	buf->address		= 0; /* FIXME */
	buf->packets_in		= ifm->ifm_data.ifi_ipackets;
	buf->packets_out	= ifm->ifm_data.ifi_opackets;
	buf->packets_total	= buf->packets_in + buf->packets_out;
	buf->bytes_in		= ifm->ifm_data.ifi_ibytes;
	buf->bytes_out		= ifm->ifm_data.ifi_obytes;
	buf->bytes_total	= buf->bytes_in + buf->bytes_out;
	buf->errors_in		= ifm->ifm_data.ifi_ierrors;
	buf->errors_out		= ifm->ifm_data.ifi_oerrors;
	buf->errors_total	= buf->errors_in + buf->errors_out;
	buf->collisions		= ifm->ifm_data.ifi_collisions;
	buf->flags		= _glibtop_sysdeps_netload;
	g_free (buf);
}
