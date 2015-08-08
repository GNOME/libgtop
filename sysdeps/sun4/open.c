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
#include <glib.h>
#include <errno.h>

#include <glibtop.h>
#include <glibtop/open.h>

struct nlist _glibtop_nlist[] = {
#ifdef i386
    { "avenrun" },		/* 0 */
    { "ccpu" },			/* 1 */
    { "mpid" },			/* 2 */
    { "nproc" },		/* 3 */
    { "proc" },			/* 4 */
    { "total" },		/* 5 */
    { "cp_time" },		/* 6 */
    { "pages" },		/* 7 */
    { "epages" },		/* 8 */
    { "shminfo" },		/* 9 */
    { "msginfo" },		/* 10 */
    { "seminfo" },		/* 11 */
#else
    { "_avenrun" },		/* 0 */
    { "_ccpu" },		/* 1 */
    { "_mpid" },		/* 2 */
    { "_nproc" },		/* 3 */
    { "_proc" },		/* 4 */
    { "_total" },		/* 5 */
    { "_cp_time" },		/* 6 */
    { "_pages" },		/* 7 */
    { "_epages" },		/* 8 */
    { "_shminfo" },		/* 9 */
    { "_msginfo" },		/* 10 */
    { "_seminfo" },		/* 11 */
#ifdef MULTIPROCESSOR
    { "_ncpu" },
    { "_mp_time" },
#endif
#endif
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
	register int pagesize;

	/* !!! WE ARE ROOT HERE - CHANGE WITH CAUTION !!! */

	server->name = program_name;

	server->machine->uid = getuid ();
	server->machine->euid = geteuid ();
	server->machine->gid = getgid ();
	server->machine->egid = getegid ();

	/* initialize the kernel interface */

	server->machine->kd = kvm_open (NULL, NULL, NULL, O_RDONLY, "libgtop");

	if (server->machine->kd == NULL)
		glibtop_error_io_r (server, "kvm_open");

	/* get the list of symbols we want to access in the kernel */

	server->machine->nlist_count = kvm_nlist
		(server->machine->kd, _glibtop_nlist);

	if (server->machine->nlist_count < 0)
		glibtop_error_io_r (server, "nlist");

#ifdef MULTIPROCESSOR
	/* were ncpu and xp_time not found in the nlist? */

	if ((server->machine->nlist_count > 0) &&
	    (_glibtop_nlist[X_NCPU].n_type == 0) &&
	    (_glibtop_nlist[X_MP_TIME].n_type == 0)) {
		/* we were compiled on an MP system but we are not running
		 * on one, so we will pretend this didn't happen and set
		 * ncpu = 1 */
		server->machine->nlist_count -= 2;
		server->machine->ncpu = 1;
	}
#endif

#ifdef solbourne
	{
		unsigned int status, type;

		/* Get the number of CPUs on this system.  */
		syscall(SYS_getcpustatus, &status,
			&server->machine->ncpu, &type);
	}
#endif

	/* Make sure all of the symbols were found. */

	if ((server->machine->nlist_count > 0) &&
	    (_glibtop_check_nlist (server, _glibtop_nlist) > 0))
		_exit (1);

	/* Get process array stuff. */

	(void) _glibtop_getkval (server, _glibtop_nlist[X_NPROC].n_value,
				 (int *)(&server->machine->nproc),
				 sizeof (server->machine->nproc),
				 _glibtop_nlist[X_NPROC].n_name);

	(void) _glibtop_getkval (server, _glibtop_nlist[X_PROC].n_value,
				 (int *)(&server->machine->ptable_offset),
				 sizeof (server->machine->ptable_offset),
				 _glibtop_nlist[X_PROC].n_name);

	server->machine->ptable_size = (unsigned long) server->machine->nproc *
		(unsigned long) sizeof (struct proc);

	server->machine->proc_table = g_malloc
		(server, server->machine->ptable_size);

	/* This are for the memory statistics. */

	(void) _glibtop_getkval (server, _glibtop_nlist[X_PAGES].n_value,
				 (int *)(&server->machine->pages),
				 sizeof (server->machine->pages),
				 _glibtop_nlist[X_PAGES].n_name);

	(void) _glibtop_getkval (server, _glibtop_nlist[X_EPAGES].n_value,
				 (int *)(&server->machine->epages),
				 sizeof (server->machine->epages),
				 _glibtop_nlist[X_EPAGES].n_name);

	server->machine->bytesize = server->machine->epages -
		server->machine->pages;
	server->machine->count = server->machine->bytesize /
		sizeof (struct page);

	server->machine->physpage = (struct page *)
		g_malloc (server->machine->bytesize);

	/* get the page size with "getpagesize" and
	 * calculate pageshift from it */

	pagesize = getpagesize();

	server->machine->pageshift = 0;

	while (pagesize > 1) {
		server->machine->pageshift++;
		pagesize >>= 1;
	}

	/* we only need the amount of log(2)1024 for our conversion */

	server->machine->pageshift -= LOG1024;

	/* Drop priviledges. */

	if (setreuid (server->machine->euid, server->machine->uid))
		_exit (1);

	if (setregid (server->machine->egid, server->machine->gid))
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

/* Used internally. Returns number of symbols that cannot be found in
 * the nlist. */

int
_glibtop_check_nlist (void *server, register struct nlist *nlst)
{
	register int not_found;

	/* check to see if we got ALL the symbols we requested */
	/* this will write one line to stderr for every symbol not found */

	not_found = 0;

	while (nlst->n_name != NULL) {

#ifdef i386
		if (nlst->n_value == 0) {
			glibtop_error_r (server,
					 "kernel: no symbol named `%s'",
					 nlst->n_name);
			not_found++;
		}
#else
		if (nlst->n_type == 0) {
			glibtop_error_r (server,
					 "kernel: no symbol named `%s'",
					 nlst->n_name);
			not_found++;
		}
#endif

		nlst++;
	}

	return not_found;
}

/* Used internally. Fetches value from kernel. */

int
_glibtop_getkval (void *void_server, unsigned long offset, int *ptr,
		  int size, char *refstr)
{
	glibtop	*server = (glibtop *) void_server;

	if (kvm_read (server->machine->kd, offset, ptr, size) != size)
		{
			if (*refstr == '!') return 0;

			glibtop_error_r (server, "kvm_read(%s): %s",
					 refstr, g_strerror (errno));
		}

	return 1;
}

/* Used internally. Reads process table from kernel. */

void
_glibtop_read_proc_table (void *void_server)
{
	glibtop *server = (glibtop *) void_server;

	/* !!! THE FOLLOWING CODE RUNS SGID KMEM - CHANGE WITH CAUTION !!! */

	setregid (server->machine->gid, server->machine->egid);

	/* Read process table from kernel. */

	(void) _glibtop_getkval (server, server->machine->ptable_offset,
				 (int *) server->machine->proc_table,
				 (size_t) server->machine->ptable_size,
				 _glibtop_nlist[X_PROC].n_name);

	if (setregid (server->machine->egid, server->machine->gid))
		_exit (1);

	/* !!! END OF SGID KMEM PART !!! */
}

/* Used internally. Finds pid in process table. */

struct proc *
_glibtop_find_pid (void *void_server, pid_t pid)
{
	register struct proc *pp;
	register int i;

	glibtop *server = (glibtop *) void_server;

	for (pp = server->machine->proc_table, i = 0;
	     i < server->machine->nproc; pp++, i++) {
		if ((pp->p_stat != 0) && (pp->p_pid == pid))
			return pp;
	}

	return NULL;
}
