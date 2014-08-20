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
#include <libintl.h>
#include <stdio.h>

#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/close.h>

#include <glibtop/parameter.h>

#include <glibtop/union.h>
#include <glibtop/sysdeps.h>

#include <sys/resource.h>

#ifndef PROFILE_COUNT
#define PROFILE_COUNT	1
#endif

int
main (int argc, char *argv [])
{
	glibtop_union data;
	glibtop_sysdeps sysdeps;
	unsigned c, method, count, port, i;
	char buffer [BUFSIZ];
	pid_t pid, ppid, *ptr;
	char *args;

	count = PROFILE_COUNT;

	setlocale (LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, GTOPLOCALEDIR);
	textdomain (GETTEXT_PACKAGE);

	glibtop_init_r (&glibtop_global_server, 0, GLIBTOP_INIT_NO_OPEN);

	glibtop_get_parameter (GLIBTOP_PARAM_METHOD, &method, sizeof (method));

	printf ("Method = %d\n", method);

	count = glibtop_get_parameter (GLIBTOP_PARAM_COMMAND, buffer, BUFSIZ);
	buffer [count] = 0;

	printf ("Command = '%s'\n", buffer);

	count = glibtop_get_parameter (GLIBTOP_PARAM_HOST, buffer, BUFSIZ);
	buffer [count] = 0;

	glibtop_get_parameter (GLIBTOP_PARAM_PORT, &port, sizeof (port));

	printf ("Host = '%s' - %u\n\n", buffer, port);

	glibtop_init_r (&glibtop_global_server, 0, 0);

	for (c = 0; c < PROFILE_COUNT; c++)
	  	glibtop_get_cpu (&data.cpu);

	printf ("CPU          (0x%08lx): %lu, %lu, %lu, %lu, %lu, %lu\n",
		(unsigned long) data.cpu.flags,
		(unsigned long) data.cpu.total,
		(unsigned long) data.cpu.user,
		(unsigned long) data.cpu.nice,
		(unsigned long) data.cpu.sys,
		(unsigned long) data.cpu.idle,
		(unsigned long) data.cpu.frequency);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_mem (&data.mem);

	printf ("Memory       (0x%08lx): "
		"%lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu\n",
		(unsigned long) data.mem.flags,
		(unsigned long) data.mem.total,
		(unsigned long) data.mem.used,
		(unsigned long) data.mem.free,
		(unsigned long) data.mem.shared,
		(unsigned long) data.mem.buffer,
		(unsigned long) data.mem.cached,
		(unsigned long) data.mem.user,
		(unsigned long) data.mem.locked);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_swap (&data.swap);

	printf ("Swap         (0x%08lx): %lu, %lu, %lu, %lu, %lu\n",
		(unsigned long) data.swap.flags,
		(unsigned long) data.swap.total,
		(unsigned long) data.swap.used,
		(unsigned long) data.swap.free,
		(unsigned long) data.swap.pagein,
		(unsigned long) data.swap.pageout);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_uptime (&data.uptime);

	printf ("Uptime       (0x%08lx): %f, %f\n",
		(unsigned long) data.uptime.flags,
		data.uptime.uptime, data.uptime.idletime);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_loadavg (&data.loadavg);

	printf ("Loadavg      (0x%08lx): %f, %f, %f - %lu, %lu, %lu\n",
		(unsigned long) data.loadavg.flags,
		(double) data.loadavg.loadavg [0],
		(double) data.loadavg.loadavg [1],
		(double) data.loadavg.loadavg [2],
		(unsigned long) data.loadavg.nr_running,
		(unsigned long) data.loadavg.nr_tasks,
		(unsigned long) data.loadavg.last_pid);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_shm_limits (&data.shm_limits);

	printf ("Shm Limits   (0x%08lx): %lu, %lu, %lu, %lu, %lu\n",
		(unsigned long) data.shm_limits.flags,
		(unsigned long) data.shm_limits.shmmax,
		(unsigned long) data.shm_limits.shmmin,
		(unsigned long) data.shm_limits.shmmni,
		(unsigned long) data.shm_limits.shmseg,
		(unsigned long) data.shm_limits.shmall);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_msg_limits (&data.msg_limits);

	printf ("Msg Limits   (0x%08lx): %lu, %lu, %lu, %lu, %lu, %lu, %lu\n",
		(unsigned long) data.msg_limits.flags,
		(unsigned long) data.msg_limits.msgpool,
		(unsigned long) data.msg_limits.msgmap,
		(unsigned long) data.msg_limits.msgmax,
		(unsigned long) data.msg_limits.msgmnb,
		(unsigned long) data.msg_limits.msgmni,
		(unsigned long) data.msg_limits.msgssz,
		(unsigned long)  data.msg_limits.msgtql);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_sem_limits (&data.sem_limits);

	printf ("Sem Limits   (0x%08lx): "
		"%lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu\n",
		(unsigned long) data.sem_limits.flags,
		(unsigned long) data.sem_limits.semmap,
		(unsigned long) data.sem_limits.semmni,
		(unsigned long) data.sem_limits.semmns,
		(unsigned long) data.sem_limits.semmnu,
		(unsigned long) data.sem_limits.semmsl,
		(unsigned long) data.sem_limits.semopm,
		(unsigned long) data.sem_limits.semume,
		(unsigned long) data.sem_limits.semusz,
		(unsigned long) data.sem_limits.semvmx,
		(unsigned long) data.sem_limits.semaem);

	printf ("\n");

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_ppp (&data.ppp, 0);

	printf ("PPP          (0x%08lx): %lu, %lu, %lu\n",
		(unsigned long) data.ppp.flags,
		(unsigned long) data.ppp.state,
		(unsigned long) data.ppp.bytes_in,
		(unsigned long) data.ppp.bytes_out);

	printf ("\n");

	glibtop_get_sysdeps (&sysdeps);

	printf ("Sysdeps      (0x%08lx): %lu, %lu, %lu, %lu, %lu, "
		"%lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, "
		"%lu, %lu, %lu, %lu, %lu, %lu\n",
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
		(unsigned long) sysdeps.proc_segment,
		(unsigned long) sysdeps.proc_args,
		(unsigned long) sysdeps.proc_map,
		(unsigned long) sysdeps.mountlist,
		(unsigned long) sysdeps.fsusage,
		(unsigned long) sysdeps.netload,
		(unsigned long) sysdeps.ppp);

	printf ("\n");

	ptr = glibtop_get_proclist (&data.proclist, 0, 0);

	printf ("Proclist     (0x%08lx): %lu, %lu, %lu\n",
		(unsigned long) data.proclist.flags,
		(unsigned long) data.proclist.number,
		(unsigned long) data.proclist.size,
		(unsigned long) data.proclist.total);

	if (ptr) {
		printf ("\nProcess: ");
		for (i = 0; i < data.proclist.number; i++)
			printf ("%s%u", i ? ", " : "", ptr [i]);
		printf ("\n");
	}

	g_free (ptr);

	pid = getpid ();
	ppid = getppid ();

	printf ("\n");

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_state (&data.proc_state, pid);

	printf ("Proc_State   PID  %5d (0x%08lx): '%s', %c, %lu, %lu\n",
		(int) pid, (unsigned long) data.proc_state.flags,
		data.proc_state.cmd, data.proc_state.state,
		(unsigned long) data.proc_state.uid,
		(unsigned long) data.proc_state.gid);

	for (c = 0; c < PROFILE_COUNT; c++)
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

	for (c = 0; c < PROFILE_COUNT; c++)
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

	for (c = 0; c < PROFILE_COUNT; c++)
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

	for (c = 0; c < PROFILE_COUNT; c++)
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

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_signal (&data.proc_signal, pid);

	printf ("Proc_Signal  PID  %5d (0x%08lx): "
		"%lu %lu %lu %lu\n", (int) pid,
		(unsigned long) data.proc_signal.flags,
		(unsigned long) data.proc_signal.signal,
		(unsigned long) data.proc_signal.blocked,
		(unsigned long) data.proc_signal.sigignore,
		(unsigned long) data.proc_signal.sigcatch);

	for (c = 0; c < PROFILE_COUNT; c++)
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

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_state (&data.proc_state, ppid);

	printf ("Proc_State   PPID %5d (0x%08lx): '%s', %c, %lu, %lu\n",
		(int) ppid, (unsigned long) data.proc_state.flags,
		data.proc_state.cmd, data.proc_state.state,
		(unsigned long) data.proc_state.uid,
		(unsigned long) data.proc_state.gid);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_uid (&data.proc_uid, ppid);

	printf ("Proc_Uid     PPID %5d (0x%08lx): "
		"%d %d %d %d %d %d %d %d %d %d %d %d\n", (int) ppid,
		(unsigned long) data.proc_uid.flags,
		data.proc_uid.uid, data.proc_uid.euid,
		data.proc_uid.gid, data.proc_uid.egid,
		data.proc_uid.pid, data.proc_uid.ppid,
		data.proc_uid.pgrp, data.proc_uid.session,
		data.proc_uid.tty, data.proc_uid.tpgid,
		data.proc_uid.priority, data.proc_uid.nice);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_mem (&data.proc_mem, ppid);

	printf ("Proc_Mem     PPID %5d (0x%08lx): "
		"%lu %lu %lu %lu %lu %lu\n", (int) ppid,
		(unsigned long) data.proc_mem.flags,
		(unsigned long) data.proc_mem.size,
		(unsigned long) data.proc_mem.vsize,
		(unsigned long) data.proc_mem.resident,
		(unsigned long) data.proc_mem.share,
		(unsigned long) data.proc_mem.rss,
		(unsigned long) data.proc_mem.rss_rlim);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_segment (&data.proc_segment, ppid);

	printf ("Proc_Segment PPID %5d (0x%08lx): "
		"%lu %lu %lu %lu %lu %lu %lu %lu\n", (int) ppid,
		(unsigned long) data.proc_segment.flags,
		(unsigned long) data.proc_segment.text_rss,
		(unsigned long) data.proc_segment.shlib_rss,
		(unsigned long) data.proc_segment.data_rss,
		(unsigned long) data.proc_segment.stack_rss,
		(unsigned long) data.proc_segment.dirty_size,
		(unsigned long) data.proc_segment.start_code,
		(unsigned long) data.proc_segment.end_code,
		(unsigned long) data.proc_segment.start_stack);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_time (&data.proc_time, ppid);

	printf ("Proc_Time    PPID %5d (0x%08lx): "
		"%lu %lu %lu %lu %lu %lu %lu %lu %lu\n", (int) ppid,
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

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_signal (&data.proc_signal, ppid);

	printf ("Proc_Signal  PPID %5d (0x%08lx): %lu %lu %lu %lu\n",
		(int) ppid, (unsigned long) data.proc_signal.flags,
		(unsigned long) data.proc_signal.signal,
		(unsigned long) data.proc_signal.blocked,
		(unsigned long) data.proc_signal.sigignore,
		(unsigned long) data.proc_signal.sigcatch);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_kernel (&data.proc_kernel, ppid);

	printf ("Proc_Kernel  PPID %5d (0x%08lx): "
		"%lu %lu %lu %lu %lu 0x%lx 0x%lx 0x%lx (%s)\n", (int) ppid,
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

	args = glibtop_get_proc_args (&data.proc_args, ppid, 0);

	for (i = 0; i < data.proc_args.size; i++) {
		if (args [i]) continue;
		args [i] = '|';
	}

	printf ("Proc_Args    PID  %5d (0x%08lx): %lu - '%s'\n", (int) ppid,
		(unsigned long) data.proc_args.flags,
		(unsigned long) data.proc_args.size,
		args ? args : "");

	g_free (args);

	printf ("\n");

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_state (&data.proc_state, 1);

	printf ("Proc_State   INIT %5d (0x%08lx): '%s', %c, %lu, %lu\n", 1,
		(unsigned long) data.proc_state.flags,
		data.proc_state.cmd, data.proc_state.state,
		(unsigned long) data.proc_state.uid,
		(unsigned long) data.proc_state.gid);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_uid (&data.proc_uid, 1);

	printf ("Proc_Uid     INIT %5d (0x%08lx): "
		"%d %d %d %d %d %d %d %d %d %d %d %d\n", 1,
		(unsigned long) data.proc_uid.flags,
		data.proc_uid.uid, data.proc_uid.euid,
		data.proc_uid.gid, data.proc_uid.egid,
		data.proc_uid.pid, data.proc_uid.ppid,
		data.proc_uid.pgrp, data.proc_uid.session,
		data.proc_uid.tty, data.proc_uid.tpgid,
		data.proc_uid.priority, data.proc_uid.nice);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_mem (&data.proc_mem, 1);

	printf ("Proc_Mem     INIT %5d (0x%08lx): "
		"%lu %lu %lu %lu %lu %lu\n", 1,
		(unsigned long) data.proc_mem.flags,
		(unsigned long) data.proc_mem.size,
		(unsigned long) data.proc_mem.vsize,
		(unsigned long) data.proc_mem.resident,
		(unsigned long) data.proc_mem.share,
		(unsigned long) data.proc_mem.rss,
		(unsigned long) data.proc_mem.rss_rlim);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_segment (&data.proc_segment, 1);

	printf ("Proc_Segment INIT %5d (0x%08lx): "
		"%lu %lu %lu %lu %lu %lu %lu %lu\n", 1,
		(unsigned long) data.proc_segment.flags,
		(unsigned long) data.proc_segment.text_rss,
		(unsigned long) data.proc_segment.shlib_rss,
		(unsigned long) data.proc_segment.data_rss,
		(unsigned long) data.proc_segment.stack_rss,
		(unsigned long) data.proc_segment.dirty_size,
		(unsigned long) data.proc_segment.start_code,
		(unsigned long) data.proc_segment.end_code,
		(unsigned long) data.proc_segment.start_stack);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_time (&data.proc_time, 1);

	printf ("Proc_Time    INIT %5d (0x%08lx): "
		"%lu %lu %lu %lu %lu %lu %lu %lu %lu\n", 1,
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

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_signal (&data.proc_signal, 1);

	printf ("Proc_Signal  INIT %5d (0x%08lx): "
		"%lu %lu %lu %lu\n", 1,
		(unsigned long) data.proc_signal.flags,
		(unsigned long) data.proc_signal.signal,
		(unsigned long) data.proc_signal.blocked,
		(unsigned long) data.proc_signal.sigignore,
		(unsigned long) data.proc_signal.sigcatch);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_kernel (&data.proc_kernel, 1);

	printf ("Proc_Kernel  INIT %5d (0x%08lx): "
		"%lu %lu %lu %lu %lu 0x%lx 0x%lx 0x%lx (%s)\n", 1,
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

	args = glibtop_get_proc_args (&data.proc_args, 1, 0);

	for (i = 0; i < data.proc_args.size; i++) {
		if (args [i]) continue;
		args [i] = '|';
	}

	printf ("Proc_Args    PID  %5d (0x%08lx): %lu - '%s'\n", 1,
		(unsigned long) data.proc_args.flags,
		(unsigned long) data.proc_args.size,
		args ? args : "");

	g_free (args);

	glibtop_close ();

	exit (0);
}
