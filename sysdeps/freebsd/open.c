/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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
#include <glibtop/open.h>
#include <glibtop/xmalloc.h>

/* !!! THIS FUNCTION RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

void
glibtop_init_p (glibtop *server, const unsigned long features,
		const unsigned flags)
{
	glibtop_init_func_t *init_fkt;

	if (server == NULL)
		glibtop_error_r (NULL, "glibtop_init_p (server == NULL)");

	/* Do the initialization, but only if not already initialized. */

	if ((server->flags & _GLIBTOP_INIT_STATE_SYSDEPS) == 0) {
		glibtop_open_p (server, "glibtop", features, flags);

		for (init_fkt = _glibtop_init_hook_p; *init_fkt; init_fkt++)
			(*init_fkt) (server);
		
		server->flags |= _GLIBTOP_INIT_STATE_SYSDEPS;
	}
}

void
glibtop_open_p (glibtop *server, const char *program_name,
		const unsigned long features, const unsigned flags)
{
#ifdef DEBUG
	fprintf (stderr, "DEBUG (%d): glibtop_open_p ()\n", getpid ()); 
#endif

	/* !!! WE ARE ROOT HERE - CHANGE WITH CAUTION !!! */

	server->name = program_name;

	server->machine.uid = getuid ();
	server->machine.euid = geteuid ();
	server->machine.gid = getgid ();
	server->machine.egid = getegid ();

	server->os_version_code = __FreeBSD_version;
  
	/* Setup machine-specific data */
	server->machine.kd = kvm_open (NULL, NULL, NULL, O_RDONLY, "kvm_open");
	
	if (server->machine.kd == NULL)
		glibtop_error_io_r (server, "kvm_open");
	
	/* Drop priviledges. */	

	if (setreuid (server->machine.euid, server->machine.uid))
		_exit (1);
	
	if (setregid (server->machine.egid, server->machine.gid))
		_exit (1);
	
	/* !!! END OF SUID ROOT PART !!! */
		
	/* Our effective uid is now those of the user invoking the server,
	 * so we do no longer have any priviledges. */

	/* NOTE: On FreeBSD, we do not need to be suid root, we just need to
	 * be sgid kmem.
	 *
	 * The server will only use setegid() to get back it's priviledges,
	 * so it will fail if it is suid root and not sgid kmem. */
}
