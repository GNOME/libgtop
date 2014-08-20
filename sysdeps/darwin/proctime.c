/*
   This file is part of LibGTop 2.0.

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
#include <unistd.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/proctime.h>

#include <glibtop_suid.h>

#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/mach_port.h>
#include <mach/task.h>
#include <mach/vm_map.h>
#include <sys/types.h>
#include <sys/sysctl.h>

static const unsigned long _glibtop_sysdeps_proc_time =
(1L << GLIBTOP_PROC_TIME_UTIME) + (1L << GLIBTOP_PROC_TIME_STIME) +
(1L << GLIBTOP_PROC_TIME_FREQUENCY) + (1L << GLIBTOP_PROC_TIME_START_TIME);

#define tv2sec(tv) (((u_int64_t) tv.seconds * 1000000) + (u_int64_t) tv.microseconds)

/* Init function. */

void
_glibtop_init_proc_time_p (glibtop *server)
{
	server->sysdeps.proc_time = _glibtop_sysdeps_proc_time;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_time_p (glibtop *server, glibtop_proc_time *buf,
			 pid_t pid)
{
	unsigned int count;
	task_basic_info_data_t ti;
	task_thread_times_info_data_t tti;
	task_port_t task;
	time_value_t utime = {0, 0}, stime = {0, 0};
	size_t length;
	struct kinfo_proc kinfo;
	int mib[] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, 0 };

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_TIME), 0);

	memset (buf, 0, sizeof (glibtop_proc_time));

	mib[3] = pid;
	length = sizeof (kinfo);
	if (sysctl (mib, 4, &kinfo, &length, NULL, 0) < 0) {
		glibtop_suid_leave (server);
		return;
	}

	/* !!! THE FOLLOWING CODE RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

	glibtop_suid_enter (server);

	if (task_for_pid (mach_task_self(), pid, &task) != KERN_SUCCESS) {
		glibtop_suid_leave (server);
		return;
	}

	glibtop_suid_leave (server);

	/* !!! END OF SUID ROOT PART !!! */

	count = TASK_BASIC_INFO_COUNT;
	if (task_info(task, TASK_BASIC_INFO, (task_info_t)&ti, &count) != KERN_SUCCESS) {
		glibtop_warn_io_r (server, "task_info (proctime)");
		if (task != mach_task_self())
			mach_port_deallocate (mach_task_self(), task);
		return;
	}

	count = TASK_THREAD_TIMES_INFO_COUNT;
	if (task_info(task, TASK_THREAD_TIMES_INFO, (task_info_t)&tti, &count) != KERN_SUCCESS) {
		glibtop_warn_io_r (server, "task_info (proctime)");
		if (task != mach_task_self())
			mach_port_deallocate (mach_task_self(), task);
		return;
	}

	time_value_add (&utime, &ti.user_time);
	time_value_add (&stime, &ti.system_time);
	time_value_add (&utime, &tti.user_time);
	time_value_add (&stime, &tti.system_time);

	/* use the same frequency as cpu.c */
	buf->utime  = tv2sec (utime) / 10000;
	buf->stime  = tv2sec (stime) / 10000;
	buf->frequency = 1000000 / 10000;
	buf->start_time = kinfo.kp_proc.p_starttime.tv_sec;

	buf->flags  = _glibtop_sysdeps_proc_time;
}
