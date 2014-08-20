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
#include <procinfo.h>
#include <sys/proc.h>

#include <glibtop.h>
#include <glibtop/prockernel.h>
#include <glibtop/error.h>

#include "utils.h"

static const unsigned long _glibtop_sysdeps_proc_kernel =
(1 <<GLIBTOP_PROC_KERNEL_MIN_FLT) + (1 <<GLIBTOP_PROC_KERNEL_MAJ_FLT) +
(1 <<GLIBTOP_PROC_KERNEL_CMIN_FLT) + (1 <<GLIBTOP_PROC_KERNEL_CMAJ_FLT) +
(1 <<GLIBTOP_PROC_KERNEL_KSTK_ESP) + (1 <<GLIBTOP_PROC_KERNEL_KSTK_EIP) +
(1 <<GLIBTOP_PROC_KERNEL_NWCHAN);



/* Init function. */

void
_glibtop_init_proc_kernel_s (glibtop *server)
{
	server->sysdeps.proc_kernel = _glibtop_sysdeps_proc_kernel;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_kernel_s (glibtop *server, glibtop_proc_kernel *buf,
			   pid_t pid)
{
	struct thrdsinfo thinfo;
	struct procsinfo *pinfo;
	tid_t thid;
	int result;

	glibtop_init_s (&server, (1L << GLIBTOP_SYSDEPS_PROC_KERNEL), 0);

	memset (buf, 0, sizeof (glibtop_proc_kernel));

	pinfo = _glibtop_get_procinfo(server, pid);
	if (pinfo == NULL) return;

	if (!(pinfo->pi_flags | SKPROC))
	{
		/* not a kernel process */

		return;
	}

	buf->min_flt = pinfo->pi_ru.ru_minflt;
	buf->maj_flt = pinfo->pi_ru.ru_majflt;

	buf->cmin_flt = pinfo->pi_cru.ru_minflt;
	buf->cmaj_flt = pinfo->pi_cru.ru_majflt;

	thid = 0;
	result = getthrds(pid, &thinfo, sizeof(thinfo), &thid, 1);
	if (result != 1)
	{
		glibtop_error_io_r(server, "Cannot read thrdsinfo");

		return;
	}

	buf->kstk_esp = thinfo.ti_ustk;
	buf->kstk_eip = thinfo.ti_code;

	buf->nwchan = thinfo.ti_wchan;

	buf->flags = _glibtop_sysdeps_proc_kernel;
}
