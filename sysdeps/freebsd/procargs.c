/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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
#include <glibtop/error.h>
#include <glibtop/xmalloc.h>
#include <glibtop/procargs.h>

#include <glibtop_suid.h>

#include <kvm.h>
#include <sys/param.h>
#include <sys/proc.h>

static const unsigned long _glibtop_sysdeps_proc_args =
(1 << GLIBTOP_PROC_ARGS_SIZE);

/* Init function. */

void
glibtop_init_proc_args_p (glibtop *server)
{
	server->sysdeps.proc_args = _glibtop_sysdeps_proc_args;
}

/* Provides detailed information about a process. */

const char *
glibtop_get_proc_args_p (glibtop *server, glibtop_proc_args *buf,
			 pid_t pid, unsigned max_len)
{
	struct kinfo_proc *pinfo;
	char *retval, **args, **ptr;
	unsigned size = 0;
	int count;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_ARGS), 0);
	
	memset (buf, 0, sizeof (glibtop_proc_args));

	glibtop_suid_enter (server);

	/* Get the process data */
	pinfo = kvm_getprocs (server->machine.kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count < 1))
		glibtop_error_io_r (server, "kvm_getprocs (proc_args)");
	
	args = kvm_getargv (server->machine.kd, pinfo, max_len);
	if (args == NULL) {
		glibtop_warn_io_r (server, "kvm_getargv");
		glibtop_suid_leave (server);
		return NULL;
	}

	glibtop_suid_leave (server);

	for (ptr = args; *ptr; ptr++)
		size += strlen (*ptr)+1;

	buf->size = size+2;

	retval = glibtop_malloc_r (server, buf->size);
	memset (retval, 0, buf->size);

	for (ptr = args; *ptr; ptr++) {
		if (*retval) strcat (retval, " ");
		strcat (retval, *ptr);
	}

	buf->flags = _glibtop_sysdeps_proc_args;

	return retval;
}
