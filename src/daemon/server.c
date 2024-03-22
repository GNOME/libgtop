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
#include "daemon.h"

#include <glibtop.h>
#include <glibtop/union.h>
#include <glibtop/sysdeps.h>
#include <glibtop/error.h>
#include <glibtop/machine.h>

#include <sys/utsname.h>

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

	glibtop_debug ("uid=%d euid=%d gid=%d egid=%d", getuid(), geteuid(), getgid(), getegid());

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
#ifndef __linux__
	if (strcmp (uts.sysname, LIBGTOP_COMPILE_SYSTEM) ||
	    strcmp (uts.release, LIBGTOP_COMPILE_RELEASE) ||
	    strcmp (uts.machine, LIBGTOP_COMPILE_MACHINE)) {
		fprintf (stderr, "This libgtop was compiled on %s %s %s\n",
			 LIBGTOP_COMPILE_SYSTEM,
			 LIBGTOP_COMPILE_RELEASE,
			 LIBGTOP_COMPILE_MACHINE);
                fprintf (stderr, "If you see strange problems caused by it,\n");
                fprintf (stderr, "you should recompile libgtop and dependent "
                         "applications.\n");
	}
#endif
#endif

	glibtop_machine_new (glibtop_global_server);

	glibtop_init_p (glibtop_global_server, 0, 0);

	glibtop_debug ("uid=%d euid=%d gid=%d egid=%d", getuid(), geteuid(), getgid(), getegid());

	if (setreuid (euid, uid)) _exit (1);

	if (setregid (egid, gid)) _exit (1);

	glibtop_debug ("uid=%d euid=%d gid=%d egid=%d", getuid(), geteuid(), getgid(), getegid());

	/* !!! END OF SUID ROOT PART !!! */

	handle_slave_connection (0, 0);

	_exit (0);
}
