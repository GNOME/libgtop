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

#include <glibtop_private.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/netload.h>

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

static const unsigned long _glibtop_sysdeps_netload = 
(1L << GLIBTOP_NETLOAD_BYTES_IN) +
(1L << GLIBTOP_NETLOAD_BYTES_OUT) +
(1L << GLIBTOP_NETLOAD_BYTES_TOTAL) +
(1L << GLIBTOP_NETLOAD_PACKETS_IN) +
(1L << GLIBTOP_NETLOAD_PACKETS_OUT) +
(1L << GLIBTOP_NETLOAD_PACKETS_TOTAL) +
(1L << GLIBTOP_NETLOAD_ERRORS_IN) +
(1L << GLIBTOP_NETLOAD_ERRORS_OUT) +
(1L << GLIBTOP_NETLOAD_ERRORS_TOTAL) +
(1L << GLIBTOP_NETLOAD_COLLISIONS);

/* Init function. */

int
glibtop_init_netload_s (glibtop *server)
{
    server->sysdeps.netload = _glibtop_sysdeps_netload;

    return 0;
}

/* Provides network statistics. */

int
glibtop_get_netload_s (glibtop *server, glibtop_netload *buf,
		       const char *interface, unsigned transport,
		       unsigned protocol)
{
    libgtop_netload_t netload;
    int retval;

    memset (buf, 0, sizeof (glibtop_netload));

    retval = glibtop_get_proc_data_netload_s (server, &netload, interface);
    if (retval)
	return retval;

    buf->bytes_in = netload.rx_bytes;
    buf->bytes_out = netload.tx_bytes;
    buf->bytes_total = buf->bytes_in + buf->bytes_out;

    buf->packets_in = netload.rx_packets;
    buf->packets_out = netload.tx_packets;
    buf->packets_total = buf->packets_in + buf->packets_out;

    buf->errors_in = netload.rx_errors;
    buf->errors_out = netload.tx_errors;
    buf->errors_total = buf->errors_in + buf->errors_out;

    buf->collisions = netload.collisions;

    buf->flags |= _glibtop_sysdeps_netload;

    return 0;
}
