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
#include <glibtop/open.h>
#include <glibtop/xmalloc.h>

struct nlist _glibtop_nlist[] = {
	{ "_ccpu" },		/* 0 */
	{ "_cp_time" },		/* 1 */
	{ "_hz" },		/* 2 */
	{ "_stathz" },		/* 3 */
	{ "_averunnable" },	/* 4 */
	{ "_swaplist" },	/* 5 */
	{ "_swdevt" },		/* 6 */
	{ "_nswap" },		/* 7 */
	{ "_nswdev" },		/* 8 */
	{ "_dmmax" },		/* 9 */
	{ "_cnt" },		/* 10 */
	{ "_nextpid" },		/* 11 */
	{ 0 }
};

/* Opens pipe to gtop server. Returns 0 on success and -1 on error. */

/* !!! THIS FUNCTION RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

void
glibtop_init_p (glibtop *server, const unsigned long features,
		const unsigned flags)
{
	if (server == NULL)
		glibtop_error_r (NULL, "glibtop_init_p (server == NULL)");

	glibtop_open_p (server, "glibtop", features, flags);
}

void
glibtop_open_p (glibtop *server, const char *program_name,
		const unsigned long features, const unsigned flags)
{
	fprintf (stderr, "DEBUG (%d): glibtop_open_p ()\n", getpid ()); 

	/* !!! WE ARE ROOT HERE - CHANGE WITH CAUTION !!! */

	server->name = program_name;

	server->machine.uid = getuid ();
	server->machine.euid = geteuid ();
	server->machine.gid = getgid ();
	server->machine.egid = getegid ();

	/* initialize the kernel interface */

	server->machine.kd = kvm_open (NULL, NULL, NULL, O_RDONLY, "libgtop");
	
	if (server->machine.kd == NULL)
		glibtop_error_io_r (server, "kvm_open");
	
	/* get the list of symbols we want to access in the kernel */
	
	server->machine.nlist_count = kvm_nlist
		(server->machine.kd, _glibtop_nlist);
	
	/* On FreeBSD, kvm_nlist () returns the number of invalid
	 * entries in the nlist. */
	
	if (server->machine.nlist_count)
		glibtop_error_io_r (server, "nlist");
	
	/* Get tick frequency. */
	
	(void) _glibtop_getkval (server, _glibtop_nlist [X_STATHZ].n_value,
				 (int *) &server->machine.hz,
				 sizeof (server->machine.hz),
				 _glibtop_nlist [X_STATHZ].n_name);
	
	if (!server->machine.hz)
		(void) _glibtop_getkval
			(server, _glibtop_nlist [X_HZ].n_value,
			 (int *) &server->machine.hz,
			 sizeof (server->machine.hz),
			 _glibtop_nlist [X_STATHZ].n_name);

	/* Drop priviledges. */	
	
	if (setreuid (server->machine.euid, server->machine.uid))
		_exit (1);
	
	if (setregid (server->machine.egid, server->machine.gid))
		_exit (1);
	
	/* !!! END OF SUID ROOT PART !!! */
		
	/* Our effective uid is now those of the user invoking the server,
	 * so we do no longer have any priviledges. */

	/* NOTE: On SunOS, we do not need to be suid root, we just need to
	 * be sgid kmem.
	 *
	 * The server will only use setegid() to get back it's priviledges,
	 * so it will fail if it is suid root and not sgid kmem. */
}

/* Used internally. Fetches value from kernel. */

int
_glibtop_getkval (void *void_server, unsigned long offset, int *ptr,
		  int size, char *refstr)
{
	glibtop	*server = (glibtop *) void_server;

	fprintf (stderr, "DEBUG: kvm_read: %d - %lu - %p - %lu\n",
		 server->machine.kd, offset, ptr, size);

	if (kvm_read (server->machine.kd, offset, ptr, size) != size)
		{
			if (*refstr == '!') return 0;
			
			glibtop_error_r (server, "kvm_read(%s): %s",
					 refstr, strerror (errno));
		}
	
	return 1;
}

