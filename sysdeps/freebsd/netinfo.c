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
#include <glibtop/netinfo.h>

#include <glibtop_suid.h>

#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_types.h>

#ifdef HAVE_NET_IF_VAR_H
#include <net/if_var.h>
#endif

#include <netinet/in.h>
#include <netinet/in_var.h>

static const unsigned long _glibtop_sysdeps_netinfo =
(1L << GLIBTOP_NETINFO_IF_FLAGS) +
(1L << GLIBTOP_NETINFO_MTU) +
(1L << GLIBTOP_NETINFO_SUBNET) +
(1L << GLIBTOP_NETINFO_ADDRESS);

/* nlist structure for kernel access */
static struct nlist nlst [] = {
    { "_ifnet" },
    { 0 }
};

/* Init function. */

int
glibtop_init_netinfo_p (glibtop *server)
{
    server->sysdeps.netinfo = _glibtop_sysdeps_netinfo;

    if (kvm_nlist (server->_priv->machine.kd, nlst) != 0)
	glibtop_error_io_r (server, "kvm_nlist");

    return 0;
}

/* Provides Network statistics. */

int
glibtop_get_netinfo_p (glibtop *server, glibtop_netinfo *buf,
		       const char *interface, unsigned transport)
{
    struct ifnet ifnet;
    u_long ifnetaddr, ifnetfound, ifaddraddr;
    struct sockaddr *sa;
    char tname [16];

    union {
	struct ifaddr ifa;
	struct in_ifaddr in;
    } ifaddr;
    
    glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_NETINFO), 0);
	
    memset (buf, 0, sizeof (glibtop_netinfo));

    if (kvm_read (server->_priv->machine.kd, nlst [0].n_value,
		  &ifnetaddr, sizeof (ifnetaddr)) != sizeof (ifnetaddr))
	glibtop_error_io_r (server, "kvm_read (ifnet)");
    
    ifaddraddr = 0;
    while (ifnetaddr || ifaddraddr) {
	struct sockaddr_in *sin;
	register char *cp;

	if (ifaddraddr == 0) {
	    ifnetfound = ifnetaddr;

	    if (kvm_read (server->_priv->machine.kd, ifnetaddr, &ifnet,
			  sizeof (ifnet)) != sizeof (ifnet))
		glibtop_error_io_r (server, "kvm_read (ifnetaddr)");

#if defined(__FreeBSD__) || defined(__bsdi__)
	    if (kvm_read (server->_priv->machine.kd, (u_long) ifnet.if_name,
			  tname, 16) != 16)
		glibtop_error_io_r (server, "kvm_read (if_name)");
#else
	    strncpy (tname, ifnet.if_xname, 16);
	    tname [15] = 0;
#endif

#if defined(__FreeBSD__) && (__FreeBSD_version >= 300000)
	    ifaddraddr = (u_long) ifnet.if_addrhead.tqh_first;
#elif defined(__FreeBSD__) || defined(__bsdi__)
	    ifaddraddr = (u_long) ifnet.if_addrlist;
#else
	    ifaddraddr = (u_long) ifnet.if_addrlist.tqh_first;
#endif
	}

	if (ifaddraddr) {
	    if ((kvm_read (server->_priv->machine.kd, ifaddraddr, &ifaddr,
			   sizeof (ifaddr)) != sizeof (ifaddr)))
		glibtop_error_io_r (server, "kvm_read (ifaddraddr)");
	
#define CP(x) ((char *)(x))
	    cp = (CP(ifaddr.ifa.ifa_addr) - CP(ifaddraddr)) +
		CP(&ifaddr); sa = (struct sockaddr *)cp;
	
	    if (!strcmp (interface, tname) && (sa->sa_family == AF_INET)) {
		sin = (struct sockaddr_in *)sa;

		if (ifnet.if_flags & IFF_UP)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_UP;
		if (ifnet.if_flags & IFF_BROADCAST)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_BROADCAST;
		if (ifnet.if_flags & IFF_DEBUG)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_DEBUG;
		if (ifnet.if_flags & IFF_LOOPBACK)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_LOOPBACK;
		if (ifnet.if_flags & IFF_POINTOPOINT)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_POINTOPOINT;
		if (ifnet.if_flags & IFF_RUNNING)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_RUNNING;
		if (ifnet.if_flags & IFF_NOARP)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_NOARP;
		if (ifnet.if_flags & IFF_PROMISC)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_PROMISC;
		if (ifnet.if_flags & IFF_ALLMULTI)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_ALLMULTI;
		if (ifnet.if_flags & IFF_OACTIVE)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_OACTIVE;
		if (ifnet.if_flags & IFF_SIMPLEX)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_SIMPLEX;
		if (ifnet.if_flags & IFF_LINK0)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_LINK0;
		if (ifnet.if_flags & IFF_LINK1)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_LINK1;
		if (ifnet.if_flags & IFF_LINK2)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_LINK2;
#ifdef __FreeBSD__
		if (ifnet.if_flags & IFF_ALTPHYS)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_ALTPHYS;
#endif
		if (ifnet.if_flags & IFF_MULTICAST)
		    buf->if_flags |= GLIBTOP_IF_FLAGS_MULTICAST;

		buf->subnet = htonl (ifaddr.in.ia_subnet);
		buf->address = sin->sin_addr.s_addr;

		buf->mtu = ifnet.if_mtu;

		buf->flags = _glibtop_sysdeps_netinfo;
		return -1;
	    }

#if defined(__FreeBSD__) && (__FreeBSD_version >= 300000)
	    ifaddraddr = (u_long)ifaddr.ifa.ifa_link.tqe_next;
#elif defined(__FreeBSD__) || defined(__bsdi__)
	    ifaddraddr = (u_long)ifaddr.ifa.ifa_next;
#else
	    ifaddraddr = (u_long)ifaddr.ifa.ifa_list.tqe_next;
#endif
	}
	
#if defined(__FreeBSD__) && (__FreeBSD_version >= 300000)
	ifnetaddr = (u_long) ifnet.if_link.tqe_next;
#elif defined(__FreeBSD__) || defined(__bsdi__)
	ifnetaddr = (u_long) ifnet.if_next;
#else
	ifnetaddr = (u_long) ifnet.if_list.tqe_next;
#endif
    }

    return 0;
}
