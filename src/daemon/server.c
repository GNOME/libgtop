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

#include <config.h>
#include "daemon.h"

static glibtop _glibtop_global_server;
glibtop *glibtop_global_server = &_glibtop_global_server;

#include <glibtop.h>
#include <glibtop/union.h>
#include <glibtop/sysdeps.h>

#include <sys/utsname.h>

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
GLIBTOP_SUID_PROC_SEGMENT +
GLIBTOP_SUID_PROC_ARGS +
GLIBTOP_SUID_PROC_MAP +
GLIBTOP_SUID_NETLOAD +
GLIBTOP_SUID_NETLIST +
GLIBTOP_SUID_PROC_WD +
GLIBTOP_SUID_PROC_AFFINITY +
GLIBTOP_SUID_PPP;

#include <fcntl.h>
#include <locale.h>

int
main(int argc, char *argv[])
{
	struct utsname uts;
	int uid, euid, gid, egid;

	/* !!! WE ARE ROOT HERE - CHANGE WITH CAUTION !!! */

	uid = getuid (); euid = geteuid ();
	gid = getgid (); egid = getegid ();

	if (uname (&uts) < 0) _exit (1);

#ifdef _AIX
	/*
	 * [FIXME]: should be in sysdeps part ?
	 */

	if ((strcmp (uts.sysname, LIBGTOP_COMPILE_SYSTEM) != 0) ||
	    ((atol(uts.version) < atol(LIBGTOP_COMPILE_VERSION)) &&
	     (atol(uts.release) < atol(LIBGTOP_COMPILE_RELEASE))) ) {
		fprintf (stderr, "Can only run on %s %s.%s and upper\n",
			 LIBGTOP_COMPILE_SYSTEM,
			 LIBGTOP_COMPILE_VERSION,
			 LIBGTOP_COMPILE_RELEASE);
		_exit (1);
	}
#else
	if (strcmp (uts.sysname, LIBGTOP_COMPILE_SYSTEM) ||
	    strcmp (uts.release, LIBGTOP_COMPILE_RELEASE) ||
	    strcmp (uts.machine, LIBGTOP_COMPILE_MACHINE)) {
		fprintf (stderr, "Can only run on %s %s %s\n",
			 LIBGTOP_COMPILE_SYSTEM,
			 LIBGTOP_COMPILE_RELEASE,
			 LIBGTOP_COMPILE_MACHINE);
		_exit (1);
	}
#endif

	glibtop_init_p (glibtop_global_server, 0, 0);

	if (setreuid (euid, uid)) _exit (1);

	if (setregid (egid, gid)) _exit (1);

	/* !!! END OF SUID ROOT PART !!! */

	handle_slave_connection (0, 0);

	_exit (0);
}
