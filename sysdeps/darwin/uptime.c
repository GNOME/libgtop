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
#include <glibtop/uptime.h>

#include <glibtop_suid.h>

#include <sys/param.h>
#include <sys/sysctl.h>
#include <time.h>

static const unsigned long _glibtop_sysdeps_uptime =
(1L << GLIBTOP_UPTIME_UPTIME);

/* Init function. */

void
_glibtop_init_uptime_p (glibtop *server)
{
	server->sysdeps.uptime = _glibtop_sysdeps_uptime;
}

/* Provides uptime and idle time. */

void
glibtop_get_uptime_p (glibtop *server, glibtop_uptime *buf)
{
	int mib [] = { CTL_KERN, KERN_BOOTTIME };
	struct timeval boottime;
	size_t size = sizeof (boottime);
	time_t now;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_UPTIME), 0);
	
	memset (buf, 0, sizeof (glibtop_uptime));

	/* copied from gkrellm port */

	if (sysctl (mib, 2, &boottime, &size, NULL, 0) == -1)
		return;
	time (&now);

	buf->uptime = now - boottime.tv_sec + 30;
	buf->flags = _glibtop_sysdeps_uptime;
}
