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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procuid.h>

static const unsigned long _glibtop_sysdeps_proc_uid =
(1 << GLIBTOP_PROC_UID_UID) + (1 << GLIBTOP_PROC_UID_EUID) +
(1 << GLIBTOP_PROC_UID_GID) + (1 << GLIBTOP_PROC_UID_EGID) +
(1 << GLIBTOP_PROC_UID_PID) + (1 << GLIBTOP_PROC_UID_PPID) +
(1 << GLIBTOP_PROC_UID_PGRP) + (1 << GLIBTOP_PROC_UID_SESSION) +
(1 << GLIBTOP_PROC_UID_TTY) + (1 << GLIBTOP_PROC_UID_TPGID) +
(1 << GLIBTOP_PROC_UID_PRIORITY) + (1 << GLIBTOP_PROC_UID_NICE);

#define LINUX_VERSION(x,y,z)	(0x10000*(x) + 0x100*(y) + z)

/* Provides detailed information about a process. */

void
glibtop_get_proc_uid_s (glibtop *server, glibtop_proc_uid *buf, pid_t pid)
{
	char buffer [BUFSIZ], input [BUFSIZ], *tmp;
	int fd = 0, nread;
	
	glibtop_init_r (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_proc_uid));

	if (pid == 0) {
		/* Client is only interested in the flags. */
		buf->flags = _glibtop_sysdeps_proc_uid;
		return;
	}

	sprintf (input, "/proc/%d/status", pid);
		
	fd = open (input, O_RDONLY);
	if (fd == -1)
		glibtop_error_r (server, "open (%s): %s",
				 input, strerror (errno));

	nread = read (fd, buffer, BUFSIZ);
	if (nread == -1)
		glibtop_error_r (server, "read (%s): %s",
				 input, strerror (errno));
	
	buffer [nread] = 0;
	close (fd);

	/* Search substring 'Pid:' */

	tmp = strstr (buffer, "Pid:");

	if (tmp == NULL) return;

	sscanf (tmp, "\nPid: %u\nPPid: %u\nUid: %u %u %*u %*u\n"
		"Gid: %u %u %*u %*u\n", &buf->pid, &buf->ppid,
		&buf->uid, &buf->euid, &buf->gid, &buf->egid);
	
	sprintf (input, "/proc/%d/stat", pid);
		
	fd = open (input, O_RDONLY);
	if (fd == -1)
		glibtop_error_r (server, "open (%s): %s",
				 input, strerror (errno));
		
	nread = read (fd, buffer, BUFSIZ);
	if (nread == -1)
		glibtop_error_r (server, "read (%s): %s",
				 input, strerror (errno));
		
	buffer [nread] = 0;
	close (fd);
	
	/* This is from guile-utils/gtop/proc/readproc.c */
	
	/* split into "PID (cmd" and "<rest>" */
	tmp = strrchr (buffer, ')');
	*tmp = '\0';		/* replace trailing ')' with NUL */
	/* parse these two strings separately, skipping the leading "(". */
	sscanf(tmp + 2,		/* skip space after ')' too */
	       "%*c %*d %d %d %d %d %*u %*u %*u %*u %*u "
	       "%*d %*d %*d %*d %d %d",
	       &buf->pgrp, &buf->session, &buf->tty, &buf->tpgid,
	       &buf->priority, &buf->nice);

	if (buf->tty == 0)
		/* the old notty val, update elsewhere bef. moving to 0 */
		buf->tty = -1;

	if (server->os_version_code < LINUX_VERSION(1,3,39)) {
		/* map old meanings to new */
		buf->priority = 2*15 - buf->priority;
		buf->nice = 15 - buf->nice;
	}
	if (server->os_version_code < LINUX_VERSION(1,1,30) && buf->tty != -1)
		/* when tty wasn't full devno */
		buf->tty = 4*0x100 + buf->tty;
	
	buf->flags = _glibtop_sysdeps_proc_uid;
}
