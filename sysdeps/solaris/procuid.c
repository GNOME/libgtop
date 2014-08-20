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
#include <glibtop/procuid.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proc_uid_psinfo =
(1L << GLIBTOP_PROC_UID_EUID) + (1L << GLIBTOP_PROC_UID_UID) +
(1L << GLIBTOP_PROC_UID_EGID) + (1L << GLIBTOP_PROC_UID_GID) +
(1L << GLIBTOP_PROC_UID_PID) + (1L << GLIBTOP_PROC_UID_PPID) +
(1L << GLIBTOP_PROC_UID_PGRP) + (1L << GLIBTOP_PROC_UID_SESSION) +
(1L << GLIBTOP_PROC_UID_TTY) + (1L << GLIBTOP_PROC_UID_PRIORITY) +
(1L << GLIBTOP_PROC_UID_NICE);
static const unsigned long _glibtop_sysdeps_proc_uid_prcred =
#if LIBGTOP_VERSION_CODE >= 1001002
#ifdef HAVE_PROCFS_H
(1L << GLIBTOP_PROC_UID_GROUPS) +
#endif
(1L << GLIBTOP_PROC_UID_SUID) + (1L << GLIBTOP_PROC_UID_SGID) +
(1L << GLIBTOP_PROC_UID_NGROUPS);
#else
0;
#endif
/* Init function. */

void
_glibtop_init_proc_uid_s (glibtop *server)
{
	server->sysdeps.proc_uid = _glibtop_sysdeps_proc_uid_psinfo +
	   			   _glibtop_sysdeps_proc_uid_prcred;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_uid_s (glibtop *server, glibtop_proc_uid *buf, pid_t pid)
{
#if LIBGTOP_VERSION_CODE >= 1001002
	struct prcred prcred;
#endif
#ifdef HAVE_PROCFS_H
	struct psinfo psinfo;
#if LIBGTOP_VERSION_CODE >= 1001002
	gid_t groups[GLIBTOP_MAX_GROUPS];
#endif
#else
	struct prpsinfo psinfo;
	gid_t groups[1];	/* dummy for consistent function prototype */
#endif

	memset (buf, 0, sizeof (glibtop_proc_uid));

	if (glibtop_get_proc_data_psinfo_s (server, &psinfo, pid))
		return;

	buf->euid = psinfo.pr_euid;
	buf->uid = psinfo.pr_uid;
	buf->egid = psinfo.pr_egid;
	buf->gid = psinfo.pr_gid;

	buf->pid = psinfo.pr_pid;
	buf->ppid = psinfo.pr_ppid;
#ifdef HAVE_PROCFS_H
	buf->pgrp = psinfo.pr_pgid;
#else
	buf->pgrp = psinfo.pr_pgrp;
#endif

	buf->session = psinfo.pr_sid;
	buf->tty = psinfo.pr_ttydev;

#ifdef HAVE_PROCFS_H
	buf->priority = psinfo.pr_lwp.pr_pri;
	buf->nice = psinfo.pr_lwp.pr_nice - NZERO;
#else
	buf->priority = psinfo.pr_pri;
	buf->nice = psinfo.pr_nice - NZERO;
#endif

	buf->flags = _glibtop_sysdeps_proc_uid_psinfo;

#if LIBGTOP_VERSION_CODE >= 1001002
	if(glibtop_get_proc_credentials_s(server, &prcred, groups, pid))
		return;

	buf->suid = prcred.pr_suid;
	buf->sgid = prcred.pr_sgid;
	buf->ngroups = (prcred.pr_ngroups <= GLIBTOP_MAX_GROUPS) ?
	   		prcred.pr_ngroups : GLIBTOP_MAX_GROUPS;

#ifdef HAVE_PROCFS_H
	if(sizeof(int) == sizeof(gid_t))
	   	memcpy(buf->groups, &groups, buf->ngroups * sizeof(gid_t));
	else
	{
	   	int i;

		for(i = 0; i < buf->ngroups; ++i)
		   	buf->groups[i] = groups[i];
	}
#endif
#endif

	buf->flags += _glibtop_sysdeps_proc_uid_prcred;
}
