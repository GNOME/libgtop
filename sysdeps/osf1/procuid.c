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
#include <glibtop/procuid.h>

#include <sys/table.h>
#include <sys/resource.h>

#include <mach.h>
#include <mach/mach_types.h>
#include <mach/task_info.h>

/* Provides detailed information about a process. */

static const unsigned long _glibtop_sysdeps_proc_uid =
(1 << GLIBTOP_PROC_UID_UID) + (1 << GLIBTOP_PROC_UID_EUID) +
(1 << GLIBTOP_PROC_UID_GID) + (1 << GLIBTOP_PROC_UID_EGID) +
(1 << GLIBTOP_PROC_UID_PID) + (1 << GLIBTOP_PROC_UID_PPID) +
(1 << GLIBTOP_PROC_UID_PGRP) + (1 << GLIBTOP_PROC_UID_SESSION) +
(1 << GLIBTOP_PROC_UID_TTY) + (1 << GLIBTOP_PROC_UID_TPGID);

void
glibtop_get_proc_uid_r (glibtop *server, glibtop_proc_uid *buf,
			   pid_t pid)
{
	struct tbl_procinfo procinfo;
	task_basic_info_data_t taskinfo;
	int ret, info_count;
	task_t thistask;

	glibtop_init ();
	
	memset (buf, 0, sizeof (glibtop_proc_uid));
	
	/* !!! THE FOLLOWING CODE RUNS SUID ROOT - CHANGE WITH CAUTION !!! */
	
	setreuid (server->machine.uid, server->machine.euid);
	
	ret = table (TBL_PROCINFO, pid, (char *) &procinfo, 1,
		     sizeof (struct tbl_procinfo)); 
		     
	if (setreuid (server->machine.euid, server->machine.uid))
		_exit (1);
			
	/* !!! END OF SUID ROOT PART !!! */
	
	if (ret != 1) return;

	buf->uid = procinfo.pi_ruid;
	buf->euid = procinfo.pi_svuid;	
	buf->gid = procinfo.pi_rgid;
	buf->egid = procinfo.pi_svgid;	
	
	buf->pid = procinfo.pi_pid;
	buf->ppid = procinfo.pi_ppid;
	buf->pgrp = procinfo.pi_pgrp;
	buf->tty = procinfo.pi_ttyd;
	buf->session = procinfo.pi_session;
	buf->tpgid = procinfo.pi_tpgrp;
	
	buf->flags = _glibtop_sysdeps_proc_uid;
		
	/* !!! THE FOLLOWING CODE RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

	setreuid (server->machine.uid, server->machine.euid);
		
	/* Get task structure. */
	
	ret = task_by_unix_pid (task_self(), procinfo.pi_pid, &thistask);
	
	if (ret == KERN_SUCCESS) {

		/* Get taskinfo about this task. */
	
		info_count = TASK_BASIC_INFO_COUNT;
	
		ret = task_info (thistask, TASK_BASIC_INFO,
				 (task_info_t) &taskinfo, &info_count);
				 
	}
	
	if (setreuid (server->machine.euid, server->machine.uid))
		_exit (1);
			
	/* !!! END OF SUID ROOT PART !!! */
	
	if (ret != KERN_SUCCESS) return;
	
	buf->priority = taskinfo.base_priority;
	
	buf->flags += (1 << GLIBTOP_PROC_UID_PRIORITY);

	errno = 0;
	
	ret = getpriority (PRIO_PROCESS, pid);
	
	if ((ret == -1) && (errno != 0)) return;
	
	buf->nice = ret;
		
	buf->flags += (1 << GLIBTOP_PROC_UID_NICE);
}
