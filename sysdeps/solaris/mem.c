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
#include <glibtop/mem.h>

#include <assert.h>
#include <unistd.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_mem_os_sysconf =
(1L << GLIBTOP_MEM_TOTAL);
static const unsigned long _glibtop_sysdeps_mem_os_kstat =
(1L << GLIBTOP_MEM_FREE) + (1L << GLIBTOP_MEM_USED) +
(1L << GLIBTOP_MEM_LOCKED);
static const unsigned long _glibtop_sysdeps_mem_bunyip =
(1L << GLIBTOP_MEM_SHARED) + (1L << GLIBTOP_MEM_BUFFER) +
(1L << GLIBTOP_MEM_USER);

/* Init function. */

void
glibtop_init_mem_s (glibtop *server)
{
    server->sysdeps.mem = _glibtop_sysdeps_mem_os_sysconf +
        _glibtop_sysdeps_mem_os_kstat + _glibtop_sysdeps_mem_bunyip;
}

/* Provides information about memory usage. */

void
glibtop_get_mem_s (glibtop *server, glibtop_mem *buf)
{
    kstat_ctl_t *kc = server->machine.kc; 
    kstat_t *ksp;
    kstat_named_t *kn;
    int pagesize = server->machine.pagesize;

#ifndef KSTAT_DATA_UINT32
#define ui32 ul
#endif

    memset (buf, 0, sizeof (glibtop_mem));

    buf->total = (guint64)sysconf(_SC_PHYS_PAGES) << pagesize << 10;
    buf->flags = _glibtop_sysdeps_mem_os_sysconf;

    if(!kc)
        return;
    switch(kstat_chain_update(kc))
    {
        case -1: assert(0);  /* Debugging purposes, shouldn't happen */
	case 0:  break;
	default: glibtop_get_kstats(server);
    }

    if((ksp = server->machine.syspages) && kstat_read(kc, ksp, NULL) >= 0)
    {
	kn = (kstat_named_t *)kstat_data_lookup(ksp, "pagesfree");
	if(kn)
	{
#ifdef _LP64
	    buf->free = kn->value.ui64 << pagesize << 10;
#else
	    buf->free = kn->value.ui32 << pagesize << 10;
#endif
	    buf->used = buf->total - buf->free;
	}
	kn = (kstat_named_t *)kstat_data_lookup(ksp, "pageslocked");
	if(kn)
#ifdef _LP64
	    buf->locked = kn->value.ui64 << pagesize;
#else
	    buf->locked = kn->value.ui32 << pagesize;
#endif
	buf->flags += _glibtop_sysdeps_mem_os_kstat;
    }

    /* Bunyip module provides data in multiples of system page size */

    if((ksp = server->machine.bunyip) && kstat_read(kc, ksp, NULL) >= 0)
    {
        kn = (kstat_named_t *)kstat_data_lookup(ksp, "pages_anon");
	if(kn)
#ifdef _LP64
	    buf->user = kn->value.ui64 << pagesize << 10;
#else
	    buf->user = kn->value.ui32 << pagesize << 10;
#endif
	kn = (kstat_named_t *)kstat_data_lookup(ksp, "pages_exec");
	if(kn)
#ifdef _LP64
	    buf->shared = kn->value.ui64 << pagesize << 10;
#else
	    buf->shared = kn->value.ui32 << pagesize << 10;
#endif
	kn = (kstat_named_t *)kstat_data_lookup(ksp, "pages_vnode");
	if(kn)
#ifdef _LP64
	    buf->buffer = kn->value.ui64 << pagesize << 10;
#else
	    buf->buffer = kn->value.ui32 << pagesize << 10;
#endif
	buf->flags += _glibtop_sysdeps_mem_bunyip;
    }
}
