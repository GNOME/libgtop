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

int
glibtop_init_netload_p (glibtop *server)
{
    server->sysdeps.netload = _glibtop_sysdeps_netload;

    if (kvm_nlist (server->_priv->machine.kd, nlst) < 0)
	glibtop_error_io_r (server, "kvm_nlist");

    return 0;
}

/* Provides Network statistics. */

int
glibtop_get_netload_p (glibtop *server, glibtop_netload *buf,
		       const char *interface, unsigned transport,
		       unsigned protocol)
{
    struct ifnet ifnet;
    u_long ifnetaddr, ifnetfound, ifaddraddr;
    struct sockaddr *sa;
    char tname [16];

    union {
	struct ifaddr ifa;
	struct in_ifaddr in;
    } ifaddr;
    
    glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_NETLOAD), 0);
	
    memset (buf, 0, sizeof (glibtop_netload));

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
