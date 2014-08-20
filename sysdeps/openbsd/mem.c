/* Copyright (C) 1998 Joshua Sled
   This file is part of LibGTop 1.0.

   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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
#include <glibtop/mem.h>

#include <glibtop_suid.h>

#include <sys/mount.h>
#include <sys/sysctl.h>
#include <uvm/uvm_extern.h>

static const unsigned long _glibtop_sysdeps_mem =
(1L << GLIBTOP_MEM_TOTAL)  + (1L << GLIBTOP_MEM_USED) +
(1L << GLIBTOP_MEM_FREE)   + (1L << GLIBTOP_MEM_SHARED) +
(1L << GLIBTOP_MEM_BUFFER) + (1L << GLIBTOP_MEM_CACHED) +
(1L << GLIBTOP_MEM_USER)   + (1L << GLIBTOP_MEM_LOCKED);

#ifndef LOG1024
#define LOG1024		10
#endif

/* these are for getting the memory statistics */
static int pageshift;		/* log base 2 of the pagesize */

/* define pagetok in terms of pageshift */
#define pagetok(size) ((size) << pageshift)

/* MIB array for sysctl */
static int uvmexp_mib  [] = { CTL_VM, VM_UVMEXP };
static int bcstats_mib [] = { CTL_VFS, VFS_GENERIC, VFS_BCACHESTAT };

/* Init function. */

void
_glibtop_init_mem_s (glibtop *server)
{
	register int pagesize;

	/* get the page size and calculate pageshift from it */
	pagesize = sysconf(_SC_PAGESIZE);
	pageshift = 0;
	while (pagesize > 1) {
		pageshift++;
		pagesize >>= 1;
	}

	/* we only need the amount of log(2)1024 for our conversion */
	pageshift -= LOG1024;

	server->sysdeps.mem = _glibtop_sysdeps_mem;
}

void
glibtop_get_mem_s (glibtop *server, glibtop_mem *buf)
{
	struct uvmexp uvmexp;
	struct bcachestats bcstats;
	size_t length;

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_MEM), 0);

	memset (buf, 0, sizeof (glibtop_mem));

	if (server->sysdeps.mem == 0)
		return;

	/* Get the data from sysctl */
	length = sizeof (uvmexp);
	if (sysctl (uvmexp_mib, 2, &uvmexp, &length, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (vm.uvmexp)");
		bzero(&uvmexp, sizeof(length));
		return;
	}

	length = sizeof (bcstats);
	if (sysctl (bcstats_mib, 3, &bcstats, &length, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (vfs.generic.bcstats)");
		bzero(&bcstats, sizeof(length));
		return;
	}

	/* convert memory stats to Kbytes */
	buf->total = (guint64) pagetok (uvmexp.npages) << LOG1024;
	buf->used  = (guint64) pagetok (uvmexp.npages - uvmexp.free) << LOG1024;
	buf->free  = (guint64) pagetok (uvmexp.free) << LOG1024;
	buf->locked = (guint64) pagetok (uvmexp.wired) << LOG1024;
	buf->shared = (guint64) pagetok (0 /* XXX */) << LOG1024;
	buf->cached = (guint64) pagetok (bcstats.numbufpages) << LOG1024;
	buf->buffer = 0;

	buf->user = buf->total - buf->free - buf->cached - buf->buffer;

	/* Set the values to return */
	buf->flags = _glibtop_sysdeps_mem;
}
