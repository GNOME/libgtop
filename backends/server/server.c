/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

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

#include "server.h"

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
GLIBTOP_SUID_NETINFO +
GLIBTOP_SUID_NETLOAD +
GLIBTOP_SUID_INTERFACE_NAMES +
GLIBTOP_SUID_PPP;

#include <fcntl.h>
#include <locale.h>

int
main(int argc, char *argv[])
{
    struct utsname uts;
    int uid, euid, gid, egid;
    glibtop *server;

    /* !!! WE ARE ROOT HERE - CHANGE WITH CAUTION !!! */

    uid = getuid (); euid = geteuid ();
    gid = getgid (); egid = getegid ();

    if (uname (&uts) < 0) _exit (1);

    if (strcmp (uts.sysname, LIBGTOP_COMPILE_SYSTEM) ||
	strcmp (uts.release, LIBGTOP_COMPILE_RELEASE) ||
	strcmp (uts.machine, LIBGTOP_COMPILE_MACHINE)) {
	fprintf (stderr, "Can only run on %s %s %s\n",
		 LIBGTOP_COMPILE_SYSTEM,
		 LIBGTOP_COMPILE_RELEASE,
		 LIBGTOP_COMPILE_MACHINE);
	_exit (1);
    }

    server = glibtop_global_server;
	
    if (!server->_priv)
	server->_priv = glibtop_calloc_r
	    (server, 1, sizeof (glibtop_server_private));

    glibtop_init_p (server, 0, 0);

    if (setreuid (euid, uid)) _exit (1);

    if (setregid (egid, gid)) _exit (1);

    /* !!! END OF SUID ROOT PART !!! */

    handle_slave_connection (0, 0);

    _exit (0);
}

void
handle_slave_connection (int input, int output)
{
    glibtop_command _cmnd, *cmnd = &_cmnd;
    glibtop *server = glibtop_global_server;

    // glibtop_send_version_i (glibtop_global_server, output);

    while (do_read (input, cmnd, sizeof (glibtop_command))) {
	size_t recv_size = 0, send_size = 0, recv_data_size = 0;
	void *recv_ptr = NULL, *recv_data_ptr = NULL, *data_ptr = NULL;
	char parameter [BUFSIZ];
	int retval, func_retval;
	glibtop_response resp;

#ifdef DEBUG
	fprintf (stderr, "Slave %d received command "
		 "%ld from client.\n", getpid (), (long) cmnd->command);
#endif

	if (cmnd->send_size >= BUFSIZ)
	    glibtop_error ("Client sent %d bytes, but buffer is %d",
			   cmnd->send_size, BUFSIZ);
	else if (cmnd->param_size >= BUFSIZ)
	    glibtop_error ("Client sent %d bytes, but buffer is %d",
			   cmnd->param_size, BUFSIZ);

	memset (parameter, 0, sizeof (parameter));

	if (cmnd->send_size) {
#ifdef DEBUG
	    fprintf (stderr, "Client has %d bytes of data.\n",
		     cmnd->send_size);
#endif

	    send_size = cmnd->send_size;
	    do_read (input, parameter, send_size);
	} else if (cmnd->param_size) {
#ifdef DEBUG
	    fprintf (stderr, "Client has %d bytes of parameter data.\n",
		     cmnd->param_size);
#endif
	    send_size = cmnd->param_size;
	    memcpy (parameter, cmnd->parameter, send_size);
	}

	if (cmnd->data_size) {
#ifdef DEBUG
	    fprintf (stderr, "CLIENT has %d bytes of extra data for us.\n",
		     cmnd->data_size);
#endif

	    data_ptr = glibtop_malloc_r (server, cmnd->data_size);
	    do_read (input, data_ptr, cmnd->data_size);
	}

	retval = glibtop_demarshal_func_i (server, NULL,
					   cmnd->command,
					   parameter, send_size,
					   data_ptr, cmnd->data_size,
					   &recv_ptr, &recv_size,
					   &recv_data_ptr, &recv_data_size,
					   &func_retval);

#ifdef DEBUG
	fprintf (stderr, "Retval %d / %d - %p - %d\n",
		 retval, func_retval, recv_ptr, recv_size);

	if (recv_data_size) {
	    fprintf (stderr, "Returning %d bytes of data from %p.\n",
		     recv_data_size, recv_data_ptr);
	}
#endif

	memset (&resp, 0, sizeof (glibtop_response));

	resp.retval = retval;
	resp.glibtop_errno = func_retval;
	resp.recv_size = recv_size;
	resp.data_size = recv_data_size;

	do_output (output, &resp, recv_ptr, recv_data_ptr);
    }		
}
