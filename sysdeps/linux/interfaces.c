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
#include <glibtop/interfaces.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>

static const unsigned long _glibtop_sysdeps_interface_names = 
(1L << GLIBTOP_INTERFACE_NAMES_NUMBER) +
(1L << GLIBTOP_INTERFACE_NAMES_SIZE);

/* Init function. */

int
glibtop_init_interface_names_s (glibtop *server)
{
    server->sysdeps.interface_names = _glibtop_sysdeps_interface_names;

    return 0;
}

/* Provides network statistics. */

glibtop_interface *
glibtop_get_interface_names_s (glibtop *server, glibtop_array *array,
			       u_int64_t interface, u_int64_t number,
			       u_int64_t instance, u_int64_t strategy)
{
    GPtrArray *parray;
    u_int64_t strategy_flags;
    glibtop_interface *retval = NULL;
    int skfd, i, n, numreqs = 30;
    struct ifconf ifc;
    struct ifreq *ifr;

    glibtop_init_s (&server, GLIBTOP_SYSDEPS_INTERFACE_NAMES, 0);

    memset (array, 0, sizeof (glibtop_array));

    strategy_flags = strategy & GLIBTOP_STRATEGY_FLAGS_MASK;
    strategy &= ~GLIBTOP_STRATEGY_FLAGS_MASK;

    parray = g_ptr_array_new ();

    skfd = socket (AF_INET, SOCK_DGRAM, 0);
    if (!skfd) {
	glibtop_error_io_r (server, "socket (AF_INET, SOCK_DGRAM, 0)");
	return NULL;
    }

    ifc.ifc_buf = NULL;
    for (;;) {
	ifc.ifc_len = sizeof (struct ifreq) * numreqs;
	ifc.ifc_buf = glibtop_realloc_r (server, ifc.ifc_buf, ifc.ifc_len);

	if (ioctl (skfd, SIOCGIFCONF, &ifc) < 0) {
	    glibtop_error_io_r (server, "SIOCGIFCONF");
	    glibtop_free_r (server, ifc.ifc_buf);
	    return NULL;
	}
	if (ifc.ifc_len == sizeof (struct ifreq) * numreqs) {
	    /* assume it overflowed and try again */
	    numreqs += 10;
	    continue;
	}
	break;
    }

    ifr = ifc.ifc_req;
    for (n = 0; n < ifc.ifc_len; n += sizeof(struct ifreq), ifr++) {
	glibtop_interface *interface_ptr;

	if (strchr (ifr->ifr_name, ':')) {
	    /* This is a logical interface */
	    if (!(strategy_flags & GLIBTOP_INTERFACES_INCLUDE_LOGICAL))
		continue;
	}

	interface_ptr = g_new0 (glibtop_interface, 1);

	strcpy (interface_ptr->name, ifr->ifr_name);

	g_ptr_array_add (parray, interface_ptr);
    }

    glibtop_free_r (server, ifc.ifc_buf);

    close (skfd);

    retval = glibtop_calloc_r (server, parray->len, sizeof (glibtop_interface));

    for (i = 0; i < parray->len; i++)
	retval [i] = *(glibtop_interface *) parray->pdata [i];

    array->number = parray->len;
    array->size = sizeof (glibtop_interface);
    array->total = array->number * array->size;

    g_ptr_array_free (parray, TRUE);

    return retval;
}
