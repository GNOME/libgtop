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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procuid.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_proc_uid =
(1L << GLIBTOP_PROC_UID_UID) + (1L << GLIBTOP_PROC_UID_EUID) +
(1L << GLIBTOP_PROC_UID_EGID) + (1L << GLIBTOP_PROC_UID_PID) +
(1L << GLIBTOP_PROC_UID_PPID) + (1L << GLIBTOP_PROC_UID_PGRP) +
(1L << GLIBTOP_PROC_UID_TPGID) + (1L << GLIBTOP_PROC_UID_PRIORITY) +
(1L << GLIBTOP_PROC_UID_NICE);

static const unsigned long _glibtop_sysdeps_proc_uid_groups =
#if LIBGTOP_VERSION_CODE >= 1001000
#if defined(__NetBSD__) && (__NetBSD_Version__ >= 104000000)
(1L << GLIBTOP_PROC_UID_NGROUPS) + (1L << GLIBTOP_PROC_UID_GROUPS);
#endif
#else
0;
#endif

/* Init function. */

void
glibtop_init_proc_uid_p (glibtop *server)
{
	server->sysdeps.proc_uid = _glibtop_sysdeps_proc_uid |
		_glibtop_sysdeps_proc_uid_groups;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_uid_p (glibtop *server, glibtop_proc_uid *buf,
			pid_t pid)
{
	struct kinfo_proc *pinfo;
	int count = 0;

#if LIBGTOP_VERSION_CODE >= 1001000
#if defined(__NetBSD__) && (__NetBSD_Version__ >= 104000000)
	struct ucred ucred;
	void *ucred_ptr;
#endif
#endif

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_PROC_UID), 0);
	
	memset (buf, 0, sizeof (glibtop_proc_uid));

	/* It does not work for the swapper task. */
	if (pid == 0) return;
	
	/* Get the process information */
	pinfo = kvm_getprocs (server->machine.kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1)) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		return;
	}

	buf->uid  = pinfo [0].kp_eproc.e_pcred.p_ruid;
	buf->euid = pinfo [0].kp_eproc.e_pcred.p_svuid;
	buf->gid  = pinfo [0].kp_eproc.e_pcred.p_rgid;
	buf->egid = pinfo [0].kp_eproc.e_pcred.p_svgid;

	buf->ppid  = pinfo [0].kp_eproc.e_ppid;
	buf->pgrp  = pinfo [0].kp_eproc.e_pgid;
	buf->tpgid = pinfo [0].kp_eproc.e_tpgid;

	buf->nice     = pinfo [0].kp_proc.p_nice;
	buf->priority = pinfo [0].kp_proc.p_priority;

	/* Set the flags for the data we're about to return*/
	buf->flags = _glibtop_sysdeps_proc_uid;

	/* Use LibGTop conditionals here so we can more easily merge this
	 * code into the LIBGTOP_STABLE_1_0 branch. */
#if LIBGTOP_VERSION_CODE >= 1001000
	/* This probably also works with other versions, but not yet
	 * tested. Please remove the conditional if this is true. */
#if defined(__NetBSD__) && (__NetBSD_Version__ >= 104000000)
	ucred_ptr = (void *) pinfo [0].kp_eproc.e_pcred.pc_ucred;

	if (ucred_ptr) {
		if (kvm_read (server->machine.kd, (unsigned long) ucred_ptr,
			      &ucred, sizeof (ucred)) != sizeof (ucred)) {
			glibtop_warn_io_r (server, "kvm_read (ucred)");
		} else {
			int count = (ucred.cr_ngroups < GLIBTOP_MAX_GROUPS) ?
				ucred.cr_ngroups : GLIBTOP_MAX_GROUPS;
			int i;

			for (i = 0; i < count; i++)
				buf->groups [i] = ucred.cr_groups [i];
			buf->ngroups = count;

			buf->flags |= _glibtop_sysdeps_proc_uid_groups;
		}
	}
#endif
#endif
}
