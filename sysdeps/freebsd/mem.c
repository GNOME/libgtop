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

/* nlist structure for kernel access */
static struct nlist nlst [] = {
	{ "_cnt" },
	{ "_bufspace" },
	{ 0 }
};

/* MIB array for sysctl */
static const int mib_length=2;
static const int mib [] = { CTL_VM, VM_METER };

/* Init function. */

void
glibtop_init_mem_p (glibtop *server)
{
	server->sysdeps.mem = _glibtop_sysdeps_mem;

	if (kvm_nlist (server->machine.kd, nlst) != 0)
		glibtop_error_io_r (server, "kvm_nlist");

}

void
glibtop_get_mem_p (glibtop *server, glibtop_mem *buf)
{
	struct vmtotal vmt;
	/* for sysctl(3) */
	size_t length_vmt;
	struct vmmeter vmm;
	int bufspace;

	glibtop_init_p (server, GLIBTOP_SYSDEPS_MEM, 0);
	
	memset (buf, 0, sizeof (glibtop_mem));

	/* Get the data from sysctl */
	length_vmt = sizeof (vmt);
	if (sysctl (mib, 2, &vmt, &length_vmt, NULL, 0))
		glibtop_error_io_r (server, "sysctl");
	
	/* Get the data from kvm_* */
	if (kvm_read (server->machine.kd, nlst[0].n_value,
		      &vmm, sizeof (vmm)))
		glibtop_error_io_r (server, "kvm_read (cnt)");

	if (kvm_read (server->machine.kd, nlst[1].n_value,
		      &bufspace, sizeof (bufspace)))
		glibtop_error_io_r (server, "kvm_read (bufspace)");
  
	/* Set the values to return */
	buf->flags = _glibtop_sysdeps_mem;
	/* total */
	buf->total = (u_int64_t) vmt.t_vm;
	/* used */
	buf->used = (u_int64_t) vmt.t_avm;
	/* free */
	buf->free = (u_int64_t) vmt.t_free;
	/* shared */
	buf->shared = (u_int64_t) vmt.t_vmshr;
	/* buffer */
	buf->buffer = (u_int64_t) bufspace;
	/* cached */
	buf->cached = (u_int64_t) (vmm.v_cache_count * vmm.v_page_size);

	/* user */
	buf->user = buf->total - buf->free - buf->shared - buf->buffer;
}
