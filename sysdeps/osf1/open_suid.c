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
#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/init_hooks.h>
#include <glibtop_suid.h>

#include <unistd.h>
#include <sys/types.h>

/* !!! THIS FUNCTION RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

void
glibtop_init_p (glibtop *server, const unsigned long features,
		const unsigned flags)
{
	const _glibtop_init_func_t *init_fkt;

	if (server == NULL)
		glibtop_error_r (NULL, "glibtop_init_p (server == NULL)");

	/* Do the initialization, but only if not already initialized. */

	if ((server->flags & _GLIBTOP_INIT_STATE_INIT) == 0) {
		glibtop_open_p (server, "glibtop", features, flags);

		for (init_fkt = _glibtop_init_hook_p; *init_fkt; init_fkt++)
			(*init_fkt) (server);

		server->flags |= _GLIBTOP_INIT_STATE_INIT;
	}
}

/* !!! THIS FUNCTION RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

void
glibtop_open_p (glibtop *server, const char *program_name,
		const unsigned long features, const unsigned flags)
{
	/* !!! WE ARE ROOT HERE - CHANGE WITH CAUTION !!! */

	server->name = program_name;

	server->machine.uid = getuid ();
	server->machine.euid = geteuid ();
	server->machine.gid = getgid ();
	server->machine.egid = getegid ();

	server->machine.proctable_entries = table
		(TBL_PROCINFO, 0, NULL, INT_MAX, 0);

	/* Drop priviledges; we only become root when necessary.

	   setreuid (ruid, euid) - set real and effective user id;
	   setregid (rgid, egid) - set real and effective group id;

	 */

	if (setreuid (server->machine.euid, server->machine.uid))
		_exit (1);

	if (setregid (server->machine.egid, server->machine.gid))
		_exit (1);

	/* !!! END OF SUID ROOT PART !!! */

	/* Our effective uid is now those of the user invoking the server,
	   so we do no longer have any priviledges.
	 */
}
