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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/netload.h>

#include "glibtop_private.h"

#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#if !defined (__GLIBC__) || __GNU_LIBRARY__ > 1
/* GNU LibC */
#include <net/if.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/if.h>
#elif defined (__GLIBC__) /* Libc 5 */
#include <linux/if.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#endif


#ifdef HAVE_IFADDRS_H
/* needed for IPV6 support */

#include <ifaddrs.h>

#ifndef IN6_IS_ADDR_GLOBAL
#define IN6_IS_ADDR_GLOBAL(a) \
   (((((__const uint8_t *) (a))[0] & 0xff) == 0x3f   \
     || (((__const uint8_t *) (a))[0] & 0xff) == 0x20))
#endif
#endif /* HAVE_IFADDRS_H */


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

static const unsigned long _glibtop_sysdeps_netload_6 =
(1L << GLIBTOP_NETLOAD_ADDRESS6) +
(1L << GLIBTOP_NETLOAD_PREFIX6) +
(1L << GLIBTOP_NETLOAD_SCOPE6);

/* Init function. */

void
_glibtop_init_netload_s (glibtop *server)
{
    server->sysdeps.netload = _glibtop_sysdeps_netload |
	_glibtop_sysdeps_netload_data |
	_glibtop_sysdeps_netload_bytes |
	_glibtop_sysdeps_netload_packets;
}


#ifdef HAVE_IFADDRS_H

static void get_ipv6(glibtop *server, glibtop_netload *buf,
		     const char *interface)
{
	struct ifaddrs *ifa0, *ifr6;

	if(getifaddrs (&ifa0) != 0)
	{
		glibtop_warn_r(server, "getifaddrs failed : %s", g_strerror(errno));
		return;
	}

	for (ifr6 = ifa0; ifr6; ifr6 = ifr6->ifa_next) {
		if (strcmp (ifr6->ifa_name, interface) == 0
		    && ifr6->ifa_addr != NULL
		    && ifr6->ifa_addr->sa_family == AF_INET6)
			break;
	}

	if(!ifr6) goto free_ipv6;

	memcpy(buf->address6,
	       &((struct sockaddr_in6 *) ifr6->ifa_addr)->sin6_addr,
	       16);

	memcpy(buf->prefix6,
	       &((struct sockaddr_in6 *) ifr6->ifa_netmask)->sin6_addr,
	       16);


	if (IN6_IS_ADDR_LINKLOCAL (buf->address6))
		buf->scope6 = GLIBTOP_IF_IN6_SCOPE_LINK;

	else if (IN6_IS_ADDR_SITELOCAL (buf->address6))
		buf->scope6 = GLIBTOP_IF_IN6_SCOPE_SITE;

	else if (IN6_IS_ADDR_GLOBAL (buf->address6)
		 || IN6_IS_ADDR_MC_ORGLOCAL (buf->address6)
		 || IN6_IS_ADDR_V4COMPAT (buf->address6)
		 || IN6_IS_ADDR_MULTICAST (buf->address6)
		 || IN6_IS_ADDR_UNSPECIFIED (buf->address6)
		)
		buf->scope6 = GLIBTOP_IF_IN6_SCOPE_GLOBAL;

	else if (IN6_IS_ADDR_LOOPBACK (buf->address6))
		buf->scope6 = GLIBTOP_IF_IN6_SCOPE_HOST;

	else
		buf->scope6 = GLIBTOP_IF_IN6_SCOPE_UNKNOWN;

	buf->flags |= _glibtop_sysdeps_netload_6;

 free_ipv6:
	freeifaddrs(ifa0);
}

#endif /* HAVE_IFADDRS_H */




static gboolean
read_value(glibtop *server,
	   const char *device,
	   const char *filename,
	   guint64 *value)
{
    char buffer[BUFSIZ];

    if(try_file_to_buffer(buffer,
    			  sizeof buffer,
			  "/sys/class/net/%s/statistics/%s",
			  device,
			  filename))
    {
	glibtop_warn_io_r(server,
			  "Failed to open \"/sys/class/net/%s/statistics/%s\"",
			  device,
			  filename);

	return FALSE;
    }

    *value = strtoull(buffer, NULL, 10);
    return TRUE;
}




static void
linux_2_6_stats(glibtop *server,
		   glibtop_netload *buf,
		   const char *dev)
{
    if(read_value(server, dev, "rx_packets", &buf->packets_in))
	buf->flags |= (1 << GLIBTOP_NETLOAD_PACKETS_IN);

    if(read_value(server, dev, "tx_packets", &buf->packets_out))
	buf->flags |= (1 << GLIBTOP_NETLOAD_PACKETS_OUT);

    buf->packets_total = buf->packets_in + buf->packets_out;
    buf->flags |= (1 << GLIBTOP_NETLOAD_PACKETS_TOTAL);


    if(read_value(server, dev, "rx_bytes", &buf->bytes_in))
	buf->flags |= (1 << GLIBTOP_NETLOAD_BYTES_IN);

    if(read_value(server, dev, "tx_bytes", &buf->bytes_out))
	buf->flags |= (1 << GLIBTOP_NETLOAD_BYTES_OUT);

    buf->bytes_total = buf->bytes_in + buf->bytes_out;
    buf->flags |= (1 << GLIBTOP_NETLOAD_BYTES_TOTAL);


    if(read_value(server, dev, "rx_errors", &buf->errors_in))
	buf->flags |= (1 << GLIBTOP_NETLOAD_ERRORS_IN);

    if(read_value(server, dev, "tx_errors", &buf->errors_out))
	buf->flags |= (1 << GLIBTOP_NETLOAD_ERRORS_OUT);

    buf->errors_total = buf->errors_in + buf->errors_out;
    buf->flags |= (1 << GLIBTOP_NETLOAD_ERRORS_TOTAL);


    if(read_value(server, dev, "collisions", &buf->collisions))
	buf->flags |= (1 << GLIBTOP_NETLOAD_COLLISIONS);
}




static void
linux_2_0_stats(glibtop *server,
		glibtop_netload *buf,
		const char *interface)
{
    FILE *f;
    char buffer[BUFSIZ];

    f = fopen ("/proc/net/ip_acct", "r");

    if (!f) {
	glibtop_warn_io_r (server,
			   "Failed to open \"/proc/net/ip_acct\"");
	return;
    }

    /* Skip over the header line. */
    fgets (buffer, BUFSIZ-1, f);

    while (fgets (buffer, BUFSIZ-1, f)) {
	unsigned long long flags, packets, bytes;
	char *p, *dev;

	/* Skip over the network thing. */
	dev = skip_token (buffer) + 1;
	p = skip_token (dev);
	*p++ = 0;

	if (strcmp (dev, interface))
	    continue;

	p = skip_token (p);

	flags   = strtoull (p, &p, 16);

	p = skip_multiple_token (p, 2);

	packets = strtoull (p, &p, 0);
	bytes   = strtoull (p, &p, 0);

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
}




static void
linux_2_4_stats(glibtop *server,
		glibtop_netload *buf,
		const char *interface)
{
    char buffer [BUFSIZ], *p;
    int have_bytes, fields;
    FILE *f;

    /* Ok, either IP accounting is not enabled in the kernel or
     * it was not enabled for the requested interface. */

    f = fopen ("/proc/net/dev", "r");
    if (!f) {
	glibtop_warn_io_r(server,
			  "Failed to open \"/proc/net/dev\"");
	return;
    }

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
    if (fields < 2) {
	fclose (f);
	return;
    }
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

	/* Ok, we've found the interface */

	/* Only read byte counts if we really have them. */

	if (have_bytes) {
	    buf->bytes_in = strtoull (p, &p, 0);
	    fields--;
	}

	buf->packets_in = strtoull (p, &p, 0);
	buf->errors_in  = strtoull (p, &p, 0);

	p = skip_multiple_token (p, fields);

	if (have_bytes)
	    buf->bytes_out = strtoull (p, &p, 0);

	buf->packets_out = strtoull (p, &p, 0);
	buf->errors_out  = strtoull (p, &p, 0);

	p = skip_multiple_token (p, 2);

	buf->collisions  = strtoull (p, &p, 0);

	/* Compute total valules. */

	buf->bytes_total = buf->bytes_in + buf->bytes_out;
	buf->packets_total = buf->packets_in + buf->packets_out;

	/* And now the flags. */

	buf->flags |= _glibtop_sysdeps_netload;
	buf->flags |= _glibtop_sysdeps_netload_packets;

	if (have_bytes)
	    buf->flags |= _glibtop_sysdeps_netload_bytes;

	break; /* finished */
    }

    fclose (f);
}




/* Provides network statistics. */

void
glibtop_get_netload_s (glibtop *server, glibtop_netload *buf,
		       const char *interface)
{
    int skfd;
    memset (buf, 0, sizeof (glibtop_netload));

    skfd = socket (AF_INET, SOCK_DGRAM, 0);
    if (skfd) {
	struct ifreq ifr;

	g_strlcpy (ifr.ifr_name, interface, sizeof ifr.ifr_name);
	if (!ioctl (skfd, SIOCGIFFLAGS, &ifr)) {
	    const unsigned long long flags = ifr.ifr_flags;

	    buf->flags |= (1L << GLIBTOP_NETLOAD_IF_FLAGS);

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
	    }

	g_strlcpy (ifr.ifr_name, interface, sizeof ifr.ifr_name);
	if (!ioctl(skfd, /* SIOCGIWNAME */ 0x8B01, &ifr))
		buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_WIRELESS);

	g_strlcpy (ifr.ifr_name, interface, sizeof ifr.ifr_name);
	if (!ioctl (skfd, SIOCGIFADDR, &ifr)) {
	    buf->address = ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr;
	    buf->flags |= (1L << GLIBTOP_NETLOAD_ADDRESS);
	}

	g_strlcpy (ifr.ifr_name, interface, sizeof ifr.ifr_name);
	if (!ioctl (skfd, SIOCGIFNETMASK, &ifr)) {
	    buf->subnet = ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr;
	    buf->flags |= (1L << GLIBTOP_NETLOAD_SUBNET);
	}

	g_strlcpy (ifr.ifr_name, interface, sizeof ifr.ifr_name);
	if (!ioctl (skfd, SIOCGIFMTU, &ifr)) {
	    buf->mtu = ifr.ifr_mtu;
	    buf->flags |= (1L << GLIBTOP_NETLOAD_MTU);
	}

	g_strlcpy (ifr.ifr_name, interface, sizeof ifr.ifr_name);
	if (!ioctl (skfd, SIOCGIFHWADDR, &ifr)) {
	    memcpy(buf->hwaddress, &ifr.ifr_hwaddr.sa_data, 8);
	    buf->flags |= (1L << GLIBTOP_NETLOAD_HWADDRESS);
	}

	close (skfd);
    }


    /*
     * Statistics
     */

    /* Linux 2.1.114 - don't know where exactly this was added, but
	 * recent kernels have byte count in /proc/net/dev so we don't
	 * need IP accounting.
	 */

    if (server->os_version_code < LINUX_VERSION_CODE(2, 1, 14)) {
	linux_2_0_stats(server, buf, interface);
    }
    else if (server->os_version_code > LINUX_VERSION_CODE(2, 6, 0)
	     && has_sysfs()) {
	linux_2_6_stats(server, buf, interface);
    }
    else {
	/* 2.4 and 2.6 without /sys (?$£Z¥!) */
	linux_2_4_stats(server, buf, interface);
    }

#ifdef HAVE_IFADDRS_H
    get_ipv6(server, buf, interface);
#endif /* HAVE_IFADDRS_H */
}



