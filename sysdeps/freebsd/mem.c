/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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

#include <glibtop_suid.h>

#include <sys/sysctl.h>
#include <sys/vmmeter.h>
#include <vm/vm_param.h>

static const unsigned long _glibtop_sysdeps_mem =
(1 << GLIBTOP_MEM_TOTAL) + (1 << GLIBTOP_MEM_USED) +
(1 << GLIBTOP_MEM_FREE) + (1 << GLIBTOP_MEM_SHARED) +
(1 << GLIBTOP_MEM_BUFFER) + (1 << GLIBTOP_MEM_CACHED) +
(1 << GLIBTOP_MEM_USER);

#ifndef LOG1024
#define LOG1024		10
#endif

/* these are for getting the memory statistics */
static int pageshift;		/* log base 2 of the pagesize */

/* define pagetok in terms of pageshift */
#define pagetok(size) ((size) << pageshift)

/* nlist structure for kernel access */
static struct nlist nlst [] = {
	{ "_cnt" },
	{ "_bufspace" },
	{ 0 }
};

/* MIB array for sysctl */
/* static int mib_length=2; */
static int mib [] = { CTL_VM, VM_METER };

/* Init function. */

void
glibtop_init_mem_p (glibtop *server)
{
	register int pagesize;

	server->sysdeps.mem = _glibtop_sysdeps_mem;

	if (kvm_nlist (server->machine.kd, nlst) != 0)
		glibtop_error_io_r (server, "kvm_nlist");

	/* get the page size with "getpagesize" and calculate pageshift
	 * from it */
	pagesize = getpagesize ();
	pageshift = 0;
	while (pagesize > 1) {
		pageshift++;
		pagesize >>= 1;
	}

	/* we only need the amount of log(2)1024 for our conversion */
	pageshift -= LOG1024;
}

void
glibtop_get_mem_p (glibtop *server, glibtop_mem *buf)
{
	struct vmtotal vmt;
	size_t length_vmt;
	struct vmmeter vmm;
	int bufspace;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_MEM), 0);
	
	memset (buf, 0, sizeof (glibtop_mem));

	/* [FIXME: On FreeBSD 2.2.6, sysctl () returns an incorrect
	 *         value for `vmt.vm'. We use some code from Unix top
	 *         here.] */

	/* Get the data from sysctl */
	length_vmt = sizeof (vmt);
	if (sysctl (mib, 2, &vmt, &length_vmt, NULL, 0))
		glibtop_error_io_r (server, "sysctl");
	
	/* Get the data from kvm_* */
	if (kvm_read (server->machine.kd, nlst[0].n_value,
		      &vmm, sizeof (vmm)) != sizeof (vmm))
		glibtop_error_io_r (server, "kvm_read (cnt)");

	if (kvm_read (server->machine.kd, nlst[1].n_value,
		      &bufspace, sizeof (bufspace)) != sizeof (bufspace))
		glibtop_error_io_r (server, "kvm_read (bufspace)");
  
	/* Set the values to return */
	buf->flags = _glibtop_sysdeps_mem;

	/* convert memory stats to Kbytes */

	buf->total = (u_int64_t) pagetok (vmm.v_page_count) << LOG1024;
	buf->used = (u_int64_t) pagetok (vmm.v_active_count) << LOG1024;
	buf->free = (u_int64_t) pagetok (vmm.v_free_count) << LOG1024;

	buf->cached = (u_int64_t) pagetok (vmm.v_cache_count) << LOG1024;
	buf->shared = (u_int64_t) pagetok (vmt.t_vmshr) << LOG1024;

	buf->buffer = (u_int64_t) bufspace;

#if 0
        if (swappgsin < 0) {
	    memory_stats[5] = 0;
	    memory_stats[6] = 0;
	} else {
	    memory_stats[5] = pagetok(((vmm.v_swappgsin - swappgsin)));
	    memory_stats[6] = pagetok(((vmm.v_swappgsout - swappgsout)));
	}
        swappgsin = vmm.v_swappgsin;
	swappgsout = vmm.v_swappgsout;
#endif

	/* user */
	buf->user = buf->total - buf->free - buf->shared - buf->buffer;
}
