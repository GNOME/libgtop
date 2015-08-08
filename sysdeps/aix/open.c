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
#include <fcntl.h>
#include <unistd.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/open.h>
#include <glibtop/init_hooks.h>
#include "glibtop_suid.h"

/* !!! THIS FUNCTION RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

void
glibtop_init_p (glibtop *server, const unsigned long features,
                const unsigned flags)
{
	const _glibtop_init_func_t *init_fkt;

	if (server == NULL)
		glibtop_error_r (NULL, "glibtop_init_p (server == NULL)");

	if ((server->flags & _GLIBTOP_INIT_STATE_SYSDEPS) == 0) {
		glibtop_open_p (server, "glibtop", features, flags);

		for (init_fkt = _glibtop_init_hook_p; *init_fkt; init_fkt++)
			(*init_fkt) (server);
		server->flags |= _GLIBTOP_INIT_STATE_SYSDEPS;
	}
}

/* Opens pipe to gtop server. Returns 0 on success and -1 on error. */

void
glibtop_open_p (glibtop *server, const char *program_name,
		const unsigned long features, const unsigned flags)
{
	/* !!! WE ARE ROOT HERE - CHANGE WITH CAUTION !!! */

	server->machine->uid = getuid ();
	server->machine->euid = geteuid ();
	server->machine->gid = getgid ();
	server->machine->egid = getegid ();

	/* open kmem */

	server->machine->kmem_fd = open("/dev/kmem", O_RDONLY);
	if (server->machine->kmem_fd == -1)
		glibtop_error_io_r (server, "Cannot open /dev/kmem");

	/* Drop priviledges. */

	if (seteuid (server->machine->uid))
		_exit (1);

	if (setegid (server->machine->gid))
		_exit (1);

	/* !!! END OF SUID ROOT PART !!! */

	server->name = program_name;
}
