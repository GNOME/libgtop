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
#include <glibtop/open.h>
#include <glibtop/union.h>
#include <glibtop/sysdeps.h>

glibtop_init_func_t _glibtop_init_hook_p [] = {
#if GLIBTOP_SUID_CPU
	glibtop_init_cpu_p,
#endif
#if GLIBTOP_SUID_MEM
	glibtop_init_mem_p,
#endif
#if GLIBTOP_SUID_SWAP
	glibtop_init_swap_p,
#endif
#if GLIBTOP_SUID_UPTIME
	glibtop_init_uptime_p,
#endif
#if GLIBTOP_SUID_LOADAVG
	glibtop_init_loadavg_p,
#endif
#if GLIBTOP_SUID_SHM_LIMITS
	glibtop_init_shm_limits_p,
#endif
#if GLIBTOP_SUID_MSG_LIMITS
	glibtop_init_msg_limits_p,
#endif
#if GLIBTOP_SUID_SEM_LIMITS
	glibtop_init_sem_limits_p,
#endif
#if GLIBTOP_SUID_PROCLIST
	glibtop_init_proclist_p,
#endif
#if GLIBTOP_SUID_PROC_STATE
	glibtop_init_proc_state_p,
#endif
#if GLIBTOP_SUID_PROC_UID
	glibtop_init_proc_uid_p,
#endif
#if GLIBTOP_SUID_PROC_MEM
	glibtop_init_proc_mem_p,
#endif
#if GLIBTOP_SUID_PROC_TIME
	glibtop_init_proc_time_p,
#endif
#if GLIBTOP_SUID_PROC_SIGNAL
	glibtop_init_proc_signal_p,
#endif
#if GLIBTOP_SUID_PROC_KERNEL
	glibtop_init_proc_kernel_p,
#endif
#if GLIBTOP_SUID_PROC_SEGMENT
	glibtop_init_proc_segment_p,
#endif
#if GLIBTOP_SUID_PROC_ARGS
	glibtop_init_proc_args_p,
#endif
#if GLIBTOP_SUID_PROC_MAP
	glibtop_init_proc_map_p,
#endif
#if GLIBTOP_SUID_PPP
	glibtop_init_ppp_p,
#endif
	NULL
};

