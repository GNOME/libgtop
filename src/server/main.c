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

int main(int argc, char *argv[])
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

	/* !!! WE ARE ROOT HERE - CHANGE WITH CAUTION !!! */

	int uid, euid, gid, egid;

	uid = getuid (); euid = geteuid ();
	gid = getgid (); egid = getegid ();

	if (setreuid (euid, uid)) _exit (1);

	if (setregid (egid, gid)) _exit (1);

	/* !!! END OF SUID ROOT PART !!! */

	/* For security reasons, we temporarily drop our priviledges
	 * before doing the gettext stuff. */

	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE, GTOPLOCALEDIR);
	textdomain (PACKAGE);

	glibtop_version ();

	/* !!! WE ARE ROOT HERE - CHANGE WITH CAUTION !!! */

	setreuid (uid, euid); setregid (gid, egid);
	
	glibtop_open__r (&server, argv [0], 0, 0);

	if (setreuid (euid, uid)) _exit (1);

	if (setregid (egid, gid)) _exit (1);

	/* !!! END OF SUID ROOT PART !!! */

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
#if GLIBTOP_SUID_CPU
			glibtop_get_cpu__p (&server, &data.cpu);
#endif
			glibtop_output (sizeof (glibtop_cpu), &data.cpu);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_MEM:
#if GLIBTOP_SUID_MEM
			glibtop_get_mem__p (&server, &data.mem);
#endif
			glibtop_output (sizeof (glibtop_mem), &data.mem);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_SWAP:
#if GLIBTOP_SUID_SWAP
			glibtop_get_swap__p (&server, &data.swap);
#endif
			glibtop_output (sizeof (glibtop_swap), &data.swap);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_UPTIME:
#if GLIBTOP_SUID_UPTIME
			glibtop_get_uptime__p (&server, &data.uptime);
#endif
			glibtop_output (sizeof (glibtop_uptime), &data.uptime);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_LOADAVG:
#if GLIBTOP_SUID_LOADAVG
			glibtop_get_loadavg__p (&server, &data.loadavg);
#endif
			glibtop_output (sizeof (glibtop_loadavg), &data.loadavg);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_SHM_LIMITS:
#if GLIBTOP_SUID_SHM_LIMITS
			glibtop_get_shm_limits__p (&server, &data.shm_limits);
#endif
			glibtop_output (sizeof (glibtop_shm_limits),
					&data.shm_limits);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_MSG_LIMITS:
#if GLIBTOP_SUID_MSG_LIMITS
			glibtop_get_msg_limits__p (&server, &data.msg_limits);
#endif
			glibtop_output (sizeof (glibtop_msg_limits),
					&data.msg_limits);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_SEM_LIMITS:
#if GLIBTOP_SUID_SEM_LIMITS
			glibtop_get_sem_limits__p (&server, &data.sem_limits);
#endif
			glibtop_output (sizeof (glibtop_sem_limits),
					&data.sem_limits);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROCLIST:
#if GLIBTOP_SUID_PROCLIST
			ptr = glibtop_get_proclist__p (&server, &data.proclist);
#else
			ptr = NULL;
#endif
			glibtop_output (sizeof (glibtop_proclist),
					&data.proclist);
			glibtop_output (data.proclist.total, ptr);
			glibtop_free__r (&server, ptr);
			break;
		case GLIBTOP_CMND_PROC_STATE:
			memcpy (&pid, parameter, sizeof (pid_t));
#if GLIBTOP_SUID_PROC_STATE
			glibtop_get_proc_state__p
				(&server, &data.proc_state, pid);
#endif
			glibtop_output (sizeof (glibtop_proc_state),
					&data.proc_state);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROC_UID:
			memcpy (&pid, parameter, sizeof (pid_t));
#if GLIBTOP_SUID_PROC_UID
			glibtop_get_proc_uid__p
				(&server, &data.proc_uid, pid);
#endif
			glibtop_output (sizeof (glibtop_proc_uid),
					&data.proc_uid);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROC_MEM:
			memcpy (&pid, parameter, sizeof (pid_t));
#if GLIBTOP_SUID_PROC_MEM
			glibtop_get_proc_mem__p
				(&server, &data.proc_mem, pid);
#endif
			glibtop_output (sizeof (glibtop_proc_mem),
					&data.proc_mem);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROC_TIME:
			memcpy (&pid, parameter, sizeof (pid_t));
#if GLIBTOP_SUID_PROC_TIME
			glibtop_get_proc_time__p
				(&server, &data.proc_time, pid);
#endif
			glibtop_output (sizeof (glibtop_proc_time),
					&data.proc_time);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROC_SIGNAL:
			memcpy (&pid, parameter, sizeof (pid_t));
#if GLIBTOP_SUID_PROC_SIGNAL
			glibtop_get_proc_signal__p
				(&server, &data.proc_signal, pid);
#endif
			glibtop_output (sizeof (glibtop_proc_signal),
					&data.proc_signal);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROC_KERNEL:
			memcpy (&pid, parameter, sizeof (pid_t));
#if GLIBTOP_SUID_PROC_KERNEL
			glibtop_get_proc_kernel__p
				(&server, &data.proc_kernel, pid);
#endif
			glibtop_output (sizeof (glibtop_proc_kernel),
					&data.proc_kernel);
			glibtop_output (0, NULL);
			break;
		case GLIBTOP_CMND_PROC_SEGMENT:
			memcpy (&pid, parameter, sizeof (pid_t));
#if GLIBTOP_SUID_PROC_SEGMENT
			glibtop_get_proc_segment__p
				(&server, &data.proc_segment, pid);
#endif
			glibtop_output (sizeof (glibtop_proc_segment),
					&data.proc_segment);
			glibtop_output (0, NULL);
			break;

		}
	}
	_exit(0);
}
