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
#include <glibtop/union.h>
#include <glibtop/sysdeps.h>

const unsigned long glibtop_server_features =
GLIBTOP_SUID_CPU +
GLIBTOP_SUID_MEM +
GLIBTOP_SUID_SWAP +
GLIBTOP_SUID_UPTIME +
GLIBTOP_SUID_LOADAVG +
GLIBTOP_SUID_SHM_LIMITS +
GLIBTOP_SUID_MSG_LIMITS +
GLIBTOP_SUID_SEM_LIMITS +
GLIBTOP_SUID_PROCLIST +
GLIBTOP_SUID_PROC_STATE +
GLIBTOP_SUID_PROC_UID +
GLIBTOP_SUID_PROC_MEM +
GLIBTOP_SUID_PROC_TIME +
GLIBTOP_SUID_PROC_SIGNAL +
GLIBTOP_SUID_PROC_KERNEL +
GLIBTOP_SUID_PROC_SEGMENT +
GLIBTOP_SUID_PROC_ARGS +
GLIBTOP_SUID_PROC_MAP +
GLIBTOP_SUID_NETLOAD +
GLIBTOP_SUID_PPP;

glibtop_init_func_t _glibtop_init_hook_s [] = {
#if !GLIBTOP_SUID_CPU
	glibtop_init_cpu_s,
#endif
#if !GLIBTOP_SUID_MEM
	glibtop_init_mem_s,
#endif
#if !GLIBTOP_SUID_SWAP
	glibtop_init_swap_s,
#endif
#if !GLIBTOP_SUID_UPTIME
	glibtop_init_uptime_s,
#endif
#if !GLIBTOP_SUID_LOADAVG
	glibtop_init_loadavg_s,
#endif
#if !GLIBTOP_SUID_SHM_LIMITS
	glibtop_init_shm_limits_s,
#endif
#if !GLIBTOP_SUID_MSG_LIMITS
	glibtop_init_msg_limits_s,
#endif
#if !GLIBTOP_SUID_SEM_LIMITS
	glibtop_init_sem_limits_s,
#endif
#if !GLIBTOP_SUID_PROCLIST
	glibtop_init_proclist_s,
#endif
#if !GLIBTOP_SUID_PROC_STATE
	glibtop_init_proc_state_s,
#endif
#if !GLIBTOP_SUID_PROC_UID
	glibtop_init_proc_uid_s,
#endif
#if !GLIBTOP_SUID_PROC_MEM
	glibtop_init_proc_mem_s,
#endif
#if !GLIBTOP_SUID_PROC_TIME
	glibtop_init_proc_time_s,
#endif
#if !GLIBTOP_SUID_PROC_SIGNAL
	glibtop_init_proc_signal_s,
#endif
#if !GLIBTOP_SUID_PROC_KERNEL
	glibtop_init_proc_kernel_s,
#endif
#if !GLIBTOP_SUID_PROC_SEGMENT
	glibtop_init_proc_segment_s,
#endif
#if !GLIBTOP_SUID_PROC_ARGS
	glibtop_init_proc_args_s,
#endif
#if !GLIBTOP_SUID_PROC_MAP
	glibtop_init_proc_map_s,
#endif
#if !GLIBTOP_SUID_NETLOAD
	glibtop_init_netload_s,
#endif
#if !GLIBTOP_SUID_PPP
	glibtop_init_ppp_s,
#endif
	NULL
};

/* Checks which features are implemented. */

void
glibtop_get_sysdeps_r (glibtop *server, glibtop_sysdeps *buf)
{
	memcpy (buf, &server->sysdeps, sizeof (glibtop_sysdeps));
}
