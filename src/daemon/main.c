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

#include "daemon.h"

#ifdef LIBGTOP_ENABLE_DEBUG
#ifndef PARENT_DEBUG
#define PARENT_DEBUG 1
#endif
#ifndef DEBUG
#define DEBUG 1
#endif
#endif

void
handle_parent_connection (int s)
{
	glibtop *server = glibtop_global_server;
	glibtop_response _resp, *resp = &_resp;
	glibtop_command _cmnd, *cmnd = &_cmnd;
	glibtop_mountentry *mount_list;
	char parameter [BUFSIZ];
	unsigned short device;
	int64_t *param_ptr;
	int all_fs;
	pid_t pid;
	void *ptr;

	glibtop_send_version (glibtop_global_server, s);

	fprintf (stderr, "Parent features = %lu\n", glibtop_server_features);

#ifdef DEBUG
	fprintf (stderr, "SIZEOF: %u - %u - %u - %u - %u - %u\n",
		 sizeof (glibtop_command), sizeof (glibtop_response),
		 sizeof (glibtop_mountentry), sizeof (glibtop_union),
		 sizeof (glibtop_sysdeps), sizeof (glibtop_response_union));
#endif

	while (do_read (s, cmnd, sizeof (glibtop_command))) {
#ifdef PARENT_DEBUG
		fprintf (stderr, "Parent (%d) received command %d from client.\n",
			 getpid (), (int) cmnd->command);
#endif

		if (cmnd->data_size >= BUFSIZ) {
			glibtop_warn ("Client sent %d bytes, but buffer is %d",
				      cmnd->data_size, BUFSIZ);
			return;
		}
		
		memset (resp, 0, sizeof (glibtop_response));

		memset (parameter, 0, sizeof (parameter));
		
		if (cmnd->data_size) {
#ifdef PARENT_DEBUG
			fprintf (stderr, "Client has %d bytes of data.\n",
				 (int) cmnd->data_size);
#endif

			do_read (s, parameter, cmnd->data_size);

		} else if (cmnd->size) {
			memcpy (parameter, cmnd->parameter, cmnd->size);
		}

		switch (cmnd->command) {
		case GLIBTOP_CMND_QUIT:
			do_output (s, resp, 0, 0, NULL);

#ifdef GLIBTOP_DAEMON_SLAVE
			fprintf (stderr, "Sending QUIT command (%d).\n",
				 server->socket);

			glibtop_call_l (server, GLIBTOP_CMND_QUIT,
					0, NULL, 0, NULL);
			
			fprintf (stderr, "Done sending QUIT command (%d).\n",
				 server->socket);
			
			close (server->socket);
#endif
			return;
		case GLIBTOP_CMND_SYSDEPS:
			memcpy (&resp->u.sysdeps, &server->sysdeps, 
				sizeof (glibtop_sysdeps));
			resp->u.sysdeps.features = GLIBTOP_SYSDEPS_ALL;
			do_output (s, resp, _offset_union (sysdeps), 0, NULL);
			break;
		case GLIBTOP_CMND_CPU:
			glibtop_get_cpu_l (server, &resp->u.data.cpu);
			do_output (s, resp, _offset_data (cpu), 0, NULL);
			break;
		case GLIBTOP_CMND_MEM:
			glibtop_get_mem_l (server, &resp->u.data.mem);
			do_output (s, resp, _offset_data (mem), 0, NULL);
			break;
		case GLIBTOP_CMND_SWAP:
			glibtop_get_swap_l (server, &resp->u.data.swap);
			do_output (s, resp, _offset_data (swap), 0, NULL);
			break;
		case GLIBTOP_CMND_UPTIME:
			glibtop_get_uptime_l (server, &resp->u.data.uptime);
			do_output (s, resp, _offset_data (uptime), 0, NULL);
			break;
		case GLIBTOP_CMND_LOADAVG:
			glibtop_get_loadavg_l (server, &resp->u.data.loadavg);
			do_output (s, resp, _offset_data (loadavg), 0, NULL);
			break;
		case GLIBTOP_CMND_SHM_LIMITS:
			glibtop_get_shm_limits_l
				(server, &resp->u.data.shm_limits);
			do_output (s, resp, _offset_data (shm_limits), 0, NULL);
			break;
		case GLIBTOP_CMND_MSG_LIMITS:
			glibtop_get_msg_limits_l
				(server, &resp->u.data.msg_limits);
			do_output (s, resp, _offset_data (msg_limits), 0, NULL);
			break;
		case GLIBTOP_CMND_SEM_LIMITS:
			glibtop_get_sem_limits_l
				(server, &resp->u.data.sem_limits);
			do_output (s, resp, _offset_data (sem_limits), 0, NULL);
			break;
		case GLIBTOP_CMND_PROCLIST:
			param_ptr = (int64_t *) parameter;
			ptr = glibtop_get_proclist_l (server,
						      &resp->u.data.proclist,
						      param_ptr [0],
						      param_ptr [1]);
			do_output (s, resp, _offset_data (proclist),
				   resp->u.data.proclist.total, ptr);
			glibtop_free_r (server, ptr);
			break;
		case GLIBTOP_CMND_PROC_MAP:
			memcpy (&pid, parameter, sizeof (pid_t));
			ptr = glibtop_get_proc_map_l (server,
						      &resp->u.data.proc_map,
						      pid);
			do_output (s, resp, _offset_data (proc_map),
				   resp->u.data.proc_map.total, ptr);
			glibtop_free_r (server, ptr);
			break;
		case GLIBTOP_CMND_PROC_ARGS:
			memcpy (&pid, parameter, sizeof (pid_t));
			ptr = glibtop_get_proc_args_l (server,
						       &resp->u.data.proc_args,
						       pid, 0);
			do_output (s, resp, _offset_data (proc_args),
				   ptr ? resp->u.data.proc_args.size+1 : 0, ptr);
			glibtop_free_r (server, ptr);
			break;
		case GLIBTOP_CMND_PROC_STATE:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_state_l
				(server, &resp->u.data.proc_state, pid);
			do_output (s, resp, _offset_data (proc_state), 0, NULL);
			break;
		case GLIBTOP_CMND_PROC_UID:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_uid_l
				(server, &resp->u.data.proc_uid, pid);
			do_output (s, resp, _offset_data (proc_uid), 0, NULL);
			break;
		case GLIBTOP_CMND_PROC_MEM:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_mem_l
				(server, &resp->u.data.proc_mem, pid);
			do_output (s, resp, _offset_data (proc_mem), 0, NULL);
			break;
		case GLIBTOP_CMND_PROC_TIME:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_time_l
				(server, &resp->u.data.proc_time, pid);
			do_output (s, resp, _offset_data (proc_time), 0, NULL);
			break;
		case GLIBTOP_CMND_PROC_SIGNAL:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_signal_l
				(server, &resp->u.data.proc_signal, pid);
			do_output (s, resp, _offset_data (proc_signal), 0, NULL);
			break;
		case GLIBTOP_CMND_PROC_KERNEL:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_kernel_l
				(server, &resp->u.data.proc_kernel, pid);
			do_output (s, resp, _offset_data (proc_kernel), 0, NULL);
			break;
		case GLIBTOP_CMND_PROC_SEGMENT:
			memcpy (&pid, parameter, sizeof (pid_t));
			glibtop_get_proc_segment_l
				(server, &resp->u.data.proc_segment, pid);
			do_output (s, resp, _offset_data (proc_segment), 0, NULL);
			break;
		case GLIBTOP_CMND_MOUNTLIST:
			memcpy (&all_fs, parameter, sizeof (all_fs));
			mount_list = glibtop_get_mountlist_l
				(server, &resp->u.data.mountlist, all_fs);
			do_output (s, resp, _offset_data (mountlist),
				   resp->u.data.mountlist.total, mount_list);
			glibtop_free_r (server, mount_list);
			break;
		case GLIBTOP_CMND_FSUSAGE:
			glibtop_get_fsusage_l
				(server, &resp->u.data.fsusage, parameter);
			do_output (s, resp, _offset_data (fsusage),
				   0, NULL);
			break;
		case GLIBTOP_CMND_PPP:
			memcpy (&device, parameter, sizeof (device));
			glibtop_get_ppp_l
				(server, &resp->u.data.ppp, device);
			do_output (s, resp, _offset_data (ppp), 0, NULL);
			break;
		case GLIBTOP_CMND_NETLOAD:
			glibtop_get_netload_l
				(server, &resp->u.data.netload, parameter);
			do_output (s, resp, _offset_data (netload),
				   0, NULL);
			break;
		default:
			glibtop_warn ("Parent received unknown command %u",
				      cmnd->command);
			break;
		}
	}
}
