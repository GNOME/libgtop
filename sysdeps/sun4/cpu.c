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
	long cp_time [CPUSTATES], mp_time [NCPU][CPUSTATES];
	int i;

	glibtop_init_r (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_cpu));

	/* !!! THE FOLLOWING CODE RUNS SGID KMEM - CHANGE WITH CAUTION !!! */
	
	setregid (server->machine.gid, server->machine.egid);
	
	/* get the cp_time array */

	(void) _glibtop_getkval (server, _glibtop_nlist [X_CP_TIME].n_value,
				 (int *) cp_time, sizeof (cp_time),
				 _glibtop_nlist [X_CP_TIME].n_name);
	
#ifdef MULTIPROCESSOR
	/* get the mp_time array as well */
	
	if (server->machine.ncpu > 1) {
		(void) _glibtop_getkval (server, _glibtop_nlist [X_MP_TIME].n_value,
					 (int *) mp_time, sizeof (mp_time),
					 _glibtop_nlist [X_MP_TIME].n_name);
	}
#endif
	
	if (setregid (server->machine.egid, server->machine.gid))
		_exit (1);
	
	/* !!! END OF SUID ROOT PART !!! */
	
#ifdef MULTIPROCESSOR
	/* If we have multiple processors, we add the times for each of them and set
	   frequency to 100 times the number of the processors. */

	/* [FIXME]: I had no machine with more than one processor to test this code !!! */

	if (server->machine.ncpu > 1) {
		for (i = 0; i < server->machine.ncpu; i++) {
			buf->user += mp_time [i][CP_USER];
			buf->nice += mp_time [i][CP_NICE];
			buf->sys  += mp_time [i][CP_SYS];
			buf->idle += mp_time [i][CP_IDLE];
			buf->frequency += 100;
		}
	} else {
		buf->user = cp_time [CP_USER];
		buf->nice = cp_time [CP_NICE];
		buf->sys  = cp_time [CP_SYS];
		buf->idle = cp_time [CP_IDLE];
		buf->frequency = 100;
	}
#else
	buf->user = cp_time [CP_USER];
	buf->nice = cp_time [CP_NICE];
	buf->sys  = cp_time [CP_SYS];
	buf->idle = cp_time [CP_IDLE];
	buf->frequency = 100;
#endif

	/* Calculate total time. */

	buf->total = buf->user + buf->nice + buf->sys + buf->idle;

	/* Now we can set the flags. */

	buf->flags = _glibtop_sysdeps_cpu;
}
