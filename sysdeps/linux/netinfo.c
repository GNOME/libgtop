/* $Id$ */

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
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/netinfo.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#if !defined (_LIBC) && defined (__GNU_LIBRARY__) && __GNU_LIBRARY__ > 1
/* GNU LibC */
#include <net/if.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/if.h>
#else /* Libc 5 */
#include <linux/if.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#endif

static const unsigned long _glibtop_sysdeps_netinfo =
(1L << GLIBTOP_NETINFO_ADDRESS) +
(1L << GLIBTOP_NETINFO_SUBNET) +
(1L << GLIBTOP_NETINFO_MTU);

/* Init function. */

int
glibtop_init_netinfo_s (glibtop *server)
{
    server->sysdeps.netinfo = _glibtop_sysdeps_netinfo;

    return 0;
}

/* Provides network statistics. */

int
glibtop_get_netinfo_s (glibtop *server, glibtop_netinfo *buf,
		       const char *interface, unsigned transport)
{
    int skfd;

    memset (buf, 0, sizeof (glibtop_netinfo));

    skfd = socket (AF_INET, SOCK_DGRAM, 0);
    if (skfd) {
	struct ifreq ifr;
	unsigned flags;

	strcpy (ifr.ifr_name, interface);
	if (!ioctl (skfd, SIOCGIFFLAGS, &ifr)) {
	    buf->flags |= (1L << GLIBTOP_NETINFO_IF_FLAGS);
	    flags = ifr.ifr_flags;
	} else
	    flags = 0;

	if (flags & IFF_UP)
	    buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_UP);

	if (flags & IFF_BROADCAST)
	    buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_BROADCAST);

	if (flags & IFF_DEBUG)
	    buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_DEBUG);

	if (flags & IFF_LOOPBACK)
	    buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_LOOPBACK);

	if (flags & IFF_POINTOPOINT)
	    buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_POINTOPOINT);

	if (flags & IFF_RUNNING)
	    buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_RUNNING);

	if (flags & IFF_NOARP)
	    buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_NOARP);

	if (flags & IFF_PROMISC)
	    buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_PROMISC);

	if (flags & IFF_ALLMULTI)
	    buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_ALLMULTI);

	if (flags & IFF_MULTICAST)
	    buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_MULTICAST);

	strcpy (ifr.ifr_name, interface);
	if (!ioctl (skfd, SIOCGIFADDR, &ifr)) {
	    struct sockaddr_in addr =
		*(struct sockaddr_in *) &ifr.ifr_addr;
	    buf->address = addr.sin_addr.s_addr;
	    buf->flags |= (1L << GLIBTOP_NETINFO_ADDRESS);
	}

	strcpy (ifr.ifr_name, interface);
	if (!ioctl (skfd, SIOCGIFNETMASK, &ifr)) {
	    struct sockaddr_in addr =
		*(struct sockaddr_in *) &ifr.ifr_addr;
	    buf->subnet = addr.sin_addr.s_addr;
	    buf->flags |= (1L << GLIBTOP_NETINFO_SUBNET);
	}

	strcpy (ifr.ifr_name, interface);
	if (!ioctl (skfd, SIOCGIFMTU, &ifr)) {
	    buf->mtu = ifr.ifr_mtu;
	    buf->flags |= (1L << GLIBTOP_NETINFO_MTU);
	}

	close (skfd);
    }

    return 0;
}
