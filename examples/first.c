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

#include <glibtop/parameter.h>

#include <glibtop/union.h>
#include <glibtop/sysdeps.h>

#ifndef PROFILE_COUNT
#define PROFILE_COUNT	1
#endif

int
main (int argc, char *argv [])
{
	glibtop_union data;
	glibtop_sysdeps sysdeps;
	unsigned c, method, count, port, i, *ptr;
	char buffer [BUFSIZ];
	pid_t pid, ppid;

	count = PROFILE_COUNT;

	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE, GTOPLOCALEDIR);
	textdomain (PACKAGE);
	
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

	for (c = 0; c < PROFILE_COUNT; c++)
	  	glibtop_get_cpu (&data.cpu);

	printf ("CPU          (0x%08Lx): %Lu, %Lu, %Lu, %Lu, %Lu, %Lu\n",
		data.cpu.flags, data.cpu.total, data.cpu.user, data.cpu.nice,
		data.cpu.sys, data.cpu.idle, data.cpu.frequency);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_mem (&data.mem);

	printf ("Memory       (0x%08Lx): %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu\n",
		data.mem.flags, data.mem.total, data.mem.used, data.mem.free,
		data.mem.shared, data.mem.buffer, data.mem.cached,
		data.mem.user, data.mem.locked);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_swap (&data.swap);

	printf ("Swap         (0x%08Lx): %Lu, %Lu, %Lu\n", data.swap.flags,
		data.swap.total, data.swap.used, data.swap.free);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_uptime (&data.uptime);

	printf ("Uptime       (0x%08Lx): %f, %f\n", data.uptime.flags,
		data.uptime.uptime, data.uptime.idletime);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_loadavg (&data.loadavg);

	printf ("Loadavg      (0x%08Lx): %f, %f, %f\n", data.loadavg.flags,
		data.loadavg.loadavg [0], data.loadavg.loadavg [1],
		data.loadavg.loadavg [2]);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_shm_limits (&data.shm_limits);
	
	printf ("Shm Limits   (0x%08Lx): %Lu, %Lu, %Lu, %Lu, %Lu\n",
		data.shm_limits.flags, data.shm_limits.shmmax,
		data.shm_limits.shmmin, data.shm_limits.shmmni,
		data.shm_limits.shmseg, data.shm_limits.shmall);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_msg_limits (&data.msg_limits);

	printf ("Msg Limits   (0x%08Lx): %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu\n",
		data.msg_limits.flags, data.msg_limits.msgpool,
		data.msg_limits.msgmap, data.msg_limits.msgmax,
		data.msg_limits.msgmnb, data.msg_limits.msgmni,
		data.msg_limits.msgssz, data.msg_limits.msgtql);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_sem_limits (&data.sem_limits);
	
	printf ("Sem Limits   (0x%08Lx): "
		"%Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu\n",
		data.sem_limits.flags, data.sem_limits.semmap,
		data.sem_limits.semmni, data.sem_limits.semmns,
		data.sem_limits.semmnu, data.sem_limits.semmsl,
		data.sem_limits.semopm, data.sem_limits.semume,
		data.sem_limits.semusz, data.sem_limits.semvmx,
		data.sem_limits.semaem);

	printf ("\n");

	glibtop_get_sysdeps (&sysdeps);

	printf ("Sysdeps      (0x%08Lx): %Lu, %Lu, %Lu, %Lu, %Lu, "
		"%Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu, %Lu\n",
		sysdeps.flags, sysdeps.cpu, sysdeps.mem, sysdeps.swap,
		sysdeps.uptime, sysdeps.loadavg, sysdeps.shm_limits,
		sysdeps.msg_limits, sysdeps.sem_limits,
		sysdeps.proclist, sysdeps.proc_state,
		sysdeps.proc_uid, sysdeps.proc_mem,
		sysdeps.proc_time, sysdeps.proc_signal,
		sysdeps.proc_kernel, sysdeps.proc_segment);

	printf ("\n");
	
	ptr = glibtop_get_proclist (&data.proclist);

	printf ("Proclist     (0x%08Lx): %Lu, %Lu, %Lu\n",
		data.proclist.flags, data.proclist.number,
		data.proclist.size, data.proclist.total);

	if (ptr) {
		printf ("\nProcess: ");
		for (i = 0; i < data.proclist.number; i++)
			printf ("%s%u", i ? ", " : "", ptr [i]);
		printf ("\n");
	}

	glibtop_free (ptr);

	pid = getpid ();
	ppid = getppid ();

	printf ("\n");

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_state (&data.proc_state, pid);

	printf ("Proc_State   PID  %5u (0x%08Lx): '%s', %c, %u, %u\n",
		pid, data.proc_state.flags, data.proc_state.cmd,
		data.proc_state.state, data.proc_state.uid,
		data.proc_state.gid);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_uid (&data.proc_uid, pid);

	printf ("Proc_Uid     PID  %5u (0x%08Lx): "
		"%d %d %d %d %d %d %d %d %d %d %d %d\n",
		pid, data.proc_uid.flags, data.proc_uid.uid,
		data.proc_uid.euid, data.proc_uid.gid,
		data.proc_uid.egid, data.proc_uid.pid,
		data.proc_uid.ppid, data.proc_uid.pgrp,
		data.proc_uid.session, data.proc_uid.tty,
		data.proc_uid.tpgid, data.proc_uid.priority,
		data.proc_uid.nice);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_mem (&data.proc_mem, pid);

	printf ("Proc_Mem     PID  %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu %Lu %Lu\n", pid, data.proc_mem.flags,
		data.proc_mem.size, data.proc_mem.vsize,
		data.proc_mem.resident, data.proc_mem.share,
		data.proc_mem.rss, data.proc_mem.rss_rlim);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_time (&data.proc_time, pid);

	printf ("Proc_Time    PID  %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu %Lu %Lu %Lu\n", pid, data.proc_time.flags,
		data.proc_time.start_time, data.proc_time.utime,
		data.proc_time.stime, data.proc_time.cutime,
		data.proc_time.cstime, data.proc_time.timeout,
		data.proc_time.it_real_value);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_signal (&data.proc_signal, pid);
	
	printf ("Proc_Signal  PID  %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu\n", pid, data.proc_signal.flags,
		data.proc_signal.signal, data.proc_signal.blocked,
		data.proc_signal.sigignore, data.proc_signal.sigcatch);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_kernel (&data.proc_kernel, pid);

	printf ("Proc_Kernel  PID  %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu\n", pid,
		data.proc_kernel.flags, data.proc_kernel.k_flags,
		data.proc_kernel.min_flt, data.proc_kernel.maj_flt,
		data.proc_kernel.cmin_flt, data.proc_kernel.cmaj_flt,
		data.proc_kernel.kstk_esp, data.proc_kernel.kstk_eip,
		data.proc_kernel.wchan);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_segment (&data.proc_segment, pid);

	printf ("Proc_Segment PID  %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu %Lu %Lu %Lu\n", pid, data.proc_segment.flags,
		data.proc_segment.trs, data.proc_segment.lrs,
		data.proc_segment.drs, data.proc_segment.dt,
		data.proc_segment.start_code, data.proc_segment.end_code,
		data.proc_segment.start_stack);

	printf ("\n");

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_state (&data.proc_state, ppid);

	printf ("Proc_State   PPID %5u (0x%08Lx): '%s', %c, %u, %u\n",
		ppid, data.proc_state.flags, data.proc_state.cmd,
		data.proc_state.state, data.proc_state.uid,
		data.proc_state.gid);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_uid (&data.proc_uid, ppid);

	printf ("Proc_Uid     PPID %5u (0x%08Lx): "
		"%d %d %d %d %d %d %d %d %d %d %d %d\n",
		ppid, data.proc_uid.flags, data.proc_uid.uid,
		data.proc_uid.euid, data.proc_uid.gid,
		data.proc_uid.egid, data.proc_uid.pid,
		data.proc_uid.ppid, data.proc_uid.pgrp,
		data.proc_uid.session, data.proc_uid.tty,
		data.proc_uid.tpgid, data.proc_uid.priority,
		data.proc_uid.nice);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_mem (&data.proc_mem, ppid);

	printf ("Proc_Mem     PPID %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu %Lu %Lu\n", ppid, data.proc_mem.flags,
		data.proc_mem.size, data.proc_mem.vsize,
		data.proc_mem.resident, data.proc_mem.share,
		data.proc_mem.rss, data.proc_mem.rss_rlim);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_time (&data.proc_time, ppid);

	printf ("Proc_Time    PPID %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu %Lu %Lu %Lu\n", ppid, data.proc_time.flags,
		data.proc_time.start_time, data.proc_time.utime,
		data.proc_time.stime, data.proc_time.cutime,
		data.proc_time.cstime, data.proc_time.timeout,
		data.proc_time.it_real_value);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_signal (&data.proc_signal, ppid);

	printf ("Proc_Signal  PPID %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu\n", ppid, data.proc_signal.flags,
		data.proc_signal.signal, data.proc_signal.blocked,
		data.proc_signal.sigignore, data.proc_signal.sigcatch);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_kernel (&data.proc_kernel, ppid);

	printf ("Proc_Kernel  PPID %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu\n", ppid,
		data.proc_kernel.flags, data.proc_kernel.k_flags,
		data.proc_kernel.min_flt, data.proc_kernel.maj_flt,
		data.proc_kernel.cmin_flt, data.proc_kernel.cmaj_flt,
		data.proc_kernel.kstk_esp, data.proc_kernel.kstk_eip,
		data.proc_kernel.wchan);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_segment (&data.proc_segment, ppid);

	printf ("Proc_Segment PPID %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu %Lu %Lu %Lu\n", ppid, data.proc_segment.flags,
		data.proc_segment.trs, data.proc_segment.lrs,
		data.proc_segment.drs, data.proc_segment.dt,
		data.proc_segment.start_code, data.proc_segment.end_code,
		data.proc_segment.start_stack);

	printf ("\n");

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_state (&data.proc_state, 1);

	printf ("Proc_State   INIT %5u (0x%08Lx): '%s', %c, %u, %u\n",
		1, data.proc_state.flags, data.proc_state.cmd,
		data.proc_state.state, data.proc_state.uid,
		data.proc_state.gid);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_uid (&data.proc_uid, 1);

	printf ("Proc_Uid     INIT %5u (0x%08Lx): "
		"%d %d %d %d %d %d %d %d %d %d %d %d\n",
		1, data.proc_uid.flags, data.proc_uid.uid,
		data.proc_uid.euid, data.proc_uid.gid,
		data.proc_uid.egid, data.proc_uid.pid,
		data.proc_uid.ppid, data.proc_uid.pgrp,
		data.proc_uid.session, data.proc_uid.tty,
		data.proc_uid.tpgid, data.proc_uid.priority,
		data.proc_uid.nice);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_mem (&data.proc_mem, 1);

	printf ("Proc_Mem     INIT %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu %Lu %Lu\n", 1, data.proc_mem.flags,
		data.proc_mem.size, data.proc_mem.vsize,
		data.proc_mem.resident, data.proc_mem.share,
		data.proc_mem.rss, data.proc_mem.rss_rlim);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_time (&data.proc_time, 1);

	printf ("Proc_Time    INIT %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu %Lu %Lu %Lu\n", 1, data.proc_time.flags,
		data.proc_time.start_time, data.proc_time.utime,
		data.proc_time.stime, data.proc_time.cutime,
		data.proc_time.cstime, data.proc_time.timeout,
		data.proc_time.it_real_value);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_signal (&data.proc_signal, 1);

	printf ("Proc_Signal  INIT %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu\n", 1, data.proc_signal.flags,
		data.proc_signal.signal, data.proc_signal.blocked,
		data.proc_signal.sigignore, data.proc_signal.sigcatch);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_kernel (&data.proc_kernel, 1);

	printf ("Proc_Kernel  INIT %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu\n", 1,
		data.proc_kernel.flags, data.proc_kernel.k_flags,
		data.proc_kernel.min_flt, data.proc_kernel.maj_flt,
		data.proc_kernel.cmin_flt, data.proc_kernel.cmaj_flt,
		data.proc_kernel.kstk_esp, data.proc_kernel.kstk_eip,
		data.proc_kernel.wchan);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_segment (&data.proc_segment, 1);

	printf ("Proc_Segment INIT %5u (0x%08Lx): "
		"%Lu %Lu %Lu %Lu %Lu %Lu %Lu\n", 1, data.proc_segment.flags,
		data.proc_segment.trs, data.proc_segment.lrs,
		data.proc_segment.drs, data.proc_segment.dt,
		data.proc_segment.start_code, data.proc_segment.end_code,
		data.proc_segment.start_stack);

	glibtop_close ();

	exit (0);
}
