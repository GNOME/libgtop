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
#include "daemon.h"
#include <glibtop/error.h>

void
handle_slave_connection (int input, int output)
{
	glibtop *server G_GNUC_UNUSED = glibtop_global_server;
	const void *ptr G_GNUC_UNUSED;

	unsigned short max_len G_GNUC_UNUSED;
	pid_t pid G_GNUC_UNUSED;
	gint64 proc_which G_GNUC_UNUSED;
	gint64 proc_arg G_GNUC_UNUSED;

	glibtop_response _resp, *resp = &_resp;
	glibtop_command _cmnd, *cmnd = &_cmnd;
	char parameter [BUFSIZ];

	glibtop_send_version (glibtop_global_server, output);

	while (do_read (input, cmnd, sizeof (glibtop_command))) {
		glibtop_debug ("Slave %" G_GINT64_FORMAT " received command "
			 "%" G_GUINT64_FORMAT " from client.",
			 (gint64)getpid (), cmnd->command);

		memset (resp, 0, sizeof (glibtop_response));

		memset (parameter, 0, sizeof (parameter));

		if (cmnd->data_size) {
			if (cmnd->data_size >= BUFSIZ)
				glibtop_error ("Client sent %" G_GUINT64_FORMAT " bytes, "
					       "but buffer is %lu",
					       cmnd->data_size, (unsigned long)BUFSIZ);

			glibtop_debug ("Client has %" G_GUINT64_FORMAT " bytes of data.",
				 cmnd->data_size);

			do_read (input, parameter, cmnd->data_size);
		} else if (cmnd->size) {
			if (cmnd->size >= BUFSIZ)
				glibtop_error ("Client sent %" G_GUINT64_FORMAT " bytes, "
					       "but buffer is %lu",
					       cmnd->size, (unsigned long)BUFSIZ);

			memcpy (parameter, cmnd->parameter, cmnd->size);
		}

		switch (cmnd->command) {
		case GLIBTOP_CMND_QUIT:
			do_output (output, resp, 0, 0, NULL);
			return;
#if GLIBTOP_SUID_PROCLIST
		case GLIBTOP_CMND_PROCLIST:
			memcpy(&proc_which, parameter, sizeof proc_which);
			memcpy(&proc_arg, parameter + sizeof proc_which, sizeof proc_arg);
			ptr = glibtop_get_proclist_p
				(server, &resp->u.data.proclist,
				 proc_which, proc_arg);
			do_output (output, resp, _offset_data (proclist),
				   resp->u.data.proclist.total, ptr);
			g_free (ptr);
			break;
#endif
#if GLIBTOP_SUID_PROC_ARGS
		case GLIBTOP_CMND_PROC_ARGS:
			memcpy (&pid, parameter, sizeof (pid_t));
			memcpy (&max_len, parameter + sizeof (pid_t),
				sizeof (max_len));
			ptr = glibtop_get_proc_args_p (server,
						       &resp->u.data.proc_args,
						       pid, max_len);
			do_output (output, resp, _offset_data (proc_args),
				   ptr ? resp->u.data.proc_args.size+1 : 0,
				   ptr);
			g_free (ptr);
			break;
#endif
#if GLIBTOP_SUID_PROC_MAP
		case GLIBTOP_CMND_PROC_MAP:
			memcpy (&pid, parameter, sizeof (pid_t));
			ptr = glibtop_get_proc_map_p (server,
						      &resp->u.data.proc_map,
						      pid);
			do_output (output, resp, _offset_data (proc_map),
				   resp->u.data.proc_map.total, ptr);
			g_free (ptr);
			break;
#endif
		default:
			handle_slave_command (cmnd, resp, parameter);
			do_output (output, resp, resp->offset, 0, NULL);
			break;
		}
	}
}

void
handle_slave_command (glibtop_command *cmnd, glibtop_response *resp,
		      const void *parameter)
{
	glibtop *server = glibtop_global_server;
	unsigned device G_GNUC_UNUSED;
	pid_t pid G_GNUC_UNUSED;

	switch (cmnd->command) {
	case GLIBTOP_CMND_SYSDEPS:
		memcpy (&resp->u.sysdeps, &server->sysdeps,
			sizeof (glibtop_sysdeps));
		resp->u.sysdeps.features = glibtop_server_features;
		resp->u.sysdeps.flags = glibtop_server_features |
		  (1L << GLIBTOP_SYSDEPS_FEATURES);
		resp->offset = _offset_union (sysdeps);
		break;
#if GLIBTOP_SUID_CPU
	case GLIBTOP_CMND_CPU:
		glibtop_get_cpu_p (server, &resp->u.data.cpu);
		resp->offset = _offset_data (cpu);
		break;
#endif
#if GLIBTOP_SUID_DISK
	case GLIBTOP_CMND_DISK:
		glibtop_get_disk_p (server, &resp->u.data.disk);
		resp->offset = _offset_data (disk);
		break;
#endif
#if GLIBTOP_SUID_MEM
	case GLIBTOP_CMND_MEM:
		glibtop_get_mem_p (server, &resp->u.data.mem);
		resp->offset = _offset_data (mem);
		break;
#endif
#if GLIBTOP_SUID_SWAP
	case GLIBTOP_CMND_SWAP:
		glibtop_get_swap_p (server, &resp->u.data.swap);
		resp->offset = _offset_data (swap);
		break;
#endif
#if GLIBTOP_SUID_UPTIME
	case GLIBTOP_CMND_UPTIME:
		glibtop_get_uptime_p (server, &resp->u.data.uptime);
		resp->offset = _offset_data (uptime);
		break;
#endif
#if GLIBTOP_SUID_LOADAVG
	case GLIBTOP_CMND_LOADAVG:
		glibtop_get_loadavg_p (server, &resp->u.data.loadavg);
		resp->offset = _offset_data (loadavg);
		break;
#endif
#if GLIBTOP_SUID_SHM_LIMITS
	case GLIBTOP_CMND_SHM_LIMITS:
		glibtop_get_shm_limits_p (server, &resp->u.data.shm_limits);
		resp->offset = _offset_data (shm_limits);
		break;
#endif
#if GLIBTOP_SUID_MSG_LIMITS
	case GLIBTOP_CMND_MSG_LIMITS:
		glibtop_get_msg_limits_p (server, &resp->u.data.msg_limits);
		resp->offset = _offset_data (msg_limits);
		break;
#endif
#if GLIBTOP_SUID_SEM_LIMITS
	case GLIBTOP_CMND_SEM_LIMITS:
		glibtop_get_sem_limits_p (server, &resp->u.data.sem_limits);
		resp->offset = _offset_data (sem_limits);
		break;
#endif
#if GLIBTOP_SUID_PROC_STATE
	case GLIBTOP_CMND_PROC_STATE:
		memcpy (&pid, parameter, sizeof (pid_t));
		glibtop_get_proc_state_p
			(server, &resp->u.data.proc_state, pid);
		resp->offset = _offset_data (proc_state);
		break;
#endif
#if GLIBTOP_SUID_PROC_UID
	case GLIBTOP_CMND_PROC_UID:
		memcpy (&pid, parameter, sizeof (pid_t));
		glibtop_get_proc_uid_p
			(server, &resp->u.data.proc_uid, pid);
		resp->offset = _offset_data (proc_uid);
		break;
#endif
#if GLIBTOP_SUID_PROC_MEM
	case GLIBTOP_CMND_PROC_MEM:
		memcpy (&pid, parameter, sizeof (pid_t));
		glibtop_get_proc_mem_p
			(server, &resp->u.data.proc_mem, pid);
		resp->offset = _offset_data (proc_mem);
		break;
#endif
#if GLIBTOP_SUID_PROC_TIME
	case GLIBTOP_CMND_PROC_TIME:
		memcpy (&pid, parameter, sizeof (pid_t));
		glibtop_get_proc_time_p
			(server, &resp->u.data.proc_time, pid);
		resp->offset = _offset_data (proc_time);
		break;
#endif
#if GLIBTOP_SUID_PROC_IO
	case GLIBTOP_CMND_PROC_IO:
		memcpy (&pid, parameter, sizeof (pid_t));
		glibtop_get_proc_io_p
			(server, &resp->u.data.proc_io, pid);
		resp->offset = _offset_data (proc_time);
		break;
#endif
#if GLIBTOP_SUID_PROC_SIGNAL
	case GLIBTOP_CMND_PROC_SIGNAL:
		memcpy (&pid, parameter, sizeof (pid_t));
		glibtop_get_proc_signal_p
			(server, &resp->u.data.proc_signal, pid);
		resp->offset = _offset_data (proc_signal);
		break;
#endif
#if GLIBTOP_SUID_PROC_KERNEL
	case GLIBTOP_CMND_PROC_KERNEL:
		memcpy (&pid, parameter, sizeof (pid_t));
		glibtop_get_proc_kernel_p
			(server, &resp->u.data.proc_kernel, pid);
		resp->offset = _offset_data (proc_kernel);
		break;
#endif
#if GLIBTOP_SUID_PROC_SEGMENT
	case GLIBTOP_CMND_PROC_SEGMENT:
		memcpy (&pid, parameter, sizeof (pid_t));
		glibtop_get_proc_segment_p
			(server, &resp->u.data.proc_segment, pid);
		resp->offset = _offset_data (proc_segment);
		break;
#endif
#if GLIBTOP_SUID_NETLOAD
	case GLIBTOP_CMND_NETLOAD:
		glibtop_get_netload_p (server, &resp->u.data.netload, parameter);
		resp->offset = _offset_data (netload);
		break;
#endif
#if GLIBTOP_SUID_PPP
	case GLIBTOP_CMND_PPP:
		memcpy (&device, parameter, sizeof (unsigned short));
		glibtop_get_ppp_p (server, &resp->u.data.ppp, device);
		resp->offset = _offset_data (ppp);
		break;
#endif
	default:
		glibtop_error ("Child received unknown command %" G_GUINT64_FORMAT,
			       cmnd->command);
		break;
	}
}
