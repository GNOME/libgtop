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

#define _GLIBTOP_IP_FW_ACCTIN	0x1000	/* Account incoming packets only. */
#define _GLIBTOP_IP_FW_ACCTOUT	0x2000	/* Account outgoing packets only. */

static const unsigned long _glibtop_sysdeps_netload = 
(1L << GLIBTOP_NETLOAD_ERRORS_IN) +
(1L << GLIBTOP_NETLOAD_ERRORS_OUT) +
(1L << GLIBTOP_NETLOAD_COLLISIONS);

static const unsigned long _glibtop_sysdeps_netload_data =
(1L << GLIBTOP_NETLOAD_ADDRESS) +
(1L << GLIBTOP_NETLOAD_SUBNET) +
(1L << GLIBTOP_NETLOAD_MTU);

static const unsigned long _glibtop_sysdeps_netload_bytes =
(1L << GLIBTOP_NETLOAD_BYTES_IN) +
(1L << GLIBTOP_NETLOAD_BYTES_OUT) +
(1L << GLIBTOP_NETLOAD_BYTES_TOTAL);

static const unsigned long _glibtop_sysdeps_netload_packets =
(1L << GLIBTOP_NETLOAD_PACKETS_IN) +
(1L << GLIBTOP_NETLOAD_PACKETS_OUT) +
(1L << GLIBTOP_NETLOAD_PACKETS_TOTAL);

static const unsigned long _glibtop_sysdeps_netload_total =
(1L << GLIBTOP_NETLOAD_PACKETS_TOTAL) +
(1L << GLIBTOP_NETLOAD_BYTES_TOTAL);

static const unsigned long _glibtop_sysdeps_netload_in =
(1L << GLIBTOP_NETLOAD_PACKETS_TOTAL) +
(1L << GLIBTOP_NETLOAD_BYTES_TOTAL) +
(1L << GLIBTOP_NETLOAD_PACKETS_IN) +
(1L << GLIBTOP_NETLOAD_BYTES_IN);

static const unsigned long _glibtop_sysdeps_netload_out =
(1L << GLIBTOP_NETLOAD_PACKETS_TOTAL) +
(1L << GLIBTOP_NETLOAD_BYTES_TOTAL) +
(1L << GLIBTOP_NETLOAD_PACKETS_OUT) +
(1L << GLIBTOP_NETLOAD_BYTES_OUT);

/* Init function. */

int
glibtop_init_netload_s (glibtop *server)
{
    server->sysdeps.netload = _glibtop_sysdeps_netload |
	_glibtop_sysdeps_netload_data |
	_glibtop_sysdeps_netload_bytes |
	_glibtop_sysdeps_netload_packets;
}

/* Provides network statistics. */

int
glibtop_get_netload_s (glibtop *server, glibtop_netload *buf,
		       const char *interface)
{
    char buffer [BUFSIZ], *p;
    int have_bytes, fields, skfd;
    FILE *f;

    memset (buf, 0, sizeof (glibtop_netload));

    skfd = socket (AF_INET, SOCK_DGRAM, 0);
    if (skfd) {
	struct ifreq ifr;
	unsigned flags;

	strcpy (ifr.ifr_name, interface);
	if (!ioctl (skfd, SIOCGIFFLAGS, &ifr)) {
	    buf->flags |= (1L << GLIBTOP_NETLOAD_IF_FLAGS);
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
	    buf->flags |= (1L << GLIBTOP_NETLOAD_ADDRESS);
	}

	strcpy (ifr.ifr_name, interface);
	if (!ioctl (skfd, SIOCGIFNETMASK, &ifr)) {
	    struct sockaddr_in addr =
		*(struct sockaddr_in *) &ifr.ifr_addr;
	    buf->subnet = addr.sin_addr.s_addr;
	    buf->flags |= (1L << GLIBTOP_NETLOAD_SUBNET);
	}

	strcpy (ifr.ifr_name, interface);
	if (!ioctl (skfd, SIOCGIFMTU, &ifr)) {
	    buf->mtu = ifr.ifr_mtu;
	    buf->flags |= (1L << GLIBTOP_NETLOAD_MTU);
	}

	close (skfd);
    }

    /* Linux 2.1.114 - don't know where exactly this was added, but
	 * recent kernels have byte count in /proc/net/dev so we don't
	 * need IP accounting.
	 */

    if (server->os_version_code < 131442) {

	/* If IP accounting is enabled in the kernel and it is
	 * enabled for the requested interface, we use it to
	 * get the data. In this case, we don't use /proc/net/dev
	 * to get errors and collisions.
	 */

	f = fopen ("/proc/net/ip_acct", "r");
	if (f) {
	    int success = 0;

	    /* Skip over the header line. */
	    fgets (buffer, BUFSIZ-1, f);

	    while (fgets (buffer, BUFSIZ-1, f)) {
		unsigned long flags, packets, bytes;
		char *p, *dev;

		/* Skip over the network thing. */		
		dev = skip_token (buffer) + 1;
		p = skip_token (dev);
		*p++ = 0;

		if (strcmp (dev, interface))
		    continue;

		success = 1;

		p = skip_token (p);

		flags   = strtoul (p, &p, 16);

		p = skip_multiple_token (p, 2);

		packets = strtoul (p, &p, 0);
		bytes   = strtoul (p, &p, 0);

		if (flags & _GLIBTOP_IP_FW_ACCTIN) {
		    /* Incoming packets only. */

		    buf->packets_total += packets;
		    buf->packets_in += packets;

		    buf->bytes_total += bytes;
		    buf->bytes_in += bytes;

		    buf->flags |= _glibtop_sysdeps_netload_in;

		} else if (flags & _GLIBTOP_IP_FW_ACCTOUT) {
		    /* Outgoing packets only. */

		    buf->packets_total += packets;
		    buf->packets_out += packets;

		    buf->bytes_total += bytes;
		    buf->bytes_out += bytes;

		    buf->flags |= _glibtop_sysdeps_netload_out;

		} else {
		    /* Only have total values. */

		    buf->packets_total += packets;
		    buf->bytes_total += bytes;

		    buf->flags |= _glibtop_sysdeps_netload_total;
		}
	    }

	    fclose (f);

	    if (success) return;
	}
    }

    /* Ok, either IP accounting is not enabled in the kernel or
	 * it was not enabled for the requested interface. */

    f = fopen ("/proc/net/dev", "r");
    if (!f) return;

	/* Skip over the header line. */
    fgets (buffer, BUFSIZ-1, f);
    fgets (buffer, BUFSIZ-1, f);

	/* Starting with 2.1.xx (don't know exactly which version)
	 * /proc/net/dev contains both byte and package counters. */

    p = strchr (buffer, '|');
    if (!p) {
	fclose (f);
	return;
    }

    /* Do we already have byte counters ? */
    have_bytes = strncmp (++p, "bytes", 5) == 0;

    /* Count remaining 'Receive' fields so we know where
	 * the first 'Transmit' field starts. */
	
    fields = 0;
    while (*p != '|') {
	if (!isspace (*p++)) continue;
	while (isspace (*p++)) ;
	fields++;
    }

    /* Should never happen. */
    if (fields < 2) return;
    fields--;

    while (fgets (buffer, BUFSIZ-1, f)) {
	char *p, *dev;

	dev = buffer;
	while (isspace (*dev)) dev++;

	p = strchr (dev, ':');
	if (!p) continue;
	*p++ = 0;

	/* If it's not a digit, then it's most likely an error
	 * message like 'No statistics available'. */
	while (isspace (*p)) p++;
	if (!isdigit (*p)) continue;

	if (strcmp (dev, interface))
	    continue;

	/* Only read byte counts if we really have them. */

	if (have_bytes) {
	    buf->bytes_in = strtoul (p, &p, 0);
	    fields--;
	}

	buf->packets_in = strtoul (p, &p, 0);
	buf->errors_in  = strtoul (p, &p, 0);

	p = skip_multiple_token (p, fields);

	if (have_bytes)
	    buf->bytes_out = strtoul (p, &p, 0);

	buf->packets_out = strtoul (p, &p, 0);
	buf->errors_out  = strtoul (p, &p, 0);

	p = skip_multiple_token (p, 2);

	buf->collisions  = strtoul (p, &p, 0);

	/* Compute total valules. */

	buf->bytes_total = buf->bytes_in + buf->bytes_out;
	buf->packets_total = buf->packets_in + buf->packets_out;

	/* And now the flags. */

	buf->flags |= _glibtop_sysdeps_netload;
	buf->flags |= _glibtop_sysdeps_netload_packets;

	if (have_bytes)
	    buf->flags |= _glibtop_sysdeps_netload_bytes;
    }

    fclose (f);
}
