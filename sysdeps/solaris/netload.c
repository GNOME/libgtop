/* Copyright (C) 2007 Henry Zhang
   This file is part of LibGTop 2.20.

   Contributed by Henry Zhang <hua.zhang@sun.com>, October 2007. 

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
#include <kstat.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/sockio.h>

#include <net/if.h>


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
	_glibtop_sysdeps_netload_bytes |
	_glibtop_sysdeps_netload_packets;
}

static int
solaris_stats(glibtop *server,
		glibtop_netload *buf,
		const char *interface)
{
	char	*name = interface;
	char	*module;
	char	*ptr;
    	kstat_ctl_t * const kctl = server->machine->kc;
	kstat_t			*ksp;
	kstat_named_t		*kdata;
	int have_bytes = 1;

	/*
	 *  chop off the trailing interface
	 */
	module = strdup( name );
	ptr = module + strlen( module ) - 1;
	while( (ptr > module) && isdigit( (int) *ptr ) ) {
		*ptr = '\0';
		ptr--;
	}

	/*
	 *  get a kstat handle and update the user's kstat chain
	 */
	if( kctl == NULL ){
		glibtop_warn_io_r (server, "kstat_open ()");
		free( module );
		return( 0 );
	}
	while( kstat_chain_update( kctl ) != 0 )
		;

	/*
	 *  traverse the kstat chain
	 *  to find the appropriate statistics
	 */
	if( (ksp = kstat_lookup( kctl,
			module, 0, name )) == NULL ) {
		free( module );
		return( 0 );
	}
	if( kstat_read( kctl, ksp, NULL ) == -1 ) {
		free( module );
		return( 0 );
	}
	free( module );

	/*
	 *  lookup & store the data
	 */
	kdata = (kstat_named_t *) kstat_data_lookup( ksp, "ipackets" );
	if( kdata != NULL ) {
		buf->packets_in= kdata->value.ul;
	}
	kdata = (kstat_named_t *) kstat_data_lookup( ksp, "opackets" );
	if( kdata != NULL ) {
		buf->packets_out = kdata->value.ul;
	}
	kdata = (kstat_named_t *) kstat_data_lookup( ksp, "rbytes" );
	if( kdata != NULL ) {
		buf->bytes_in =kdata->value.ul;
	}
	kdata = (kstat_named_t *) kstat_data_lookup( ksp, "obytes" );
	if( kdata != NULL ) {
		buf->bytes_out =kdata->value.ul;
	}
	kdata = (kstat_named_t *) kstat_data_lookup( ksp, "ierrors" );
	if( kdata != NULL ) {
		buf->errors_in = kdata->value.ul;
	}
	kdata = (kstat_named_t *) kstat_data_lookup( ksp, "oerrors" );
	if( kdata != NULL ) {
		buf->errors_out = kdata->value.ul;
	}
	kdata = (kstat_named_t *) kstat_data_lookup( ksp, "collisions" );
	if( kdata != NULL ) {
		buf->collisions = kdata->value.ul;
	}
	
	/* Compute total valules. */

	buf->bytes_total = buf->bytes_in + buf->bytes_out;
	buf->packets_total = buf->packets_in + buf->packets_out;
	buf->errors_total = buf->errors_in + buf->errors_out;
	/* And now the flags. */
	buf->flags |= _glibtop_sysdeps_netload;
	buf->flags |= _glibtop_sysdeps_netload_bytes;
	buf->flags |= _glibtop_sysdeps_netload_packets;
	    
	/* finished */
}

/* Provides network statistics. */

void
glibtop_get_netload_s (glibtop *server, glibtop_netload *buf,
		       const char *interface)
{
   int skfd;
    memset (buf, 0, sizeof (glibtop_netload));

    /* set flag */
    skfd = socket (PF_INET, SOCK_DGRAM, 0);
    if (skfd) {
	struct ifreq ifr;

	g_strlcpy (ifr.ifr_name, interface, sizeof ifr.ifr_name);
	if (ioctl (skfd, SIOCGIFFLAGS, &ifr) >= 0) {
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
	if (!ioctl (skfd, SIOCGIFADDR, &ifr)) {
	    buf->address = ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr;
	    buf->flags |= (1L << GLIBTOP_NETLOAD_ADDRESS);
	}

	g_strlcpy (ifr.ifr_name, interface, sizeof ifr.ifr_name);
	if (!ioctl (skfd, SIOCGIFNETMASK, &ifr)) {
	    buf->subnet = ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr;
	    buf->flags |= (1L << GLIBTOP_NETLOAD_SUBNET);
	}
	close (skfd);
    }

    /*
     * Statistics
     */
     
    solaris_stats(server, buf, interface);

}
