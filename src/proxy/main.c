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

#include <glibtop/command.h>
#include <glibtop/version.h>
#include <glibtop/xmalloc.h>
#include <glibtop/union.h>
#include <glibtop/open.h>

#include <fcntl.h>

#if defined(HAVE_GETDTABLESIZE)
#define GET_MAX_FDS() getdtablesize()
#else
/* Fallthrough case - please add other #elif cases above
   for different OS's as necessary */
#define GET_MAX_FDS() 256
#endif

int
main(int argc, char *argv[])
{
	pid_t pid;
	int fd, max_fd, nread;
	size_t size, dsize;
	char parameter [BUFSIZ];
	glibtop_union data;
	glibtop_command cmnd;
	glibtop_sysdeps sysdeps;
	glibtop server;
	void *ptr;

	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE, GTOPLOCALEDIR);
	textdomain (PACKAGE);

	glibtop_version ();

	glibtop_open (&server, argv [0], 0, GLIBTOP_OPEN_NO_OVERRIDE);

	/* close all file descriptors except ones used by the pipes (0 and 1). */
	max_fd = GET_MAX_FDS();
	for(fd = 3 /* The first fd after the pipes */; fd < max_fd; fd++)
		close(fd);
	
	while(1) {
		/* block on read from client */
		/* fprintf (stderr, "waiting for input ...\n"); */
		nread = read (0, &size, sizeof (size_t));

		/* will return 0 if parent exits. */

		if (nread == 0)
			_exit(0);

		if (nread != sizeof (size_t))
			_exit(1);

		if (size != sizeof (glibtop_command))
			_exit(2);

		nread = read (0, &cmnd, size);
		
		/* will return 0 if parent exits. */

		if (nread == 0)
			_exit(0);
		
		if (nread < 0)
			_exit(1);

		nread = read (0, &dsize, sizeof (size_t));

		/* will return 0 if parent exits. */

		if (nread == 0)
			_exit(0);
		
		if (nread < 0)
			_exit(1);

		if (dsize >= BUFSIZ)
			_exit(3);

		memset (parameter, 0, sizeof (parameter));

		if (dsize) {
			nread = read (0, parameter, dsize);
		
			/* will return 0 if parent exits. */

			if (nread == 0)
				_exit(0);
		
			if (nread < 0)
				_exit(1);
		}

		switch (cmnd.command) {
		case GLIBTOP_CMND_SYSDEPS:
			sysdeps.features = GLIBTOP_SYSDEPS_ALL;
			glibtop_output (sizeof (glibtop_sysdeps), &sysdeps);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_CPU:
			glibtop_get_cpu__l (&server, &data.cpu);
			glibtop_output (sizeof (glibtop_cpu), &data.cpu);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_MEM:
			glibtop_get_mem__l (&server, &data.mem);
			glibtop_output (sizeof (glibtop_mem), &data.mem);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_SWAP:
			glibtop_get_swap__l (&server, &data.swap);
			glibtop_output (sizeof (glibtop_swap), &data.swap);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_UPTIME:
			glibtop_get_uptime__l (&server, &data.uptime);
			glibtop_output (sizeof (glibtop_uptime), &data.uptime);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_LOADAVG:
			glibtop_get_loadavg__l (&server, &data.loadavg);
			glibtop_output (sizeof (glibtop_loadavg), &data.loadavg);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_SHM_LIMITS:
			glibtop_get_shm_limits__l (&server, &data.shm_limits);
			glibtop_output (sizeof (glibtop_shm_limits),
					&data.shm_limits);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_MSG_LIMITS:
			glibtop_get_msg_limits__l (&server, &data.msg_limits);
			glibtop_output (sizeof (glibtop_msg_limits),
					&data.msg_limits);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_SEM_LIMITS:
			glibtop_get_sem_limits__l (&server, &data.sem_limits);
			glibtop_output (sizeof (glibtop_sem_limits),
					&data.sem_limits);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROCLIST:
			ptr = glibtop_get_proclist__l (&server, &data.proclist);
			glibtop_output (sizeof (glibtop_proclist),
					&data.proclist);
			glibtop_output (data.proclist.total, ptr);
			glibtop_free__r (&server, ptr);
			break;
		case GLIBTOP_CMND_PROC_STATE:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_state__l
				(&server, &data.proc_state, pid);
			glibtop_output (sizeof (glibtop_proc_state),
					&data.proc_state);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROC_UID:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_uid__l
				(&server, &data.proc_uid, pid);
			glibtop_output (sizeof (glibtop_proc_uid),
					&data.proc_uid);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROC_MEM:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_mem__l
				(&server, &data.proc_mem, pid);
			glibtop_output (sizeof (glibtop_proc_mem),
					&data.proc_mem);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROC_TIME:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_time__l
				(&server, &data.proc_time, pid);
			glibtop_output (sizeof (glibtop_proc_time),
					&data.proc_time);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROC_SIGNAL:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_signal__l
				(&server, &data.proc_signal, pid);
			glibtop_output (sizeof (glibtop_proc_signal),
					&data.proc_signal);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROC_KERNEL:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_kernel__l
				(&server, &data.proc_kernel, pid);
			glibtop_output (sizeof (glibtop_proc_kernel),
					&data.proc_kernel);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROC_SEGMENT:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_segment__l
				(&server, &data.proc_segment, pid);
			glibtop_output (sizeof (glibtop_proc_segment),
					&data.proc_segment);
			glibtop_output (0, NULL);
			break;

		}
	}
	_exit(0);
}
