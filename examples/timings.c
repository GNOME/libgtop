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

#include <locale.h>

#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/close.h>
#include <glibtop/xmalloc.h>

#include <glibtop/parameter.h>

#include <glibtop/union.h>
#include <glibtop/sysdeps.h>

#include <sys/times.h>

#include <glibtop_private.h>

#ifndef PROFILE_COUNT
#define PROFILE_COUNT	100000
#endif

int
main (int argc, char *argv [])
{
	glibtop_union data;
	glibtop_sysdeps sysdeps;
	unsigned c, count, port, i, *ptr;
	struct tms start_time, end_time;
	char buffer [BUFSIZ];
	pid_t pid, ppid;
	char *args;

	count = PROFILE_COUNT;

	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE, GTOPLOCALEDIR);
	textdomain (PACKAGE);
	
	glibtop_init_r (&glibtop_global_server, 0, 0);

	times (&start_time);
	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_cpu (&data.cpu);
	times (&end_time);

	printf ("CPU          (0x%08lx): %lu - %lu\n",
		(unsigned long) data.cpu.flags,
		end_time.tms_utime - start_time.tms_utime,
		end_time.tms_stime - start_time.tms_stime);

	times (&start_time);
	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_mem (&data.mem);
	times (&end_time);

	printf ("Memory       (0x%08lx): %lu - %lu\n",
		(unsigned long) data.mem.flags,
		end_time.tms_utime - start_time.tms_utime,
		end_time.tms_stime - start_time.tms_stime);

	times (&start_time);
	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_swap (&data.swap);
	times (&end_time);

	printf ("Swap         (0x%08lx): %lu - %lu\n",
		(unsigned long) data.swap.flags,
		end_time.tms_utime - start_time.tms_utime,
		end_time.tms_stime - start_time.tms_stime);

	times (&start_time);
	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_uptime (&data.uptime);
	times (&end_time);

	printf ("Uptime       (0x%08lx): %lu - %lu\n",
		(unsigned long) data.uptime.flags,
		end_time.tms_utime - start_time.tms_utime,
		end_time.tms_stime - start_time.tms_stime);

	times (&start_time);
	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_loadavg (&data.loadavg);
	times (&end_time);

	printf ("Loadavg      (0x%08lx): %lu - %lu\n",
		(unsigned long) data.loadavg.flags,
		end_time.tms_utime - start_time.tms_utime,
		end_time.tms_stime - start_time.tms_stime);

	printf ("\n");

	times (&start_time);
	for (c = 0; c < PROFILE_COUNT; c++) {
		ptr = glibtop_get_proclist (&data.proclist, 0, 0);
		glibtop_free (ptr);
	}
	times (&end_time);

	printf ("Proclist     (0x%08lx): %lu - %lu\n",
		(unsigned long) data.proclist.flags,
		end_time.tms_utime - start_time.tms_utime,
		end_time.tms_stime - start_time.tms_stime);

	pid = getpid ();

	printf ("\n");

	times (&start_time);
	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_state (&data.proc_state, pid);
	times (&end_time);

	printf ("Proc_State   PID  %5d (0x%08lx): %lu - %lu\n",
		(int) pid, (unsigned long) data.proc_state.flags,
		end_time.tms_utime - start_time.tms_utime,
		end_time.tms_stime - start_time.tms_stime);

	times (&start_time);
	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_uid (&data.proc_uid, pid);
	times (&end_time);

	printf ("Proc_Uid     PID  %5d (0x%08lx): %lu - %lu\n",
		(int) pid, (unsigned long) data.proc_uid.flags,
		end_time.tms_utime - start_time.tms_utime,
		end_time.tms_stime - start_time.tms_stime);

	times (&start_time);
	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_mem (&data.proc_mem, pid);
	times (&end_time);

	printf ("Proc_Mem     PID  %5d (0x%08lx): %lu - %lu\n",
		(int) pid, (unsigned long) data.proc_mem.flags,
		end_time.tms_utime - start_time.tms_utime,
		end_time.tms_stime - start_time.tms_stime);

	times (&start_time);
	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_segment (&data.proc_segment, pid);
	times (&end_time);

	printf ("Proc_Segment PID  %5d (0x%08lx): %lu - %lu\n",
		(int) pid, (unsigned long) data.proc_segment.flags,
		end_time.tms_utime - start_time.tms_utime,
		end_time.tms_stime - start_time.tms_stime);

	times (&start_time);
	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_time (&data.proc_time, pid);
	times (&end_time);

	printf ("Proc_Time    PID  %5d (0x%08lx): %lu - %lu\n",
		(int) pid, (unsigned long) data.proc_time.flags,
		end_time.tms_utime - start_time.tms_utime,
		end_time.tms_stime - start_time.tms_stime);

	times (&start_time);
	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_signal (&data.proc_signal, pid);
	times (&end_time);
	
	printf ("Proc_Signal  PID  %5d (0x%08lx): %lu - %lu\n",
		(int) pid, (unsigned long) data.proc_signal.flags,
		end_time.tms_utime - start_time.tms_utime,
		end_time.tms_stime - start_time.tms_stime);

	times (&start_time);
	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_kernel (&data.proc_kernel, pid);
	times (&end_time);

	printf ("Proc_Kernel  PID  %5d (0x%08lx): %lu - %lu\n",
		(int) pid, (unsigned long) data.proc_kernel.flags,
		end_time.tms_utime - start_time.tms_utime,
		end_time.tms_stime - start_time.tms_stime);
	
	printf ("\n");

	glibtop_close ();

	exit (0);
}
