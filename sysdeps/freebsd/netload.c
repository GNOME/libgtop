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
#include <glibtop/netload.h>

#include <glibtop_suid.h>

#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_types.h>

#ifdef HAVE_NET_IF_VAR_H
#include <net/if_var.h>
#endif

#include <netinet/in.h>
#include <netinet/in_var.h>

static const unsigned long _glibtop_sysdeps_netload =
(1L << GLIBTOP_NETLOAD_IF_FLAGS) +
(1L << GLIBTOP_NETLOAD_MTU) +
#if !defined(__bsdi__)
(1L << GLIBTOP_NETLOAD_SUBNET) +
#endif
(1L << GLIBTOP_NETLOAD_ADDRESS) +
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

/* nlist structure for kernel access */
static struct nlist nlst [] = {
    { "_ifnet" },
    { 0 }
};

/* Init function. */

void
glibtop_init_netload_p (glibtop *server)
{
    server->sysdeps.netload = _glibtop_sysdeps_netload;

    if (kvm_nlist (server->machine.kd, nlst) < 0)
	glibtop_error_io_r (server, "kvm_nlist");
}

/* Provides Network statistics. */

void
glibtop_get_netload_p (glibtop *server, glibtop_netload *buf,
		       const char *interface)
{
    struct ifnet ifnet;
    u_long ifnetaddr, ifnetfound, ifaddraddr;
    struct sockaddr *sa;
    char tname [16];
    char temp [16];

    union {
	struct ifaddr ifa;
	struct in_ifaddr in;
    } ifaddr;
    
    glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_NETLOAD), 0);
	
    memset (buf, 0, sizeof (glibtop_netload));

    if (kvm_read (server->machine.kd, nlst [0].n_value,
		  &ifnetaddr, sizeof (ifnetaddr)) != sizeof (ifnetaddr))
	glibtop_error_io_r (server, "kvm_read (ifnet)");
    
    ifaddraddr = 0;
    while (ifnetaddr || ifaddraddr) {
	struct sockaddr_in *sin;
	register char *cp;

	if (ifaddraddr == 0) {
	    ifnetfound = ifnetaddr;

	    if (kvm_read (server->machine.kd, ifnetaddr, &ifnet,
			  sizeof (ifnet)) != sizeof (ifnet))
		    glibtop_error_io_r (server, "kvm_read (ifnetaddr)");

#if (defined(__FreeBSD__) && (__FreeBSD_version < 501113)) || defined(__bsdi__)
	    if (kvm_read (server->machine.kd, (u_long) ifnet.if_name,
			  tname, 16) != 16)
		    glibtop_error_io_r (server, "kvm_read (if_name)");
	    snprintf(temp, (15 - strlen(tname)), "%d", ifnet.if_unit);
	    strcat(tname, temp);
	    tname [15] = 0;
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
	    if ((kvm_read (server->machine.kd, ifaddraddr, &ifaddr,
			   sizeof (ifaddr)) != sizeof (ifaddr)))
		glibtop_error_io_r (server, "kvm_read (ifaddraddr)");
	
#define CP(x) ((char *)(x))
	    cp = (CP(ifaddr.ifa.ifa_addr) - CP(ifaddraddr)) +
		CP(&ifaddr); sa = (struct sockaddr *)cp;
	
	    if (!strcmp (interface, tname) && (sa->sa_family == AF_INET)) {
		sin = (struct sockaddr_in *)sa;

		if (ifnet.if_flags & IFF_UP)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_UP);
		if (ifnet.if_flags & IFF_BROADCAST)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_BROADCAST);
		if (ifnet.if_flags & IFF_DEBUG)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_DEBUG);
		if (ifnet.if_flags & IFF_LOOPBACK)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_LOOPBACK);
		if (ifnet.if_flags & IFF_POINTOPOINT)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_POINTOPOINT);
		if (ifnet.if_flags & IFF_RUNNING)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_RUNNING);
		if (ifnet.if_flags & IFF_NOARP)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_NOARP);
		if (ifnet.if_flags & IFF_PROMISC)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_PROMISC);
		if (ifnet.if_flags & IFF_ALLMULTI)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_ALLMULTI);
		if (ifnet.if_flags & IFF_OACTIVE)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_OACTIVE);
		if (ifnet.if_flags & IFF_SIMPLEX)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_SIMPLEX);
		if (ifnet.if_flags & IFF_LINK0)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_LINK0);
		if (ifnet.if_flags & IFF_LINK1)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_LINK1);
		if (ifnet.if_flags & IFF_LINK2)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_LINK2);
#ifdef __FreeBSD__
		if (ifnet.if_flags & IFF_ALTPHYS)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_ALTPHYS);
#endif
		if (ifnet.if_flags & IFF_MULTICAST)
			buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_MULTICAST);

#if !defined(__bsdi__)
		/* Commenting out to "fix" #13345. */
		buf->subnet = htonl (ifaddr.in.ia_subnet);
#endif
		buf->address = sin->sin_addr.s_addr;

		buf->mtu = ifnet.if_mtu;

		buf->packets_in = ifnet.if_ipackets;
		buf->packets_out = ifnet.if_opackets;
		buf->packets_total = buf->packets_in + buf->packets_out;

		buf->bytes_in = ifnet.if_ibytes;
		buf->bytes_out = ifnet.if_obytes;
		buf->bytes_total = buf->bytes_in + buf->bytes_out;

		buf->errors_in = ifnet.if_ierrors;
		buf->errors_out = ifnet.if_oerrors;
		buf->errors_total = buf->errors_in + buf->errors_out;

		buf->collisions = ifnet.if_collisions;
		buf->flags = _glibtop_sysdeps_netload;
		return;
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
}
