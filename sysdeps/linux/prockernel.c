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
#include <glibtop/error.h>
#include <glibtop/prockernel.h>

#include "glibtop_private.h"

static const unsigned long _glibtop_sysdeps_proc_kernel =
(1L << GLIBTOP_PROC_KERNEL_K_FLAGS) + (1L << GLIBTOP_PROC_KERNEL_MIN_FLT) +
(1L << GLIBTOP_PROC_KERNEL_MAJ_FLT) + (1L << GLIBTOP_PROC_KERNEL_CMIN_FLT) +
(1L << GLIBTOP_PROC_KERNEL_CMAJ_FLT) + (1L << GLIBTOP_PROC_KERNEL_KSTK_ESP) +
(1L << GLIBTOP_PROC_KERNEL_KSTK_EIP) + (1L << GLIBTOP_PROC_KERNEL_NWCHAN) +
(1L << GLIBTOP_PROC_KERNEL_WCHAN);

/* Init function. */

void
_glibtop_init_proc_kernel_s (glibtop *server)
{
	server->sysdeps.proc_kernel = _glibtop_sysdeps_proc_kernel;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_kernel_s (glibtop *server, glibtop_proc_kernel *buf, pid_t pid)
{
	char buffer [BUFSIZ], *p;

	memset (buf, 0, sizeof (glibtop_proc_kernel));

	if (proc_stat_to_buffer(buffer, sizeof buffer, pid))
		return;

	p = proc_stat_after_cmd (buffer);
	if (!p) return;

	p = skip_multiple_token (p, 6);

	buf->k_flags   = strtoull (p, &p, 0);
	buf->min_flt   = strtoull (p, &p, 0);
	buf->cmin_flt  = strtoull (p, &p, 0);
	buf->maj_flt   = strtoull (p, &p, 0);
	buf->cmaj_flt  = strtoull (p, &p, 0);

	p = skip_multiple_token (p, 15);

	buf->kstk_esp = strtoull (p, &p, 0);
	buf->kstk_eip = strtoull (p, &p, 0);

	p = skip_multiple_token (p, 4);

	buf->nwchan = strtoull (p, &p, 0);

	proc_file_to_buffer(buf->wchan, sizeof buf->wchan, "/proc/%d/wchan", pid),

	buf->flags = _glibtop_sysdeps_proc_kernel;
}
