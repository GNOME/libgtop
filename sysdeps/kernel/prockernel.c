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
#include <glibtop/prockernel.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proc_kernel =
(1 << GLIBTOP_PROC_KERNEL_MIN_FLT) +
(1 << GLIBTOP_PROC_KERNEL_MAJ_FLT) +
(1 << GLIBTOP_PROC_KERNEL_CMIN_FLT) +
(1 << GLIBTOP_PROC_KERNEL_CMAJ_FLT) +
(1 << GLIBTOP_PROC_KERNEL_KSTK_ESP) +
(1 << GLIBTOP_PROC_KERNEL_KSTK_EIP);

static const unsigned long _glibtop_sysdeps_proc_kernel_kernel =
(1 << GLIBTOP_PROC_KERNEL_NWCHAN);

/* Init function. */

void
glibtop_init_proc_kernel_s (glibtop *server)
{
    server->sysdeps.proc_kernel = _glibtop_sysdeps_proc_kernel |
	_glibtop_sysdeps_proc_kernel_kernel;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_kernel_s (glibtop *server, glibtop_proc_kernel *buf,
			   pid_t pid)
{
    libgtop_proc_state_t proc_state;
    libgtop_proc_kernel_t proc_kernel;

    memset (buf, 0, sizeof (glibtop_proc_kernel));

    if (glibtop_get_proc_data_proc_state_s (server, &proc_state, pid))
	return;

    buf->min_flt = proc_state.min_flt;
    buf->maj_flt = proc_state.maj_flt;
    buf->cmin_flt = proc_state.cmin_flt;
    buf->cmaj_flt = proc_state.cmaj_flt;
    buf->kstk_esp = proc_state.kesp;
    buf->kstk_eip = proc_state.keip;

    buf->flags = _glibtop_sysdeps_proc_kernel;

    if (glibtop_get_proc_data_proc_kernel_s (server, &proc_kernel, pid))
	return;

    buf->nwchan = proc_kernel.wchan;
    buf->flags |= _glibtop_sysdeps_proc_kernel_kernel;
}
