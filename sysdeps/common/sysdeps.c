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
#include <glibtop/union.h>
#include <glibtop/sysdeps.h>

const unsigned long glibtop_server_features =
(1 << GLIBTOP_SUID_CPU) + (1 << GLIBTOP_SUID_MEM) +
(1 << GLIBTOP_SUID_SWAP) + (1 << GLIBTOP_SUID_UPTIME) +
(1 << GLIBTOP_SUID_LOADAVG) + (1 << GLIBTOP_SUID_SHM_LIMITS) +
(1 << GLIBTOP_SUID_MSG_LIMITS) + (1 << GLIBTOP_SUID_SEM_LIMITS) +
(1 << GLIBTOP_SUID_PROCLIST) + (1 << GLIBTOP_SUID_PROC_STATE) +
(1 << GLIBTOP_SUID_PROC_UID) + (1 << GLIBTOP_SUID_PROC_MEM) +
(1 << GLIBTOP_SUID_PROC_TIME) + (1 << GLIBTOP_SUID_PROC_SIGNAL) +
(1 << GLIBTOP_SUID_PROC_KERNEL) + (1 << GLIBTOP_SUID_PROC_SEGMENT);

/* Checks which features are implemented. */

void
glibtop_get_sysdeps__r (glibtop *server, glibtop_sysdeps *buf)
{
	glibtop_union data;

	memset (buf, 0, sizeof (glibtop_sysdeps));

	/* Call all system dependent functions to check which values
	 * they return. */

	glibtop_get_cpu__r (server, &data.cpu);
	buf->cpu = data.cpu.flags;

	glibtop_get_mem__r (server, &data.mem);
	buf->mem = data.mem.flags;
	
	glibtop_get_swap__r (server, &data.swap);
	buf->swap = data.swap.flags;

	glibtop_get_uptime__r (server, &data.uptime);
	buf->uptime = data.uptime.flags;

	glibtop_get_loadavg__r (server, &data.loadavg);
	buf->loadavg = data.loadavg.flags;

	glibtop_get_shm_limits__r (server, &data.shm_limits);
	buf->shm_limits = data.shm_limits.flags;

	glibtop_get_msg_limits__r (server, &data.msg_limits);
	buf->msg_limits = data.msg_limits.flags;

	glibtop_get_sem_limits__r (server, &data.sem_limits);
	buf->sem_limits = data.sem_limits.flags;

	glibtop_get_proclist__r (server, &data.proclist);
	buf->proclist = data.proclist.flags;

	glibtop_get_proc_state__r (server, &data.proc_state, 0);
	buf->proc_state = data.proc_state.flags;

	glibtop_get_proc_uid__r (server, &data.proc_uid, 0);
	buf->proc_uid = data.proc_uid.flags;

	glibtop_get_proc_mem__r (server, &data.proc_mem, 0);
	buf->proc_mem = data.proc_mem.flags;

	glibtop_get_proc_time__r (server, &data.proc_time, 0);
	buf->proc_time = data.proc_time.flags;

	glibtop_get_proc_kernel__r (server, &data.proc_kernel, 0);
	buf->proc_kernel = data.proc_kernel.flags;

	glibtop_get_proc_segment__r (server, &data.proc_segment, 0);
	buf->proc_segment = data.proc_segment.flags;
}
