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

#include <locale.h>

#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/close.h>
#include <glibtop/xmalloc.h>

#include <glibtop/union.h>
#include <glibtop/sysdeps.h>

int
main (int argc, char *argv [])
{
	glibtop_union data;
	glibtop_proclist proclist;
	glibtop_sysdeps sysdeps;
	unsigned *ptr, pid, i;

	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE, GTOPLOCALEDIR);
	textdomain (PACKAGE);
	
	glibtop_init ();

	glibtop_get_sysdeps (&sysdeps);

	printf ("Sysdeps      (0x%08lx): %lu, %lu, %lu, %lu, %lu, "
		"%lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu\n",
		(unsigned long) sysdeps.flags,
		(unsigned long) sysdeps.cpu,
		(unsigned long) sysdeps.mem,
		(unsigned long) sysdeps.swap,
		(unsigned long) sysdeps.uptime,
		(unsigned long) sysdeps.loadavg,
		(unsigned long) sysdeps.shm_limits,
		(unsigned long) sysdeps.msg_limits,
		(unsigned long) sysdeps.sem_limits,
		(unsigned long) sysdeps.proclist,
		(unsigned long) sysdeps.proc_state,
		(unsigned long) sysdeps.proc_uid,
		(unsigned long) sysdeps.proc_mem,
		(unsigned long) sysdeps.proc_time,
		(unsigned long) sysdeps.proc_signal,
		(unsigned long) sysdeps.proc_kernel,
		(unsigned long) sysdeps.proc_segment);

	printf ("\n");
	
	ptr = glibtop_get_proclist (&proclist, 0, 0);

	printf ("Proclist     (0x%08lx): %lu, %lu, %lu\n",
		(unsigned long) data.proclist.flags,
		(unsigned long) data.proclist.number,
		(unsigned long) data.proclist.size,
		(unsigned long) data.proclist.total);

	if (!ptr) exit (1);

	for (i = 0; i < proclist.number; i++) {

		pid = ptr [i];
		
		printf ("\n");
		
		glibtop_get_proc_state (&data.proc_state, pid);
		
		printf ("Proc_State   PID  %5u (0x%08lx): "
			"'%s', %c, %u, %u\n", pid,
			(unsigned long) data.proc_state.flags,
			data.proc_state.cmd, data.proc_state.state,
			data.proc_state.uid, data.proc_state.gid);
		
		glibtop_get_proc_uid (&data.proc_uid, pid);
		
		printf ("Proc_Uid     PID  %5u (0x%08lx): "
			"%d %d %d %d %d %d %d %d %d %d %d %d\n", pid,
			(unsigned long) data.proc_uid.flags,
			data.proc_uid.uid, data.proc_uid.euid,
			data.proc_uid.gid, data.proc_uid.egid,
			data.proc_uid.pid, data.proc_uid.ppid,
			data.proc_uid.pgrp, data.proc_uid.session,
			data.proc_uid.tty, data.proc_uid.tpgid,
			data.proc_uid.priority, data.proc_uid.nice);
		
		glibtop_get_proc_mem (&data.proc_mem, pid);
		
		printf ("Proc_Mem     PID  %5u (0x%08lx): "
			"%lu %lu %lu %lu %lu %lu\n", pid,
			(unsigned long) data.proc_mem.flags,
			(unsigned long) data.proc_mem.size,
			(unsigned long) data.proc_mem.vsize,
			(unsigned long) data.proc_mem.resident,
			(unsigned long) data.proc_mem.share,
			(unsigned long) data.proc_mem.rss,
			(unsigned long) data.proc_mem.rss_rlim);
		
		glibtop_get_proc_time (&data.proc_time, pid);
		
		printf ("Proc_Time    PID  %5u (0x%08lx): "
			"%lu %lu %lu %lu %lu %lu %lu %lu %lu\n", pid,
			(unsigned long) data.proc_time.flags,
			(unsigned long) data.proc_time.start_time,
			(unsigned long) data.proc_time.rtime,
			(unsigned long) data.proc_time.utime,
			(unsigned long) data.proc_time.stime,
			(unsigned long) data.proc_time.cutime,
			(unsigned long) data.proc_time.cstime,
			(unsigned long) data.proc_time.timeout,
			(unsigned long) data.proc_time.it_real_value,
			(unsigned long) data.proc_time.frequency);

		glibtop_get_proc_signal (&data.proc_signal, pid);
	
		printf ("Proc_Signal  PID  %5u (0x%08lx): "
			"%lu %lu %lu %lu\n", pid,
			(unsigned long) data.proc_signal.flags,
			(unsigned long) data.proc_signal.signal,
			(unsigned long) data.proc_signal.blocked,
			(unsigned long) data.proc_signal.sigignore,
			(unsigned long) data.proc_signal.sigcatch);

		glibtop_get_proc_kernel (&data.proc_kernel, pid);

		printf ("Proc_Kernel  PID  %5u (0x%08lx): "
			"%lu %lu %lu %lu %lu %lu %lu %lu (%s)\n", pid,
			(unsigned long) data.proc_kernel.flags,
			(unsigned long) data.proc_kernel.k_flags,
			(unsigned long) data.proc_kernel.min_flt,
			(unsigned long) data.proc_kernel.maj_flt,
			(unsigned long) data.proc_kernel.cmin_flt,
			(unsigned long) data.proc_kernel.cmaj_flt,
			(unsigned long) data.proc_kernel.kstk_esp,
			(unsigned long) data.proc_kernel.kstk_eip,
			(unsigned long) data.proc_kernel.nwchan,
			data.proc_kernel.wchan);

		glibtop_get_proc_segment (&data.proc_segment, pid);

		printf ("Proc_Segment PID  %5u (0x%08lx): "
			"%lu %lu %lu %lu %lu %lu %lu\n", pid,
			(unsigned long) data.proc_segment.flags,
			(unsigned long) data.proc_segment.trs,
			(unsigned long) data.proc_segment.lrs,
			(unsigned long) data.proc_segment.drs,
			(unsigned long) data.proc_segment.dt,
			(unsigned long) data.proc_segment.start_code,
			(unsigned long) data.proc_segment.end_code,
			(unsigned long) data.proc_segment.start_stack);

		printf ("\n");
	}

	glibtop_free (ptr);

	exit (0);
}
