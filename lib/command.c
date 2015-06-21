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
#include <glibtop/error.h>
#include <glibtop/read.h>
#include <glibtop/write.h>
#include <glibtop/read_data.h>

#include <glibtop/command.h>

void *
glibtop_call_l (glibtop *server, unsigned command, size_t send_size,
		const void *send_buf, size_t recv_size, void *recv_buf)
{
	glibtop_command cmnd = {0};
	glibtop_response response = {0};

	glibtop_init_r (&server, 0, 0);

	cmnd.command = command;

	/* If send_size is less than _GLIBTOP_PARAM_SIZE (normally 16 Bytes), we
	 * send it together with command, so we only need one system call instead
	 * of two. */

#ifdef LIBGTOP_ENABLE_DEBUG
	g_assert(command >= GLIBTOP_CMND_QUIT && command < GLIBTOP_MAX_CMND);
	switch (command) {
#define DEBUG_CALL(CMD) case (CMD): glibtop_warn_r(server, "CALL: command %s sending %lu bytes", #CMD, (unsigned long)send_size); break
	  DEBUG_CALL(GLIBTOP_CMND_QUIT);
	  DEBUG_CALL(GLIBTOP_CMND_SYSDEPS);
	  DEBUG_CALL(GLIBTOP_CMND_CPU);
	  DEBUG_CALL(GLIBTOP_CMND_MEM);
	  DEBUG_CALL(GLIBTOP_CMND_SWAP);
	  DEBUG_CALL(GLIBTOP_CMND_UPTIME);
	  DEBUG_CALL(GLIBTOP_CMND_LOADAVG);
	  DEBUG_CALL(GLIBTOP_CMND_SHM_LIMITS);
	  DEBUG_CALL(GLIBTOP_CMND_MSG_LIMITS);
	  DEBUG_CALL(GLIBTOP_CMND_SEM_LIMITS);
	  DEBUG_CALL(GLIBTOP_CMND_PROCLIST);
	  DEBUG_CALL(GLIBTOP_CMND_PROC_STATE);
	  DEBUG_CALL(GLIBTOP_CMND_PROC_UID);
	  DEBUG_CALL(GLIBTOP_CMND_PROC_MEM);
	  DEBUG_CALL(GLIBTOP_CMND_PROC_TIME);
	  DEBUG_CALL(GLIBTOP_CMND_PROC_SIGNAL);
	  DEBUG_CALL(GLIBTOP_CMND_PROC_KERNEL);
	  DEBUG_CALL(GLIBTOP_CMND_PROC_SEGMENT);
	  DEBUG_CALL(GLIBTOP_CMND_PROC_ARGS);
	  DEBUG_CALL(GLIBTOP_CMND_PROC_MAP);
	  DEBUG_CALL(GLIBTOP_CMND_MOUNTLIST);
	  DEBUG_CALL(GLIBTOP_CMND_FSUSAGE);
	  DEBUG_CALL(GLIBTOP_CMND_NETLOAD);
	  DEBUG_CALL(GLIBTOP_CMND_PPP);
	  DEBUG_CALL(GLIBTOP_CMND_NETLIST);
	  DEBUG_CALL(GLIBTOP_CMND_PROC_OPEN_FILES);
	  DEBUG_CALL(GLIBTOP_CMND_PROC_WD);
	  DEBUG_CALL(GLIBTOP_CMND_PROC_AFFINITY);
	default:
	  glibtop_error_r(server, "CALL: command UNKNOWN(%d) sending %lu bytes", command, (unsigned long)send_size); break;
	}
#undef DEBUG_CALL
#endif

	if (send_size <= _GLIBTOP_PARAM_SIZE) {
		memcpy (cmnd.parameter, send_buf, send_size);
		cmnd.size = send_size;
	} else {
		cmnd.data_size = send_size;
	}

	glibtop_write_l (server, sizeof (glibtop_command), &cmnd);

	glibtop_read_l (server, sizeof (glibtop_response), &response);

#ifdef LIBGTOP_ENABLE_DEBUG
	fprintf (stderr, "RESPONSE: offset=%lu - data_size=%lu\n",
		 response.offset, response.data_size);
#endif

	if (recv_buf)
		memcpy (recv_buf, ((char *) &response) + response.offset,
			recv_size);

	if (response.data_size) {
		void *ptr = g_malloc (response.data_size);

		glibtop_read_l (server, response.data_size, ptr);

		return ptr;
	}

	return NULL;
}
