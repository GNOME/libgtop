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
#include <glibtop/union.h>
#include <glibtop/sysdeps.h>
#include <glibtop/init_hooks.h>

const unsigned long glibtop_server_features =
GLIBTOP_SUID_CPU +
GLIBTOP_SUID_DISK +
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
GLIBTOP_SUID_PROC_OPEN_FILES +
GLIBTOP_SUID_NETLOAD +
GLIBTOP_SUID_NETLIST +
GLIBTOP_SUID_PROC_WD +
GLIBTOP_SUID_PROC_AFFINITY +
GLIBTOP_SUID_PPP +
GLIBTOP_SUID_PROC_IO;

const _glibtop_init_func_t _glibtop_init_hook_s [] = {
#if !GLIBTOP_SUID_CPU
	_glibtop_init_cpu_s,
#endif
#if !GLIBTOP_SUID_DISK
	_glibtop_init_disk_s,
#endif
#if !GLIBTOP_SUID_MEM
	_glibtop_init_mem_s,
#endif
#if !GLIBTOP_SUID_SWAP
	_glibtop_init_swap_s,
#endif
#if !GLIBTOP_SUID_UPTIME
	_glibtop_init_uptime_s,
#endif
#if !GLIBTOP_SUID_LOADAVG
	_glibtop_init_loadavg_s,
#endif
#if !GLIBTOP_SUID_SHM_LIMITS
	_glibtop_init_shm_limits_s,
#endif
#if !GLIBTOP_SUID_MSG_LIMITS
	_glibtop_init_msg_limits_s,
#endif
#if !GLIBTOP_SUID_SEM_LIMITS
	_glibtop_init_sem_limits_s,
#endif
#if !GLIBTOP_SUID_PROCLIST
	_glibtop_init_proclist_s,
#endif
#if !GLIBTOP_SUID_PROC_STATE
	_glibtop_init_proc_state_s,
#endif
#if !GLIBTOP_SUID_PROC_UID
	_glibtop_init_proc_uid_s,
#endif
#if !GLIBTOP_SUID_PROC_MEM
	_glibtop_init_proc_mem_s,
#endif
#if !GLIBTOP_SUID_PROC_TIME
	_glibtop_init_proc_time_s,
#endif
#if !GLIBTOP_SUID_PROC_SIGNAL
	_glibtop_init_proc_signal_s,
#endif
#if !GLIBTOP_SUID_PROC_KERNEL
	_glibtop_init_proc_kernel_s,
#endif
#if !GLIBTOP_SUID_PROC_SEGMENT
	_glibtop_init_proc_segment_s,
#endif
#if !GLIBTOP_SUID_PROC_ARGS
	_glibtop_init_proc_args_s,
#endif
#if !GLIBTOP_SUID_PROC_MAP
	_glibtop_init_proc_map_s,
#endif
#if !GLIBTOP_SUID_NETLOAD
	_glibtop_init_netload_s,
#endif
#if !GLIBTOP_SUID_NETLIST
	_glibtop_init_netlist_s,
#endif
#if !GLIBTOP_SUID_PROC_WD
	_glibtop_init_proc_wd_s,
#endif
#if !GLIBTOP_SUID_PROC_AFFINITY
	_glibtop_init_proc_affinity_s,
#endif
#if !GLIBTOP_SUID_PPP
	_glibtop_init_ppp_s,
#endif
#if !GLIBTOP_SUID_PROC_IO
	_glibtop_init_proc_io_s,
#endif
#if !GLIBTOP_SUID_PROC_OPEN_FILES
	_glibtop_init_proc_open_files_s,
#endif
	NULL
};

const _glibtop_init_func_t _glibtop_init_hook_p [] = {
#if GLIBTOP_SUID_CPU
	_glibtop_init_cpu_p,
#endif
#if GLIBTOP_SUID_DISK
	_glibtop_init_disk_p,
#endif
#if GLIBTOP_SUID_MEM
	_glibtop_init_mem_p,
#endif
#if GLIBTOP_SUID_SWAP
	_glibtop_init_swap_p,
#endif
#if GLIBTOP_SUID_UPTIME
	_glibtop_init_uptime_p,
#endif
#if GLIBTOP_SUID_LOADAVG
	_glibtop_init_loadavg_p,
#endif
#if GLIBTOP_SUID_SHM_LIMITS
	_glibtop_init_shm_limits_p,
#endif
#if GLIBTOP_SUID_MSG_LIMITS
	_glibtop_init_msg_limits_p,
#endif
#if GLIBTOP_SUID_SEM_LIMITS
	_glibtop_init_sem_limits_p,
#endif
#if GLIBTOP_SUID_PROCLIST
	_glibtop_init_proclist_p,
#endif
#if GLIBTOP_SUID_PROC_STATE
	_glibtop_init_proc_state_p,
#endif
#if GLIBTOP_SUID_PROC_UID
	_glibtop_init_proc_uid_p,
#endif
#if GLIBTOP_SUID_PROC_MEM
	_glibtop_init_proc_mem_p,
#endif
#if GLIBTOP_SUID_PROC_TIME
	_glibtop_init_proc_time_p,
#endif
#if GLIBTOP_SUID_PROC_SIGNAL
	_glibtop_init_proc_signal_p,
#endif
#if GLIBTOP_SUID_PROC_KERNEL
	_glibtop_init_proc_kernel_p,
#endif
#if GLIBTOP_SUID_PROC_SEGMENT
	_glibtop_init_proc_segment_p,
#endif
#if GLIBTOP_SUID_PROC_ARGS
	_glibtop_init_proc_args_p,
#endif
#if GLIBTOP_SUID_PROC_MAP
	_glibtop_init_proc_map_p,
#endif
#if GLIBTOP_SUID_NETLOAD
	_glibtop_init_netload_p,
#endif
#if GLIBTOP_SUID_NETLIST
	_glibtop_init_netlist_p,
#endif
#if GLIBTOP_SUID_PROC_WD
	_glibtop_init_proc_wd_p,
#endif
#if GLIBTOP_SUID_PROC_AFFINITY
	_glibtop_init_proc_affinity_p,
#endif
#if GLIBTOP_SUID_PPP
	_glibtop_init_ppp_p,
#endif
#if GLIBTOP_SUID_PROC_IO
	_glibtop_init_proc_io_p,
#endif
#if GLIBTOP_SUID_PROC_OPEN_FILES
	_glibtop_init_proc_open_files_p,
#endif
	NULL
};

/* Checks which features are implemented. */

void
glibtop_get_sysdeps_r (glibtop *server, glibtop_sysdeps *buf)
{
	*buf = server->sysdeps;
}
