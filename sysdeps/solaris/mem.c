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
#include <glibtop/mem.h>

#include <assert.h>
#include <unistd.h>

#include <glibtop_private.h>

/* Init function. */

void
_glibtop_init_mem_s (glibtop *server)
{
    server->sysdeps.mem = (1L << GLIBTOP_MEM_TOTAL)
	| (1L << GLIBTOP_MEM_FREE)
	| (1L << GLIBTOP_MEM_USED)
	| (1L << GLIBTOP_MEM_LOCKED)
	| (1L << GLIBTOP_MEM_SHARED)
	| (1L << GLIBTOP_MEM_BUFFER)
	| (1L << GLIBTOP_MEM_USER);
}

/* Provides information about memory usage. */

void
glibtop_get_mem_s (glibtop *server, glibtop_mem *buf)
{
    kstat_ctl_t * const kc = server->machine->kc;
    kstat_t *ksp;
    kstat_named_t *kn;

#undef PAGESIZE
#define PAGESIZE (server->machine->pagesize)
#define PAGESHIFT (PAGESIZE + 10)

#ifdef _LP64
#define KN_VALUE kn->value.ui64
#elif !defined(KSTAT_DATA_UINT32)
#define KN_VALUE kn->value.ul
#else
#define KN_VALUE kn->value.ui32
#endif

    memset (buf, 0, sizeof (glibtop_mem));

    buf->total = (guint64) sysconf(_SC_PHYS_PAGES) << PAGESHIFT;
    buf->flags = (1 << GLIBTOP_MEM_TOTAL);

    if(!kc)
	return;

    switch(kstat_chain_update(kc))
    {
	case -1: assert(0);  /* Debugging purposes, shouldn't happen */
	case 0:  break;
	default: glibtop_get_kstats(server);
    }

    if((ksp = server->machine->syspages) && kstat_read(kc, ksp, NULL) >= 0)
    {
	kn = kstat_data_lookup(ksp, "pagesfree");
	if(kn)
	{
	    buf->free = (KN_VALUE << PAGESHIFT);
	    buf->used = buf->total - buf->free;
	    buf->flags |= (1 << GLIBTOP_MEM_FREE);
	    buf->flags |= (1 << GLIBTOP_MEM_USED);
	}

	kn = kstat_data_lookup(ksp, "pageslocked");
	if(kn)
	{
	    buf->locked = (KN_VALUE << PAGESIZE);
	    buf->flags |= (1 << GLIBTOP_MEM_LOCKED);
	}
    }

    /* Bunyip module provides data in multiples of system page size */

    if((ksp = server->machine->bunyip) && kstat_read(kc, ksp, NULL) >= 0)
    {
	kn = kstat_data_lookup(ksp, "pages_exec");
	if(kn)
	{
	    buf->shared = (KN_VALUE << PAGESHIFT);
	    buf->flags |= (1 << GLIBTOP_MEM_SHARED);
	}

	kn = kstat_data_lookup(ksp, "pages_vnode");
	if(kn)
	{
	    buf->buffer = (KN_VALUE << PAGESHIFT);
	    buf->flags |= (1 << GLIBTOP_MEM_BUFFER);
	}

	kn = kstat_data_lookup(ksp, "pages_anon");
	if(kn)
	{
	    buf->user = (KN_VALUE << PAGESHIFT);
	    buf->flags |= (1 << GLIBTOP_MEM_USER);
	}
	else
	    goto user_old_way;
    }
    else /* Bunyip is not available, let's compute buf->user the old way */
    {
    user_old_way:
	buf->user = buf->total - buf->free - buf->cached - buf->buffer;
	buf->flags |= (1 << GLIBTOP_MEM_USER);
    }
}
