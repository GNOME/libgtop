/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

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
#include <glibtop/swap.h>

#include <assert.h>
#include <sys/sysinfo.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_swap =
(1L << GLIBTOP_SWAP_TOTAL) + (1L << GLIBTOP_SWAP_USED) +
(1L << GLIBTOP_SWAP_FREE);

/* Init function. */

int
glibtop_init_swap_s (glibtop *server)
{
    server->sysdeps.swap = _glibtop_sysdeps_swap;
}

/* Provides information about swap usage. */

int
glibtop_get_swap_s (glibtop *server, glibtop_swap *buf)
{
    kstat_ctl_t *kc = server->machine.kc;
    kstat_t *ksp = server->machine.vminfo_kstat;
    u_int64_t swap_resv, swap_alloc, swap_avail, swap_free;
    vminfo_t vminfo;
    double rate;
    kid_t ret;

    memset (buf, 0, sizeof (glibtop_swap));

    if (!ksp) return;

    switch(kstat_chain_update(kc))
    {
        case -1: assert(0); /* Debugging, shouldn't happen */
	case 0:  break;
	default: glibtop_get_kstats(server);
    }
    ret = kstat_read (kc, ksp, &vminfo);

    if (ret == -1) {
	glibtop_warn_io_r (server, "kstat_read (vminfo)");
	return;
    }

    rate = (ksp->ks_snaptime - server->machine.vminfo_snaptime) / 1E+9;

    swap_resv = (vminfo.swap_resv - server->machine.vminfo.swap_resv) / rate;
    swap_alloc = (vminfo.swap_alloc - server->machine.vminfo.swap_alloc) / rate;
    swap_avail = (vminfo.swap_avail - server->machine.vminfo.swap_avail) / rate;
    swap_free = (vminfo.swap_free - server->machine.vminfo.swap_free) / rate;

    memcpy (&server->machine.vminfo, &vminfo, sizeof (vminfo_t));
    server->machine.vminfo_snaptime = ksp->ks_snaptime;

    buf->total = swap_resv + swap_avail;
    buf->used = swap_alloc;
    buf->free = buf->total - buf->used;

    buf->flags = _glibtop_sysdeps_swap;
}
