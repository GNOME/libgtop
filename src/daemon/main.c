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

#include <glibtop/gnuserv.h>

#include <glibtop/command.h>
#include <glibtop/version.h>
#include <glibtop/xmalloc.h>
#include <glibtop/union.h>
#include <glibtop/open.h>

#include <fcntl.h>
#include <locale.h>

#if defined(HAVE_GETDTABLESIZE)
#define GET_MAX_FDS() getdtablesize()
#else
/* Fallthrough case - please add other #elif cases above
   for different OS's as necessary */
#define GET_MAX_FDS() 256
#endif

static void
do_output (int s, glibtop_response *response, size_t data_size, const void *data)
{
#ifdef REAL_DEBUG
	fprintf (stderr, "Really writing %d bytes.\n", sizeof (glibtop_response));
#endif

	response->data_size = data_size;
	
	if (send (s, response, sizeof (glibtop_response), 0) < 0)
		glibtop_warn_io ("send");

	if (response->data_size) {
#ifdef REAL_DEBUG
		fprintf (stderr, "Writing %d bytes of data.\n", response->data_size);
#endif

		if (send (s, data, response->data_size, 0) , 0)
			glibtop_warn_io ("send");
	}
}

void
handle_socket_connection (int s)
{
	pid_t pid;
	int nread;
	size_t size;
	char parameter [BUFSIZ];
	struct timeval tv;
	glibtop_response response;
	glibtop_command cmnd;
	glibtop server;
	void *ptr;

	tv.tv_sec = 5;
	tv.tv_usec = 0;
  
	while(1) {
		nread = recv (s, &cmnd, sizeof (glibtop_command), 0);
    
		if (nread < 0) {
			glibtop_warn_io ("recv");
			return;
		}
    
		if (nread == 0)
			return;

		if (nread != sizeof (glibtop_command))
			glibtop_warn ("Received %d bytes, but expected %d\n",
				      nread, sizeof (glibtop_command));

#ifdef REAL_DEBUG
		fprintf (stderr, "Received command %d from client.\n", cmnd.command);
#endif

		if (cmnd.size >= BUFSIZ) {
			glibtop_warn ("Client sent %d bytes, but buffer is %d", cmnd.size, BUFSIZ);
			return;
		}

		memset (parameter, 0, sizeof (parameter));
    
		if (cmnd.size) {
#ifdef REAL_DEBUG
			fprintf (stderr, "Client has %d bytes of data.\n", cmnd.size);
#endif

			nread = recv (s, parameter, cmnd.size, 0);
      
			/* will return 0 if parent exits. */
      
			if (nread < 0) {
				glibtop_warn_io ("recv");
				return;
			}
    
			if (nread == 0)
				return;

			if (nread != (int) cmnd.size) {
				glibtop_warn ("Expected %d bytes but got %d",
					      cmnd.size, nread);
				return;
			}
		}
    
		switch (cmnd.command) {
		case GLIBTOP_CMND_SYSDEPS:
			response.u.sysdeps.features = GLIBTOP_SYSDEPS_ALL;
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_CPU:
			glibtop_get_cpu_l (&server, &response.u.data.cpu);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_MEM:
			glibtop_get_mem_l (&server, &response.u.data.mem);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_SWAP:
			glibtop_get_swap_l (&server, &response.u.data.swap);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_UPTIME:
			glibtop_get_uptime_l (&server, &response.u.data.uptime);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_LOADAVG:
			glibtop_get_loadavg_l (&server, &response.u.data.loadavg);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_SHM_LIMITS:
			glibtop_get_shm_limits_l (&server, &response.u.data.shm_limits);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_MSG_LIMITS:
			glibtop_get_msg_limits_l (&server, &response.u.data.msg_limits);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_SEM_LIMITS:
			glibtop_get_sem_limits_l (&server, &response.u.data.sem_limits);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_PROCLIST:
			ptr = glibtop_get_proclist_l (&server, &response.u.data.proclist);
			do_output (s, &response, response.u.data.proclist.total, ptr);
			glibtop_free_r (&server, ptr);
			break;
		case GLIBTOP_CMND_PROC_STATE:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_state_l
				(&server, &response.u.data.proc_state, pid);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_PROC_UID:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_uid_l
				(&server, &response.u.data.proc_uid, pid);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_PROC_MEM:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_mem_l
				(&server, &response.u.data.proc_mem, pid);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_PROC_TIME:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_time_l
				(&server, &response.u.data.proc_time, pid);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_PROC_SIGNAL:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_signal_l
				(&server, &response.u.data.proc_signal, pid);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_PROC_KERNEL:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_kernel_l
				(&server, &response.u.data.proc_kernel, pid);
			do_output (s, &response, 0, NULL);
			break;
		case GLIBTOP_CMND_PROC_SEGMENT:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_segment_l
				(&server, &response.u.data.proc_segment, pid);
			do_output (s, &response, 0, NULL);
			break;
		}
	}
}
