/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

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
#include <glibtop/global.h>
#include <glibtop/xmalloc.h>

#include <glibtop/open.h>
#include <glibtop/union.h>
#include <glibtop/backend.h>

glibtop_call_vector glibtop_backend_sysdeps_call_vector = {
#if GLIBTOP_SUID_CPU
    NULL,
#else
    glibtop_get_cpu_s,
#endif
#if GLIBTOP_SUID_MEM
    NULL,
#else
    glibtop_get_mem_s,
#endif
#if GLIBTOP_SUID_SWAP
    NULL,
#else
    glibtop_get_swap_s,
#endif
#if GLIBTOP_SUID_UPTIME
    NULL,
#else
    glibtop_get_uptime_s,
#endif
#if GLIBTOP_SUID_LOADAVG
    NULL,
#else
    glibtop_get_loadavg_s,
#endif
#if GLIBTOP_SUID_SHM_LIMITS
    NULL,
#else
    glibtop_get_shm_limits_s,
#endif
#if GLIBTOP_SUID_MSG_LIMITS
    NULL,
#else
    glibtop_get_msg_limits_s,
#endif
#if GLIBTOP_SUID_SEM_LIMITS
    NULL,
#else
    glibtop_get_sem_limits_s,
#endif
#if GLIBTOP_SUID_PROCLIST
    NULL,
#else
    glibtop_get_proclist_s,
#endif
#if GLIBTOP_SUID_PROC_STATE
    NULL,
#else
    glibtop_get_proc_state_s,
#endif
#if GLIBTOP_SUID_PROC_UID
    NULL,
#else
    glibtop_get_proc_uid_s,
#endif
#if GLIBTOP_SUID_PROC_MEM
    NULL,
#else
    glibtop_get_proc_mem_s,
#endif
#if GLIBTOP_SUID_PROC_TIME
    NULL,
#else
    glibtop_get_proc_time_s,
#endif
#if GLIBTOP_SUID_PROC_SIGNAL
    NULL,
#else
    glibtop_get_proc_signal_s,
#endif
#if GLIBTOP_SUID_PROC_KERNEL
    NULL,
#else
    glibtop_get_proc_kernel_s,
#endif
#if GLIBTOP_SUID_PROC_SEGMENT
    NULL,
#else
    glibtop_get_proc_segment_s,
#endif
#if GLIBTOP_SUID_PROC_ARGS
    NULL,
#else
    glibtop_get_proc_args_s,
#endif
#if GLIBTOP_SUID_PROC_MAP
    NULL,
#else
    glibtop_get_proc_map_s,
#endif
    glibtop_get_mountlist_s,
    glibtop_get_fsusage_s,
#if GLIBTOP_SUID_INTERFACE_NAMES
    NULL,
#else
    glibtop_get_interface_names_s,
#endif
#if GLIBTOP_SUID_NETINFO
    NULL,
#else
    glibtop_get_netinfo_s,
#endif
#if GLIBTOP_SUID_NETLOAD
    NULL,
#else
    glibtop_get_netload_s,
#endif
#if GLIBTOP_SUID_PPP
    NULL
#else
    glibtop_get_ppp_s
#endif
};
