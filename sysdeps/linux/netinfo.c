/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

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
#include <glibtop/xmalloc.h>
#include <glibtop/netinfo.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <sys/sysctl.h>

#ifndef HAVE_AFINET
#define HAVE_AFINET 1
#endif

#ifndef HAVE_AFINET6
#define HAVE_AFINET6 1
#endif

#ifdef HAVE_AFINET6

#define _PATH_PROCNET_IFINET6	"/proc/net/if_inet6"

#define IPV6_ADDR_LOOPBACK	0x0010U
#define IPV6_ADDR_LINKLOCAL	0x0020U
#define IPV6_ADDR_SITELOCAL	0x0040U

#define IPV6_ADDR_COMPATv4	0x0080U

#endif /* HAVE_AFINET6 */

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
(1L << GLIBTOP_NETINFO_TRANSPORT) +
(1L << GLIBTOP_NETINFO_IF_FLAGS) +
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

#ifdef HAVE_AFINET

static int
_netinfo_ipv4 (glibtop *server, glibtop_netinfo *buf,
	       const char *interface, glibtop_ifaddr *address)
{
    int skfd;

    skfd = socket (AF_INET, SOCK_DGRAM, 0);
    if (skfd) {
	struct ifreq ifr;
	unsigned flags;

	if (address) {
	    address->transport = GLIBTOP_TRANSPORT_IPV4;
	    address->flags |= (1L << GLIBTOP_IFADDR_TRANSPORT);

	    buf->transport = GLIBTOP_TRANSPORT_IPV4;
	    buf->flags |= (1L << GLIBTOP_NETINFO_TRANSPORT);
	}

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

	if (address) {
	    strcpy (ifr.ifr_name, interface);
	    if (!ioctl (skfd, SIOCGIFADDR, &ifr)) {
		struct sockaddr_in addr =
		    *(struct sockaddr_in *) &ifr.ifr_addr;

		address->addr_len = 4;
		memcpy (&address->address, &addr.sin_addr.s_addr, 4);

		address->flags |= (1L << GLIBTOP_IFADDR_ADDRESS);
		address->flags |= (1L << GLIBTOP_IFADDR_ADDR_LEN);
	    }

	    strcpy (ifr.ifr_name, interface);
	    if (!ioctl (skfd, SIOCGIFNETMASK, &ifr)) {
		struct sockaddr_in addr =
		    *(struct sockaddr_in *) &ifr.ifr_addr;

		memcpy (&address->subnet, &addr.sin_addr.s_addr, 4);
		address->flags |= (1L << GLIBTOP_IFADDR_SUBNET);
	    }
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

#endif /* HAVE_AFINET */

#ifdef HAVE_AFINET6

static int
_parse_ipv6_address (const char *addr_string, u_int8_t *dest)
{
    int i;

    if (strlen (addr_string) != 32)
	return -1;

    for (i = 0; i < 16; i++) {
	char c1, c2;
	int d1, d2;

	c1 = tolower (addr_string [(i<<1)]);
	c2 = tolower (addr_string [(i<<1)+1]);

	if ((c1 >= '0') && (c1 <= '9'))
	    d1 = c1-'0';
	else if ((c1 >= 'a') && (c1 <= 'f'))
	    d1 = c1-'a'+10;
	else
	    return -1;

	if ((c2 >= '0') && (c2 <= '9'))
	    d2 = c2-'0';
	else if ((c2 >= 'a') && (c2 <= 'f'))
	    d2 = c2-'a'+10;
	else
	    return -1;

	dest [i] = (d1 << 4) + d2;
    }

    return 0;
}

static int
_netinfo_ipv6 (glibtop *server, glibtop_netinfo *buf,
	       const char *interface, GPtrArray *parray)
{
    FILE *f;
    char addr6[40], devname[20];
    int plen, scope, dad_status, if_idx;

#ifdef HAVE_AFINET6
    /* get common things such as mtu and if_flags */
    _netinfo_ipv4 (server, buf, interface, NULL);
#endif

    buf->transport = GLIBTOP_TRANSPORT_IPV6;
    buf->flags |= (1L << GLIBTOP_NETINFO_TRANSPORT);

    f = fopen (_PATH_PROCNET_IFINET6, "r");

    if (f != NULL) {
	while (fscanf (f, "%64s %02x %02x %02x %02x %20s\n",
		       addr6, &if_idx, &plen, &scope, &dad_status,
		       devname) != EOF) {
	    glibtop_ifaddr address;

	    memset (&address, 0, sizeof (glibtop_ifaddr));

	    if (strcmp (devname, interface))
		continue;

	    if (!_parse_ipv6_address (addr6, address.address))
		address.flags |= (1L << GLIBTOP_IFADDR_ADDRESS);

	    address.transport = GLIBTOP_TRANSPORT_IPV6;
	    address.subnet = plen;

	    switch (scope) {
	    case 0:
		address.scope = GLIBTOP_IPV6_SCOPE_GLOBAL;
		break;
	    case IPV6_ADDR_LINKLOCAL:
		address.scope = GLIBTOP_IPV6_SCOPE_LINKLOCAL;
		break;
	    case IPV6_ADDR_SITELOCAL:
		address.scope = GLIBTOP_IPV6_SCOPE_SITELOCAL;
		break;
	    case IPV6_ADDR_COMPATv4:
		address.scope = GLIBTOP_IPV6_SCOPE_COMPATv4;
		break;
	    case IPV6_ADDR_LOOPBACK:
		address.scope = GLIBTOP_IPV6_SCOPE_LOOPBACK;
		break;
	    default:
		address.scope = GLIBTOP_IPV6_SCOPE_UNKNOWN;
		break;
	    }

	    address.addr_len = 8;

	    address.flags |= (1L << GLIBTOP_IFADDR_TRANSPORT) |
		(1L << GLIBTOP_IFADDR_ADDR_LEN) |
		(1L << GLIBTOP_IFADDR_SUBNET) |
		(1L << GLIBTOP_IFADDR_SCOPE);

	    g_ptr_array_add (parray, g_memdup (&address, sizeof (address)));
	}
    }

    fclose (f);

    return 0;
}

#endif /* HAVE_AFINET6 */

/* Provides network statistics. */

glibtop_ifaddr *
glibtop_get_netinfo_s (glibtop *server, glibtop_array *array,
		       glibtop_netinfo *buf, const char *interface,
		       u_int64_t transport)
{
    GPtrArray *parray;
    glibtop_ifaddr *retval = NULL;
    int i;

    memset (buf, 0, sizeof (glibtop_netinfo));

    if (strlen (interface) >= GLIBTOP_INTERFACE_LEN)
	return NULL;

    /* Assume IPv4 is the standard until IPv6 becomes more popular. */
    if (transport == GLIBTOP_TRANSPORT_DEFAULT)
	transport = GLIBTOP_TRANSPORT_ALL;

    /* Get information about all possible transport methods. */
    if (transport == GLIBTOP_TRANSPORT_ALL) {
	char buffer [BUFSIZ];
	struct stat statb;

	/* We may get a little speed improvement when we use sysctl ()
	 * directly, but the following piece of code seems very stable
	 * and reliable to me.
	 *
	 * The first stat() on "/proc/sys/net" is done to find out whether
	 * the kernel has sysctl support.
	 *
	 * January 23, 1999
	 * Martin
	 */

	if (!stat ("/proc/sys/net", &statb) && S_ISDIR (statb.st_mode)) {
	    buf->flags |= (1L << GLIBTOP_NETINFO_TRANSPORT);

	    sprintf (buffer, "/proc/sys/net/ipv4/conf/%s", interface);
	    if (!stat (buffer, &statb) && S_ISDIR (statb.st_mode))
		buf->transport |= GLIBTOP_TRANSPORT_IPV4;

	    sprintf (buffer, "/proc/sys/net/ipv6/conf/%s", interface);
	    if (!stat (buffer, &statb) && S_ISDIR (statb.st_mode))
		buf->transport |= GLIBTOP_TRANSPORT_IPV6;
	}
    }

    parray = g_ptr_array_new ();

#ifdef HAVE_AFINET
    if (transport & GLIBTOP_TRANSPORT_IPV4) {
	glibtop_ifaddr address;

	memset (&address, 0, sizeof (glibtop_ifaddr));

	_netinfo_ipv4 (server, buf, interface, &address);
	if (address.flags & (1L << GLIBTOP_IFADDR_ADDRESS))
	    g_ptr_array_add (parray, g_memdup (&address, sizeof (address)));
    }
#endif /* HAVE_AFINET */

#ifdef HAVE_AFINET6
    if (transport & GLIBTOP_TRANSPORT_IPV6)
	_netinfo_ipv6 (server, buf, interface, parray);
#endif /* HAVE_AFINET6 */

    if (!parray->len) {
	g_ptr_array_free (parray, TRUE);
	return NULL;
    }

    retval = glibtop_calloc_r (server, parray->len, sizeof (glibtop_ifaddr));

    for (i = 0; i < parray->len; i++)
	retval [i] = *(glibtop_ifaddr *) parray->pdata [i];

    array->number = parray->len;
    array->size = sizeof (glibtop_ifaddr);
    array->total = array->number * array->size;

    g_ptr_array_free (parray, TRUE);

    return retval;
}
