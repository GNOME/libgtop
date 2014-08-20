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

#include <config.h>
#include <glibtop/procdata.h>

#include <sys/user.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <mach.h>
#include <mach/mach_types.h>
#include <mach/task_info.h>

/* Provides detailed information about a process. */

#define BIT_SHIFT(x)		(1L << (x % 64))

static const unsigned long _glibtop_sysdeps_procdata_0 =
BIT_SHIFT(GLIBTOP_PROCDATA_CMD) +
BIT_SHIFT(GLIBTOP_PROCDATA_STATE) +
BIT_SHIFT(GLIBTOP_PROCDATA_UID) +
BIT_SHIFT(GLIBTOP_PROCDATA_PID) +
BIT_SHIFT(GLIBTOP_PROCDATA_PPID) +
BIT_SHIFT(GLIBTOP_PROCDATA_PGRP) +
BIT_SHIFT(GLIBTOP_PROCDATA_TTY) +
BIT_SHIFT(GLIBTOP_PROCDATA_K_FLAGS) +
BIT_SHIFT(GLIBTOP_PROCDATA_SESSION) +
BIT_SHIFT(GLIBTOP_PROCDATA_TPGID) +
BIT_SHIFT(GLIBTOP_PROCDATA_SIGIGNORE) +
BIT_SHIFT(GLIBTOP_PROCDATA_SIGCATCH) +
BIT_SHIFT(GLIBTOP_PROCDATA_SIGNAL) +
BIT_SHIFT(GLIBTOP_PROCDATA_NICE);

static const unsigned long _glibtop_sysdeps_procdata_1 = 0;

void
glibtop_get_procdata_r (glibtop *server, glibtop_procdata *buf, pid_t pid)
{
	struct tbl_procinfo procinfo;
	task_basic_info_data_t taskinfo;
	struct user u;
	task_t thistask;
	int ret, info_count;

	glibtop_init ();

	memset (buf, 0, sizeof (glibtop_procdata));

	ret = table (TBL_PROCINFO, pid, (char *) &procinfo, 1,
		     sizeof (struct tbl_procinfo));

	if (ret != 1) return;

	strcpy (buf->cmd, procinfo.pi_comm);

	buf->uid = procinfo.pi_uid;
	buf->pid = procinfo.pi_pid;
	buf->ppid = procinfo.pi_ppid;
	buf->pgrp = procinfo.pi_pgrp;
	buf->tty = procinfo.pi_ttyd;
	buf->k_flags = procinfo.pi_flag;
	buf->session = procinfo.pi_session;
	buf->tpgid = procinfo.pi_tpgrp;
	buf->sigignore = procinfo.pi_sigignore;
	buf->sigcatch = procinfo.pi_sigcatch;
	buf->signal = procinfo.pi_sig;

	buf->flags [0] += _glibtop_sysdeps_procdata_0;

	if (procinfo.pi_status != PI_ACTIVE) return;

	/* From that point on, we are only interested in active processes. */

	buf->nice = getpriority (PRIO_PROCESS, pid);

	/* Get task structure. */

	ret = task_by_unix_pid (task_self(), procinfo.pi_pid, &thistask);

	if (ret != KERN_SUCCESS) return;

	/* Get taskinfo about this task. */

	info_count = TASK_BASIC_INFO_COUNT;

	ret = task_info (thistask, TASK_BASIC_INFO, (task_info_t) &taskinfo, &info_count);

	if (ret != KERN_SUCCESS) return;

	buf->priority = taskinfo.base_priority;
	buf->resident = taskinfo.resident_size;
	buf->rss = taskinfo.resident_size;
	buf->vsize = taskinfo.virtual_size;

	buf->flags [0] += BIT_SHIFT(GLIBTOP_PROCDATA_PRIORITY) +
		BIT_SHIFT(GLIBTOP_PROCDATA_RESIDENT) +
		BIT_SHIFT(GLIBTOP_PROCDATA_RSS) +
		BIT_SHIFT(GLIBTOP_PROCDATA_VSIZE);

	ret = table (TBL_UAREA, pid, (char *) &u, 1,
		     sizeof (struct user));

	if (ret != 1) return;

	buf->start_code = (unsigned long) u.u_text_start;
	buf->end_code = (unsigned long) u.u_data_start;
	buf->start_stack = (unsigned long) u.u_stack_start;

	buf->trs = u.u_tsize;
	buf->drs = u.u_dsize;

	buf->start_time = u.u_start.tv_sec;

	buf->utime = u.u_ru.ru_utime.tv_sec;
	buf->stime = u.u_ru.ru_stime.tv_sec;
	buf->cutime = u.u_cru.ru_utime.tv_sec;
	buf->cstime = u.u_cru.ru_stime.tv_sec;

	buf->flags [0] += BIT_SHIFT(GLIBTOP_PROCDATA_START_TIME) +
		BIT_SHIFT(GLIBTOP_PROCDATA_UTIME) + BIT_SHIFT(GLIBTOP_PROCDATA_STIME) +
		BIT_SHIFT(GLIBTOP_PROCDATA_CUTIME) + BIT_SHIFT(GLIBTOP_PROCDATA_CSTIME) +
		BIT_SHIFT(GLIBTOP_PROCDATA_TRS) + BIT_SHIFT(GLIBTOP_PROCDATA_DRS);

	buf->flags [1] += BIT_SHIFT(GLIBTOP_PROCDATA_START_CODE) +
		BIT_SHIFT(GLIBTOP_PROCDATA_END_CODE) +
		BIT_SHIFT(GLIBTOP_PROCDATA_START_STACK);
}
