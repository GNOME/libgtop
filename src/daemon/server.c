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

#include "daemon.h"

static glibtop _glibtop_global_server;
glibtop *glibtop_global_server = &_glibtop_global_server;

#include <glibtop.h>
#include <glibtop/union.h>
#include <glibtop/sysdeps.h>

const unsigned long glibtop_server_features =
GLIBTOP_SUID_CPU +
GLIBTOP_SUID_MEM +
GLIBTOP_SUID_SWAP +
GLIBTOP_SUID_UPTIME +
GLIBTOP_SUID_LOADAVG +
GLIBTOP_SUID_SHM_LIMITS +
GLIBTOP_SUID_MSG_LIMITS +
GLIBTOP_SUID_SEM_LIMITS +
GLIBTOP_SUID_PROCLIST +
GLIBTOP_SUID_PROC_STATE +
GLIBTOP_SUID_PROC_UID +
GLIBTOP_SUID_PROC_MEM +
GLIBTOP_SUID_PROC_TIME +
GLIBTOP_SUID_PROC_SIGNAL +
GLIBTOP_SUID_PROC_KERNEL +
GLIBTOP_SUID_PROC_SEGMENT;

#include <fcntl.h>
#include <locale.h>

#if defined(HAVE_GETDTABLESIZE)
#define GET_MAX_FDS() getdtablesize()
#else
/* Fallthrough case - please add other #elif cases above
   for different OS's as necessary */
#define GET_MAX_FDS() 256
#endif

int
main(int argc, char *argv[])
{
	int fd, max_fd;

	/* !!! WE ARE ROOT HERE - CHANGE WITH CAUTION !!! */

	int uid, euid, gid, egid;

	uid = getuid (); euid = geteuid ();
	gid = getgid (); egid = getegid ();

	glibtop_open_p (glibtop_global_server, argv [0], 0, 0);

	if (setreuid (euid, uid)) _exit (1);

	if (setregid (egid, gid)) _exit (1);

	/* !!! END OF SUID ROOT PART !!! */

	/* close all file descriptors except ones used by the pipes (0 and 1). */
	max_fd = GET_MAX_FDS();
	for(fd = 3 /* The first fd after the pipes */; fd < max_fd; fd++)
		close(fd);

	handle_slave_connection (0, 0);

	_exit (0);
}
