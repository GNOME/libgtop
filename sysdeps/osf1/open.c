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

#include <glibtop/open.h>

#include <unistd.h>
#include <sys/types.h>

/* Opens pipe to gtop server. Returns 0 on success and -1 on error. */

/* !!! THIS FUNCTION RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

void
glibtop_open (glibtop *server, const char *program_name)
{
	/* !!! WE ARE ROOT HERE - CHANGE WITH CAUTION !!! */

	memset (server, 0, sizeof (glibtop));
	server->name = program_name;
	
	server->machine.uid = getuid ();
	server->machine.euid = geteuid ();
	server->machine.gid = getgid ();
	server->machine.egid = getegid ();

        server->machine.proctable_entries = table (TBL_PROCINFO, 0, NULL, INT_MAX, 0);
        
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
