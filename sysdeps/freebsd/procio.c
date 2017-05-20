/* Copyright (C) 2017 Robert Roth
   This file is part of LibGTop.

   Contributed by Robert Roth <robert.roth.off@gmail.com>, February 2017.

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
#include <glibtop.h>
#include <glibtop/procio.h>
#include <glibtop/error.h>
#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_proc_io =
  (1UL << GLIBTOP_PROC_IO_DISK_RBYTES) + (1UL << GLIBTOP_PROC_IO_DISK_WBYTES);

/* Init function. */

void
_glibtop_init_proc_io_p (glibtop *server)
{
	server->sysdeps.proc_io = _glibtop_sysdeps_proc_io;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_io_p (glibtop *server, glibtop_proc_io *buf,
			 pid_t pid)
{
	memset (buf, 0, sizeof (glibtop_proc_io));

	struct kinfo_proc *pinfo;
	int count;

	glibtop_suid_enter (server);

	/* Get the process information */
	pinfo = kvm_getprocs (server->machine->kd, KERN_PROC_PID, pid, &count);
	if ((pinfo == NULL) || (count != 1)) {
		glibtop_warn_io_r (server, "kvm_getprocs (%d)", pid);
		glibtop_suid_leave (server);
		return;
	}

	glibtop_suid_leave (server);

	/* man getrusage

	   long ru_inblock;         == block input operations
	   long ru_oublock;         == block output operations

	   ru_inblock   the number of times the file system had to perform input.
	   ru_oublock   the number of times the file system had to perform output.

	   And then it says 'account only for real IO'.

	   But if I write 1MB in a process, I can see ru_oublock increased
	   1024. So it's neither a number of operations or times.

	   FIXME: seems the blocksize is 1024 but ...
	*/
	buf->disk_rbytes = pinfo->ki_rusage.ru_inblock << 10;
	buf->disk_wbytes = pinfo->ki_rusage.ru_oublock << 10;
	buf->flags = _glibtop_sysdeps_proc_io;
}
