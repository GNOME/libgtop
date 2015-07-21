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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/swap.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_swap =
(1L << GLIBTOP_SWAP_TOTAL) + (1L << GLIBTOP_SWAP_USED) +
(1L << GLIBTOP_SWAP_FREE);

static const unsigned long _glibtop_sysdeps_swap_paging =
(1L << GLIBTOP_SWAP_PAGEIN) + (1L << GLIBTOP_SWAP_PAGEOUT);

static int pagesize;

/* Init function. */

void
_glibtop_init_swap_p (glibtop *server)
{
	pagesize = getpagesize ();

	server->sysdeps.swap = _glibtop_sysdeps_swap |
		_glibtop_sysdeps_swap_paging;
}

/* Provides information about swap usage. */

void
glibtop_get_swap_p (glibtop *server, glibtop_swap *buf)
{
	size_t len;
	unsigned int swappgsout, swappgsin;
	int nswdev;
	struct kvm_swap kvmsw[16];

	memset (buf, 0, sizeof (glibtop_swap));
	memset (kvmsw, 0, sizeof (kvmsw));

	nswdev = kvm_getswapinfo (server->machine->kd, kvmsw, 16, 0);
	if (nswdev < 1) return;

	buf->flags = _glibtop_sysdeps_swap;

	/* See the man page for kvm_getswapinfo(3) to see why we can do this. */
	buf->used = kvmsw[nswdev].ksw_used;
	buf->total = kvmsw[nswdev].ksw_total;

	buf->total *= pagesize;
	buf->used *= pagesize;

	buf->free = buf->total - buf->used;

	len = sizeof (swappgsout);
	if (sysctlbyname ("vm.stats.vm.v_swappgsout", &swappgsout, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (vm.stats.vm.v_swappgsout)");
		return;
	}

	len = sizeof (swappgsin);
	if (sysctlbyname ("vm.stats.vm.v_swappgsin", &swappgsin, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (vm.stats.vm.v_swappgsin)");
		return;
	}

	buf->pagein  = (guint64) swappgsin;
	buf->pageout = (guint64) swappgsout;

	buf->flags |= _glibtop_sysdeps_swap_paging;
}
