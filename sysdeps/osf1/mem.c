/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/mem.h>

#include <sys/vm.h>

#include <mach.h>
#include <mach/mach_types.h>
#include <mach/vm_statistics.h>

static const unsigned long _glibtop_sysdeps_mem =
(1 << GLIBTOP_MEM_TOTAL) + (1 << GLIBTOP_MEM_USED) +
(1 << GLIBTOP_MEM_FREE);

/* Provides information about memory usage. */

void
glibtop_get_mem__r (glibtop *server, glibtop_mem *buf)
{
	vm_statistics_data_t vmstats;

	glibtop_init ();
	
	memset (buf, 0, sizeof (glibtop_mem));

	buf->flags = _glibtop_sysdeps_mem;
	
	(void) vm_statistics(task_self(), &vmstats);

	buf->free = vmstats.free_count   * vmstats.pagesize;
	buf->used = vmstats.active_count * vmstats.pagesize;

	/* [FIXME]: Is this correct? */
	
	buf->total = (vmstats.active_count + vmstats.inactive_count +
			vmstats.free_count + vmstats.wire_count) *
			vmstats.pagesize;
}
