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
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procstate.h>

#include <glibtop_suid.h>

#include <mach.h>
#include <mach/mach_types.h>
#include <mach/task_info.h>

static const unsigned long _glibtop_sysdeps_proc_state =
(1L << GLIBTOP_PROC_STATE_CMD) +
(1L << GLIBTOP_PROC_STATE_UID) + (1L << GLIBTOP_PROC_STATE_GID) +
(1L << GLIBTOP_PROC_STATE_RUID) + (1L << GLIBTOP_PROC_STATE_RGID);

/* Init function. */

void
_glibtop_init_proc_state_p (glibtop *server)
{
	server->sysdeps.proc_state = _glibtop_sysdeps_proc_state;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_state_p (glibtop *server, glibtop_proc_state *buf,
			  pid_t pid)
{
	struct tbl_procinfo procinfo;
	int minim_state = 99, ret;
	task_t thistask;

	glibtop_init_p (server, GLIBTOP_SYSDEPS_PROC_STATE, 0);

	memset (buf, 0, sizeof (glibtop_proc_state));

	/* !!! THE FOLLOWING CODE RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

	glibtop_suid_enter (server);

	ret = table (TBL_PROCINFO, pid, (char *) &procinfo, 1,
		     sizeof (struct tbl_procinfo));

	glibtop_suid_leave (server);

	/* !!! END OF SUID ROOT PART !!! */

	if (ret != 1) return;

	/* Check whether the process actually exists. */
	if (procinfo.pi_status == PI_EMPTY) return;

	/* Check whether it is not a zombie. */
	if (procinfo.pi_status == PI_ZOMBIE) {
		buf->state = GLIBTOP_PROCESS_ZOMBIE;
		buf->flags = (1L << GLIBTOP_PROC_STATE_STATE);
		return;
	}

	g_strlcpy (buf->cmd, procinfo.pi_comm, sizeof buf->cmd);

	buf->uid = procinfo.pi_svuid;
	buf->gid = procinfo.pi_svgid;
	buf->ruid = procinfo.pi_ruid;
	buf->rgid = procinfo.pi_rgid;

	buf->flags = _glibtop_sysdeps_proc_state;

	/* !!! THE FOLLOWING CODE RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

	glibtop_suid_enter (server);

	/* Get task structure. */
	ret = task_by_unix_pid (task_self(), procinfo.pi_pid, &thistask);

	if (ret == KERN_SUCCESS) {
		thread_array_t			threadarr;
		unsigned int			threadarr_l;
		thread_basic_info_t		threadinfo;
		thread_basic_info_data_t	threadinfodata;
		int				j;

		/* Get thread array. */
		(void) task_threads (thistask, &threadarr, &threadarr_l);

		threadinfo = &threadinfodata;
		for (j = 0; j < threadarr_l; j++) {
			unsigned int		threadinfo_l = THREAD_BASIC_INFO_COUNT;
			int			tret;

			tret = thread_info (threadarr [j], THREAD_BASIC_INFO,
					    (thread_info_t) threadinfo, &threadinfo_l);

			if (tret == KERN_SUCCESS) {
          			if (minim_state > threadinfo->run_state)
					minim_state=threadinfo->run_state;
			}
		}
	}

	glibtop_suid_leave (server);

	/* !!! END OF SUID ROOT PART !!! */

	if (ret != KERN_SUCCESS) return;

	switch (minim_state) {
	case TH_STATE_RUNNING:
		buf->state = GLIBTOP_PROCESS_RUNNING;
		break;
	case TH_STATE_UNINTERRUPTIBLE:
		buf->state = GLIBTOP_PROCESS_UNINTERRUPTIBLE;
		break;
	case TH_STATE_WAITING:
		buf->state = GLIBTOP_PROCESS_INTERRUPTIBLE;
		break;
	case TH_STATE_STOPPED:
	case TH_STATE_HALTED:
		buf->state = GLIBTOP_PROCESS_STOPPED;
		break;
	default:
		if (ret != KERN_SUCCESS)
			buf->state = GLIBTOP_PROCESS_ZOMBIE;
		break;
	}

	if (buf->state)
		buf->flags |= (1L << GLIBTOP_PROC_STATE_STATE);
}
