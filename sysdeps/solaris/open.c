/* $Id$ */

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

#include <glibtop/open.h>

/* Opens pipe to gtop server. Returns 0 on success and -1 on error. */

void
glibtop_open_s (glibtop *server, const char *program_name,
		const unsigned long features, const unsigned flags)
{
    kstat_t *ksp;

    server->name = program_name;

    server->machine.kc = kstat_open ();

    for (ksp = server->machine.kc->kc_chain; ksp != NULL; ksp = ksp->ks_next) {
	if (!strcmp (ksp->ks_class, "vm") && !strcmp (ksp->ks_name, "vminfo")) {
	    server->machine.vminfo_kstat = ksp;
	    kstat_read (server->machine.kc, ksp, &server->machine.vminfo);
	    server->machine.vminfo_snaptime = ksp->ks_snaptime;
	    continue;
	}

	if (!strcmp (ksp->ks_class, "misc") && !strncmp (ksp->ks_name, "cpu_stat", 8)) {
	    int cpu;

	    if ((sscanf (ksp->ks_name+8, "%d", &cpu) != 1) || (cpu > 63))
		continue;

	    if (cpu >= server->ncpu)
		server->ncpu = cpu+1;

	    server->machine.cpu_stat_kstat [cpu] = ksp;
	    continue;
	}
    }

    if (!server->machine.kc)
	glibtop_error_io_r (server, "kstat_open ()");

    fprintf (stderr, "Sleeping 2 seconds, please wait ...\n");
    sleep (2);
}
