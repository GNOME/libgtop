/* $Id$ */

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

#include <config.h>
#include <glibtop/cpu.h>

static const unsigned long _glibtop_sysdeps_cpu =
(1 << GLIBTOP_CPU_TOTAL) + (1 << GLIBTOP_CPU_USER) +
(1 << GLIBTOP_CPU_NICE) + (1 << GLIBTOP_CPU_SYS) +
(1 << GLIBTOP_CPU_IDLE) + (1 << GLIBTOP_CPU_FREQUENCY);

/* Provides information about cpu usage. */

void
glibtop_get_cpu_p (glibtop *server, glibtop_cpu *buf)
{
	long cp_time [CPUSTATES];
	int i;

	glibtop_init_p (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_cpu));

	/* !!! THE FOLLOWING CODE RUNS SGID KMEM - CHANGE WITH CAUTION !!! */
	
	setregid (server->machine.gid, server->machine.egid);
	
	/* get the cp_time array */

	(void) _glibtop_getkval (server, _glibtop_nlist [X_CP_TIME].n_value,
				 (int *) cp_time, sizeof (cp_time),
				 _glibtop_nlist [X_CP_TIME].n_name);
	
	if (setregid (server->machine.egid, server->machine.gid))
		_exit (1);
	
	/* !!! END OF SUID ROOT PART !!! */
	
	buf->user = cp_time [CP_USER];
	buf->nice = cp_time [CP_NICE];
	buf->sys  = cp_time [CP_SYS];
	buf->idle = cp_time [CP_IDLE];
	buf->frequency = server->machine.hz;

	/* Calculate total time. */

	buf->total = buf->user + buf->nice + buf->sys + buf->idle;

	/* Now we can set the flags. */

	buf->flags = _glibtop_sysdeps_cpu;
}
