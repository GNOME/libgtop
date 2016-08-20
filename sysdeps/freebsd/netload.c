/* Copyright (C) 1998-99 Martin Baulig
   Copyright (C) 2014 Gleb Smirnoff
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, October 1998.
   Contributed by Gleb Smirnoff <glebius@FreeBSD.org>, September 2014

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

#include <glibtop_suid.h>

#include <sys/ioctl.h>
#include <sys/sockio.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_media.h>
#include <ifaddrs.h>

static const unsigned long _glibtop_sysdeps_netload =
(1L << GLIBTOP_NETLOAD_IF_FLAGS) +
(1L << GLIBTOP_NETLOAD_MTU) +
(1L << GLIBTOP_NETLOAD_PACKETS_IN) +
(1L << GLIBTOP_NETLOAD_PACKETS_OUT) +
(1L << GLIBTOP_NETLOAD_PACKETS_TOTAL) +
(1L << GLIBTOP_NETLOAD_BYTES_IN) +
(1L << GLIBTOP_NETLOAD_BYTES_OUT) +
(1L << GLIBTOP_NETLOAD_BYTES_TOTAL) +
(1L << GLIBTOP_NETLOAD_ERRORS_IN) +
(1L << GLIBTOP_NETLOAD_ERRORS_OUT) +
(1L << GLIBTOP_NETLOAD_ERRORS_TOTAL) +
(1L << GLIBTOP_NETLOAD_COLLISIONS) +
(1L << GLIBTOP_NETLOAD_HWADDRESS);

static const unsigned long _glibtop_sysdeps_netload_data =
(1L << GLIBTOP_NETLOAD_SUBNET) +
(1L << GLIBTOP_NETLOAD_ADDRESS);

static const unsigned long _glibtop_sysdeps_netload6 =
(1L << GLIBTOP_NETLOAD_ADDRESS6) +
(1L << GLIBTOP_NETLOAD_PREFIX6) +
(1L << GLIBTOP_NETLOAD_SCOPE6);

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
        struct ifaddrs *ifap, *ifa;

        memset (buf, 0, sizeof (glibtop_netload));

        if (server->sysdeps.netload == 0)
                return;

        if (getifaddrs(&ifap) != 0) {
                glibtop_warn_io_r (server, "getifaddrs");
                return;
        }

#define IFA_STAT(s)     (((struct if_data *)ifa->ifa_data)->ifi_ ## s)

        for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
                if (strcmp (ifa->ifa_name, interface) != 0)
                        continue;

                switch (ifa->ifa_addr->sa_family) {
                case AF_LINK: {
                        struct sockaddr_dl *sdl;
                        struct ifmediareq ifmr;
                        struct ifreq ifr;
                        int s, flags;

                        s = socket(AF_INET, SOCK_DGRAM, 0);
                        if (s < 0) {
                                glibtop_warn_io_r(server, "socket(AF_INET)");
                                break;
                        }

                        memset(&ifmr, 0, sizeof(ifmr));
                        (void)strlcpy(ifmr.ifm_name, ifa->ifa_name,
                                sizeof(ifmr.ifm_name));
                        if (ioctl(s, SIOCGIFXMEDIA, (caddr_t)&ifmr) < 0 &&
                            ioctl(s, SIOCGIFMEDIA, (caddr_t)&ifmr) < 0) {
                                glibtop_warn_io_r(server, "ioctl(SIOCGIFMEDIA)");
                        } else {
                                if (IFM_TYPE (ifmr.ifm_current) & IFM_IEEE80211)
                                    buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_WIRELESS);
                                if (IFM_TYPE (ifmr.ifm_active) & IFM_IEEE80211)
                                    buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_WIRELESS);
                        }

                        memset(&ifr, 0, sizeof(ifr));
                        (void)strlcpy(ifr.ifr_name, ifa->ifa_name,
                                sizeof(ifr.ifr_name));
                        if (ioctl(s, SIOCGIFFLAGS, (caddr_t)&ifr) < 0) {
                                glibtop_warn_io_r(server, "ioctl(SIOCGIFFLAGS)");
                                close(s);
                                break;
                        }

                        close(s);

                        flags = (ifr.ifr_flags & 0xffff) | (ifr.ifr_flagshigh << 16);

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
                        if (flags & IFF_OACTIVE)
                                buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_OACTIVE);
                        if (flags & IFF_SIMPLEX)
                                buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_SIMPLEX);
                        if (flags & IFF_LINK0)
                                buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_LINK0);
                        if (flags & IFF_LINK1)
                                buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_LINK1);
                        if (flags & IFF_LINK2)
                                buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_LINK2);
                        if (flags & IFF_ALTPHYS)
                                buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_ALTPHYS);
                        if (flags & IFF_MULTICAST)
                                buf->if_flags |= (1L << GLIBTOP_IF_FLAGS_MULTICAST);

                        buf->packets_in = IFA_STAT(ipackets);
                        buf->packets_out = IFA_STAT(opackets);
                        buf->packets_total = buf->packets_in + buf->packets_out;

                        buf->bytes_in = IFA_STAT(ibytes);
                        buf->bytes_out = IFA_STAT(obytes);
                        buf->bytes_total = buf->bytes_in + buf->bytes_out;

                        buf->errors_in = IFA_STAT(ierrors);
                        buf->errors_out = IFA_STAT(oerrors);
                        buf->errors_total = buf->errors_in + buf->errors_out;

                        buf->collisions = IFA_STAT(collisions);

                        sdl = (struct sockaddr_dl *)(void *)ifa->ifa_addr;
                        memcpy(buf->hwaddress, LLADDR(sdl),
                                sizeof(buf->hwaddress));
                        buf->mtu = IFA_STAT(mtu);
                        buf->flags |= _glibtop_sysdeps_netload;
                        break;
                }
                case AF_INET: {
                        struct sockaddr_in *sin;

                        sin = (struct sockaddr_in *)(void *)ifa->ifa_addr;
                        buf->address = sin->sin_addr.s_addr;
                        sin = (struct sockaddr_in *)(void *)ifa->ifa_netmask;
                        buf->subnet = sin->sin_addr.s_addr & buf->address;
                        buf->flags |= _glibtop_sysdeps_netload_data;
                        break;
                }
                case AF_INET6: {
                        struct sockaddr_in6 *sin6;

                        sin6 = (struct sockaddr_in6 *)(void *)ifa->ifa_addr;
                        memcpy(buf->address6, &sin6->sin6_addr,
                                sizeof(buf->address6));
                        buf->scope6 = (guint8 )sin6->sin6_scope_id;
                        sin6 = (struct sockaddr_in6 *)(void *)ifa->ifa_netmask;
                        memcpy(buf->prefix6, &sin6->sin6_addr,
                                sizeof(buf->prefix6));
                        buf->flags |= _glibtop_sysdeps_netload6;
                        break;
                }
                } // switch() end
        }
        freeifaddrs(ifap);
}
