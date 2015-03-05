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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <libintl.h>

#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/close.h>

#include <glibtop/union.h>
#include <glibtop/sysdeps.h>

static void
output (pid_t pid)
{
	glibtop_union data;
	char *args;
	unsigned i;

	unsigned long total;
	double p_total, p_utime, p_stime;
	double b_total, b_utime, b_stime;
	double s_total, s_utime, s_stime;
	double my_utime, my_stime;
	unsigned int ncpu;

	printf ("\n");

	glibtop_get_proc_state (&data.proc_state, pid);

	printf ("Proc_State   PID  %5d (0x%08lx): "
		"'%s', %c, %lu, %lu\n", (int) pid,
		(unsigned long) data.proc_state.flags,
		data.proc_state.cmd, data.proc_state.state,
		(unsigned long) data.proc_state.uid,
		(unsigned long) data.proc_state.gid);

	glibtop_get_proc_uid (&data.proc_uid, pid);

	printf ("Proc_Uid     PID  %5d (0x%08lx): "
		"%d %d %d %d %d %d %d %d %d %d %d %d\n", (int) pid,
		(unsigned long) data.proc_uid.flags,
		data.proc_uid.uid, data.proc_uid.euid,
		data.proc_uid.gid, data.proc_uid.egid,
		data.proc_uid.pid, data.proc_uid.ppid,
		data.proc_uid.pgrp, data.proc_uid.session,
		data.proc_uid.tty, data.proc_uid.tpgid,
		data.proc_uid.priority, data.proc_uid.nice);

	glibtop_get_proc_mem (&data.proc_mem, pid);

	printf ("Proc_Mem     PID  %5d (0x%08lx): "
		"%lu %lu %lu %lu %lu %lu\n", (int) pid,
		(unsigned long) data.proc_mem.flags,
		(unsigned long) data.proc_mem.size,
		(unsigned long) data.proc_mem.vsize,
		(unsigned long) data.proc_mem.resident,
		(unsigned long) data.proc_mem.share,
		(unsigned long) data.proc_mem.rss,
		(unsigned long) data.proc_mem.rss_rlim);

	glibtop_get_proc_segment (&data.proc_segment, pid);

	printf ("Proc_Segment PID  %5d (0x%08lx): "
		"%lu %lu %lu %lu %lu %lu %lu %lu\n", (int) pid,
		(unsigned long) data.proc_segment.flags,
		(unsigned long) data.proc_segment.text_rss,
		(unsigned long) data.proc_segment.shlib_rss,
		(unsigned long) data.proc_segment.data_rss,
		(unsigned long) data.proc_segment.stack_rss,
		(unsigned long) data.proc_segment.dirty_size,
		(unsigned long) data.proc_segment.start_code,
		(unsigned long) data.proc_segment.end_code,
		(unsigned long) data.proc_segment.start_stack);

	glibtop_get_proc_time (&data.proc_time, pid);

	printf ("Proc_Time    PID  %5d (0x%08lx): "
		"%lu %lu %lu %lu %lu %lu %lu %lu %lu\n", (int) pid,
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

	printf ("Proc_Signal  PID  %5d (0x%08lx): "
		"%lu %lu %lu %lu\n", (int) pid,
		(unsigned long) data.proc_signal.flags,
		(unsigned long) data.proc_signal.signal,
		(unsigned long) data.proc_signal.blocked,
		(unsigned long) data.proc_signal.sigignore,
		(unsigned long) data.proc_signal.sigcatch);

	glibtop_get_proc_kernel (&data.proc_kernel, pid);

	printf ("Proc_Kernel  PID  %5d (0x%08lx): "
		"%lu %lu %lu %lu %lu 0x%lx 0x%lx 0x%lx (%s)\n", (int) pid,
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

	printf ("\n");

	args = glibtop_get_proc_args (&data.proc_args, pid, 0);

	for (i = 0; i < data.proc_args.size; i++) {
		if (args [i]) continue;
		args [i] = '|';
	}

	printf ("Proc_Args    PID  %5d (0x%08lx): %lu - '%s'\n", (int) pid,
		(unsigned long) data.proc_args.flags,
		(unsigned long) data.proc_args.size,
		args ? args : "");

	g_free (args);

	printf ("\n");

	ncpu = glibtop_global_server->ncpu;

	glibtop_get_proc_time (&data.proc_time, pid);

	total = (unsigned long) data.proc_time.utime +
		(unsigned long) data.proc_time.stime;

	p_total = total ? (double) total : 1.0;

	p_utime = (double) data.proc_time.utime * 100.0 / p_total;
	p_stime = (double) data.proc_time.stime * 100.0 / p_total;

	b_total = p_total / ncpu;
	b_utime = (double) data.proc_time.utime / ncpu;
	b_stime = (double) data.proc_time.stime / ncpu;

	s_total = 0.0; s_utime = 0.0; s_stime = 0.0;

	printf ("Proc_Time    PID  %5d (0x%08lx): %12lu   %12lu   %12lu\n", (int) pid,
		(unsigned long) data.proc_time.flags, total,
		(unsigned long) data.proc_time.utime,
		(unsigned long) data.proc_time.stime);

	for (i = 0; i < ncpu; i++) {
		unsigned long this_total;

		this_total = (unsigned long) data.proc_time.xcpu_utime [i] +
			(unsigned long) data.proc_time.xcpu_stime [i];

		printf ("CPU %3d      PID  %5d (0x%08lx): %12lu   %12lu   %12lu\n", i,
			(int) pid, (unsigned long) data.proc_time.flags, this_total,
			(unsigned long) data.proc_time.xcpu_utime [i],
			(unsigned long) data.proc_time.xcpu_stime [i]);

		s_total += fabs (((double) this_total) - b_total);
		s_utime += fabs (((double) data.proc_time.xcpu_utime [i]) - b_utime);
		s_stime += fabs (((double) data.proc_time.xcpu_stime [i]) - b_stime);
	}

	printf ("\n");

	printf ("Proc_Time    PID  %5d (0x%08lx): %12.3f   %12.3f   %12.3f\n", (int) pid,
		(unsigned long) data.proc_time.flags, 100.0, p_utime, p_stime);

	for (i = 0; i < ncpu; i++) {
		double this_p_total, this_p_utime, this_p_stime;
		unsigned long this_total;

		this_total = (unsigned long) data.proc_time.xcpu_utime [i] +
			(unsigned long) data.proc_time.xcpu_stime [i];

		this_p_total = (double) this_total * 100.0 / p_total;

		this_p_utime = (double) data.proc_time.xcpu_utime [i] * 100.0 / p_total;
		this_p_stime = (double) data.proc_time.xcpu_stime [i] * 100.0 / p_total;

		printf ("CPU %3d      PID  %5d (0x%08lx): %12.3f   %12.3f   %12.3f\n", i,
			(int) pid, (unsigned long) data.proc_time.flags,
			this_p_total, this_p_utime, this_p_stime);
	}

	printf ("\n");

	my_utime = (unsigned long) data.proc_time.utime ?
		(double) data.proc_time.utime : 1.0;
	my_stime = (unsigned long) data.proc_time.stime ?
		(double) data.proc_time.stime : 1.0;

	printf ("SPIN: %31s %12.3f   %12.3f   %12.3f\n", "", s_total * 100.0 / p_total,
		s_utime * 100.0 / my_utime, s_stime * 100.0 / my_stime);

	printf ("\n");
}

int
main (int argc, char *argv [])
{
	glibtop_proclist proclist;
	glibtop_sysdeps sysdeps;
	unsigned i;
	pid_t *ptr, pid;

	setlocale (LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, GTOPLOCALEDIR);
	textdomain (GETTEXT_PACKAGE);

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

	if ((argc == 2) && (sscanf (argv [1], "%d", &pid) == 1)) {
		output (pid);

		exit (0);
	}

	printf ("\n");

	ptr = glibtop_get_proclist (&proclist, 0, 0);

	printf ("Proclist     (0x%08lx): %lu, %lu, %lu\n",
		(unsigned long) proclist.flags,
		(unsigned long) proclist.number,
		(unsigned long) proclist.size,
		(unsigned long) proclist.total);

	if (!ptr) exit (1);

	for (i = 0; i < proclist.number; i++) {

		pid = ptr [i];

		output (pid);
	}

	g_free (ptr);

	exit (0);
}



