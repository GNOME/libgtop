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
#include <nlist.h>

#include <procinfo.h>
#include <sys/proc.h>
#include <sys/vminfo.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include "glibtop_suid.h"
#include "utils.h"

#ifndef HAVE_VMGETINFO
#include <dlfcn.h>
typedef int (*vmgetinfo_proto)(void *out, int command, int arg);
#endif

off_t
_glibtop_get_kmem_offset(glibtop* server, char* kname)
{
	int result;
	struct nlist kernelnames[] =
		{ {NULL, 0, 0, 0, 0, 0},
		  {NULL, 0, 0, 0, 0, 0},
		};

	kernelnames[0]._n._n_name = kname;

	glibtop_suid_enter(server);

	result = knlist(kernelnames, 1, sizeof(struct nlist));

	glibtop_suid_leave(server);

	if (result == -1)
	{
		return -1;
	}

	return kernelnames[0].n_value;
}

int
_glibtop_get_kmem_info(glibtop* server, off_t offset, void* buf, size_t len)
{
	int result;

	glibtop_suid_enter(server);

	lseek(server->machine->kmem_fd, offset, SEEK_SET);

	result = read(server->machine->kmem_fd, buf, len);

        glibtop_suid_leave(server);

	return result;
}

struct procsinfo*
_glibtop_get_procinfo (glibtop *server, pid_t pid)
{
	int result;
	pid_t current;
	static int first_time = 1;

	/* test if procsinfo already found */

	if ((server->machine->last_pinfo.pi_pid == pid) && (!first_time))
	{
		return &server->machine->last_pinfo;
	}

	/* seek procsinfo if given pid */

	first_time = 0;
	current = 0;
	while ((result =  getprocs( &server->machine->last_pinfo
				  , sizeof(struct procsinfo)
				  , NULL, 0, &current, 1)) == 1)
	{

		if (pid == server->machine->last_pinfo.pi_pid)
		{
			return &server->machine->last_pinfo;
		}
	}
	return NULL;
}

#ifndef HAVE_VMGETINFO
int
_glibtop_vmgetinfo (void *out, int command, int arg)
{
	void* handle;

	static vmgetinfo_proto kern_vmgetinfo = NULL;

	if (kern_vmgetinfo == NULL)
	{
		handle = dlopen("/unix", RTLD_NOW | RTLD_GLOBAL);
		if (handle == NULL)
		{
			return -1;
		}

		kern_vmgetinfo = dlsym( handle, "vmgetinfo");

		dlclose(handle);

		if (kern_vmgetinfo == NULL)
		{
			return -1;
		}
	}

	return kern_vmgetinfo(out, command, arg);
}
#endif
