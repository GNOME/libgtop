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
#include <glibtop/procargs.h>

#include "glibtop_private.h"

static const unsigned long _glibtop_sysdeps_proc_args =
(1L << GLIBTOP_PROC_ARGS_SIZE);

/* Init function. */

void
_glibtop_init_proc_args_s (glibtop *server)
{
	server->sysdeps.proc_args = _glibtop_sysdeps_proc_args;
}

/* Provides detailed information about a process. */

char *
glibtop_get_proc_args_s (glibtop *server, glibtop_proc_args *buf,
			 pid_t pid, unsigned max_len)
{
#ifdef HAVE_PROCFS_H
	struct psinfo pinfo;
#else
	struct prpsinfo pinfo;
#endif
	unsigned len;
	char *ret, *p;

	memset (buf, 0, sizeof (glibtop_proc_args));

	if(glibtop_get_proc_data_psinfo_s(server, &pinfo, pid))
		return NULL;

	/* strnlen */
	for(len = 0; len < PRARGSZ; ++len)
	   if(!(pinfo.pr_psargs[len]))
	      break;


	if(max_len && max_len < len)
	{
		len = max_len;
	}

	ret = g_malloc(len + 1);
	memcpy(ret, pinfo.pr_psargs, len);
	ret[len] = 0;

	for(p = ret; *p; ++p)
	{
		if(*p == ' ') *p = 0;
	}

	buf->size  = len;
	buf->flags = _glibtop_sysdeps_proc_args;

	return ret;
}
