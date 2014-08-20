/*
   This file is part of LibGTop 2.0.

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
#include <unistd.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procargs.h>

#include <glibtop_suid.h>

#include <sys/param.h>
#include <sys/sysctl.h>
#include <limits.h>

static const unsigned long _glibtop_sysdeps_proc_args =
(1L << GLIBTOP_PROC_ARGS_SIZE);

/* Init function. */

void
_glibtop_init_proc_args_p (glibtop *server)
{
	server->sysdeps.proc_args = _glibtop_sysdeps_proc_args;
}

/* Provides detailed information about a process. */

char *
glibtop_get_proc_args_p (glibtop *server, glibtop_proc_args *buf,
			 pid_t pid, unsigned max_len)
{
	char argbuf[4096];
	char *start, *end, *str;
	size_t bufsize = 4096;
	int mib [3];
	unsigned length;
	char *args;
	int *ip;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_ARGS), 0);

	memset (buf, 0, sizeof (glibtop_proc_args));

	mib [0] = CTL_KERN;
	mib [1] = KERN_PROCARGS;
	mib [2] = pid;
	
	/* !!! THE FOLLOWING CODE RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

	glibtop_suid_enter (server);
	if (sysctl (mib, 3, argbuf, &bufsize, NULL, 0) < 0) {
		glibtop_suid_leave (server);
		return NULL;
	}
	glibtop_suid_leave (server);

	/* !!! END OF SUID ROOT PART !!! */

    	end = &argbuf[bufsize];

	ip = (int *)end - 2;
	while (*--ip) {
		if (ip == (int *)argbuf) {
			return NULL;
		}
	}

	start = (char *)(ip + 1);
	while (*--ip) {
		if (ip == (int *)argbuf) {
			return NULL;
		}
	}
	ip++;

        if ((ip[0] & 0xbfff0000) == 0xbfff0000) {
		ip += 2;
		start = (char*)(ip + (strlen ((char*)ip) + 3) / 4);
		while (!*start)
			start++;
	}

	for (str = start + strlen (start); str < end - 1; str++) {
		if (*str == '=') {
			while (*str != '\0' && str > start)
				str--;
			break;
		}
	}
	end = str;

	if (start[0] == '-' || start[0] == '?' || start[0] <= ' ') {
		return NULL;
	}

	length = end - start;
	if (max_len != 0 && length > max_len)
		length = max_len;
	args = g_malloc(length + 1);
	if (args == NULL) {
		glibtop_warn_io_r (server, "malloc(procargs)");
		return NULL;
	}

	memcpy (args, start, length);
	args [length] = '\0';

	buf->size = length;
	buf->flags = _glibtop_sysdeps_proc_args;

	return args;
}
