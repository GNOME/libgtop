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

	fprintf (stderr, "Sysdeps      (0x%08Lx): %Lu, %Lu, %Lu, %Lu, %Lu, "
		 "%Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu\n",
		 sysdeps.flags, sysdeps.cpu, sysdeps.mem, sysdeps.swap,
		 sysdeps.uptime, sysdeps.loadavg, sysdeps.shm_limits,
		 sysdeps.msg_limits, sysdeps.sem_limits,
		 sysdeps.proclist, sysdeps.proc_state,
		 sysdeps.proc_uid, sysdeps.proc_mem,
		 sysdeps.proc_time, sysdeps.proc_signal,
		 sysdeps.proc_kernel, sysdeps.proc_segment);

	fprintf (stderr, "\n");
	
	ptr = glibtop_get_proclist (&proclist);

	fprintf (stderr, "Proclist     (0x%08Lx): %Ld, %Ld, %Ld\n",
		 proclist.flags, proclist.number, proclist.size,
		 proclist.total);

	if (!ptr) exit (1);

	for (i = 0; i < proclist.number; i++) {

		pid = ptr [i];
		
		fprintf (stderr, "\n");
		
		glibtop_get_proc_state (&data.proc_state, pid);
		
		fprintf (stderr, "Proc_State   PID  %5u (0x%08Lx): "
			 "'%s', %c, %u, %u\n", pid, data.proc_state.flags,
			 data.proc_state.cmd, data.proc_state.state,
			 data.proc_state.uid, data.proc_state.gid);
		
		glibtop_get_proc_uid (&data.proc_uid, pid);
		
		fprintf (stderr, "Proc_Uid     PID  %5u (0x%08Lx): "
			 "%d %d %d %d %d %d %d %d %d %d %d %d\n",
			 pid, data.proc_uid.flags, data.proc_uid.uid,
			 data.proc_uid.euid, data.proc_uid.gid,
			 data.proc_uid.egid, data.proc_uid.pid,
			 data.proc_uid.ppid, data.proc_uid.pgrp,
			 data.proc_uid.session, data.proc_uid.tty,
			 data.proc_uid.tpgid, data.proc_uid.priority,
			 data.proc_uid.nice);
		
		glibtop_get_proc_mem (&data.proc_mem, pid);
		
		fprintf (stderr, "Proc_Mem     PID  %5u (0x%08Lx): "
			 "%Ld %Ld %Ld %Ld %Ld %Ld\n", pid, data.proc_mem.flags,
			 data.proc_mem.size, data.proc_mem.vsize,
			 data.proc_mem.resident, data.proc_mem.share,
			 data.proc_mem.rss, data.proc_mem.rss_rlim);
		
		glibtop_get_proc_time (&data.proc_time, pid);
		
		fprintf (stderr, "Proc_Time    PID  %5u (0x%08Lx): "
			 "%Ld %Ld %Ld %Ld %Ld %Ld %Ld\n", pid, data.proc_time.flags,
			 data.proc_time.start_time, data.proc_time.utime,
			 data.proc_time.stime, data.proc_time.cutime,
			 data.proc_time.cstime, data.proc_time.timeout,
			 data.proc_time.it_real_value);

		glibtop_get_proc_signal (&data.proc_signal, pid);
	
		fprintf (stderr, "Proc_Signal  PID  %5u (0x%08Lx): "
			 "%Lu %Lu %Lu %Lu\n", pid, data.proc_signal.flags,
			 data.proc_signal.signal, data.proc_signal.blocked,
			 data.proc_signal.sigignore, data.proc_signal.sigcatch);

		glibtop_get_proc_kernel (&data.proc_kernel, pid);

		fprintf (stderr, "Proc_Kernel  PID  %5u (0x%08Lx): "
			 "%Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu\n", pid,
			 data.proc_kernel.flags, data.proc_kernel.k_flags,
			 data.proc_kernel.min_flt, data.proc_kernel.maj_flt,
			 data.proc_kernel.cmin_flt, data.proc_kernel.cmaj_flt,
			 data.proc_kernel.kstk_esp, data.proc_kernel.kstk_eip,
			 data.proc_kernel.wchan);

		glibtop_get_proc_segment (&data.proc_segment, pid);

		fprintf (stderr, "Proc_Segment PID  %5u (0x%08Lx): "
			 "%Ld %Ld %Ld %Ld %Lu %Lu %Lu\n", pid, data.proc_segment.flags,
			 data.proc_segment.trs, data.proc_segment.lrs,
			 data.proc_segment.drs, data.proc_segment.dt,
			 data.proc_segment.start_code, data.proc_segment.end_code,
			 data.proc_segment.start_stack);

		fprintf (stderr, "\n");
	}

	glibtop_free (ptr);

	exit (0);
}
