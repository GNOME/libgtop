/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, March 1999.

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

#ifndef __GLIBTOP_PRIVATE_H__
#define __GLIBTOP_PRIVATE_H__

#include <glibtop.h>
#include <glibtop/error.h>

#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>

#undef LIBGTOP_VERSION
#include <linux/libgtop.h>

BEGIN_LIBGTOP_DECLS

#include "backend-kernel.h"

int
glibtop_get_proc_data_stat_k (glibtop *server, libgtop_stat_t *stat);

int
glibtop_get_proc_data_mem_k (glibtop *server, libgtop_mem_t *mem);

int
glibtop_get_proc_data_swap_k (glibtop *server, libgtop_swap_t *swap);

int
glibtop_get_proc_data_proclist_k (glibtop *server,
				  libgtop_proclist_t *proclist,
				  u_int64_t which, u_int64_t arg);

int
glibtop_get_proc_data_proc_state_k (glibtop *server,
				    libgtop_proc_state_t *proc_state,
				    pid_t pid);

int
glibtop_get_proc_data_proc_mem_k (glibtop *server,
				  libgtop_proc_mem_t *proc_mem,
				  pid_t pid);

int
glibtop_get_proc_data_proc_signal_k (glibtop *server,
				     libgtop_proc_signal_t *proc_signal,
				     pid_t pid);

int
glibtop_get_proc_data_proc_kernel_k (glibtop *server,
				     libgtop_proc_kernel_t *proc_kernel,
				     pid_t pid);

int
glibtop_get_proc_data_proc_args_k (glibtop *server, pid_t pid,
				   char *result, size_t max_len);

int
glibtop_get_proc_data_proc_maps_k (glibtop *server, pid_t pid,
				   libgtop_proc_maps_t *result,
				   size_t max_len);

int
glibtop_get_proc_data_netload_k (glibtop *server,
				 libgtop_netload_t *netload,
				 const char *device);

END_LIBGTOP_DECLS

#endif /* __GLIBTOP_PRIVATE_H__ */
