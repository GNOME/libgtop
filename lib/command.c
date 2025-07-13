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

	g_assert(command >= GLIBTOP_CMND_QUIT && command < GLIBTOP_MAX_CMND);

	switch (command) {
#define CHECK_CMND(CMND) case (CMND): glibtop_debug("CALL: command %s sending %zu bytes", #CMND, send_size); break
	  CHECK_CMND(GLIBTOP_CMND_QUIT);
	  CHECK_CMND(GLIBTOP_CMND_SYSDEPS);
	  CHECK_CMND(GLIBTOP_CMND_CPU);
	  CHECK_CMND(GLIBTOP_CMND_DISK);
	  CHECK_CMND(GLIBTOP_CMND_MEM);
	  CHECK_CMND(GLIBTOP_CMND_SWAP);
	  CHECK_CMND(GLIBTOP_CMND_UPTIME);
	  CHECK_CMND(GLIBTOP_CMND_LOADAVG);
	  CHECK_CMND(GLIBTOP_CMND_SHM_LIMITS);
	  CHECK_CMND(GLIBTOP_CMND_MSG_LIMITS);
	  CHECK_CMND(GLIBTOP_CMND_SEM_LIMITS);
	  CHECK_CMND(GLIBTOP_CMND_PROCLIST);
	  CHECK_CMND(GLIBTOP_CMND_PROC_STATE);
	  CHECK_CMND(GLIBTOP_CMND_PROC_UID);
	  CHECK_CMND(GLIBTOP_CMND_PROC_MEM);
	  CHECK_CMND(GLIBTOP_CMND_PROC_TIME);
	  CHECK_CMND(GLIBTOP_CMND_PROC_SIGNAL);
	  CHECK_CMND(GLIBTOP_CMND_PROC_KERNEL);
	  CHECK_CMND(GLIBTOP_CMND_PROC_SEGMENT);
	  CHECK_CMND(GLIBTOP_CMND_PROC_ARGS);
	  CHECK_CMND(GLIBTOP_CMND_PROC_MAP);
	  CHECK_CMND(GLIBTOP_CMND_MOUNTLIST);
	  CHECK_CMND(GLIBTOP_CMND_FSUSAGE);
	  CHECK_CMND(GLIBTOP_CMND_NETLOAD);
	  CHECK_CMND(GLIBTOP_CMND_PPP);
	  CHECK_CMND(GLIBTOP_CMND_NETLIST);
	  CHECK_CMND(GLIBTOP_CMND_PROC_OPEN_FILES);
	  CHECK_CMND(GLIBTOP_CMND_PROC_WD);
	  CHECK_CMND(GLIBTOP_CMND_PROC_AFFINITY);
	  CHECK_CMND(GLIBTOP_CMND_PROC_IO);
	default:
	  glibtop_error_r(server, "CALL: command UNKNOWN(%d) sending %zu bytes", command, send_size); break;
	}
#undef CHECK_CMND

	cmnd.command = command;

	/* If send_size is less than _GLIBTOP_PARAM_SIZE (normally 16 Bytes), we
	 * send it together with command, so we only need one system call instead
	 * of two. */

	if (send_size <= _GLIBTOP_PARAM_SIZE) {
		memcpy (cmnd.parameter, send_buf, send_size);
		cmnd.size = send_size;
	} else {
		cmnd.data_size = send_size;
	}

	glibtop_write_l (server, sizeof (glibtop_command), &cmnd);

	glibtop_read_l (server, sizeof (glibtop_response), &response);

	glibtop_debug ("RESPONSE: offset=%" G_GUINT64_FORMAT " - data_size=%" G_GUINT64_FORMAT,
		 response.offset, response.data_size);

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
