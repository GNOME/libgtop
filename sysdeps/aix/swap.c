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
#include <sys/vminfo.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/swap.h>

static const unsigned long _glibtop_sysdeps_swap =
(1 << GLIBTOP_SWAP_TOTAL) + (1 << GLIBTOP_SWAP_USED) +
(1 << GLIBTOP_SWAP_FREE) + (1 << GLIBTOP_SWAP_PAGEIN) +
(1 << GLIBTOP_SWAP_PAGEOUT);

#define FILENAME	"/etc/swapspaces"

/* Init function. */

void
_glibtop_init_swap_s (glibtop *server)
{
	server->sysdeps.swap = _glibtop_sysdeps_swap;
}

/* Provides information about swap usage. */

void
glibtop_get_swap_s (glibtop *server, glibtop_swap *buf)
{
	struct vminfo vminfo;
	struct pginfo pginfo;
	int pagesize;
	int result;
	FILE* fd;
	char device_name[128];
	int c;
	int comment = 0;
	int pos = 0;

	glibtop_init_s (&server, (1L << GLIBTOP_SYSDEPS_SWAP), 0);

	memset (buf, 0, sizeof (glibtop_swap));

	pagesize = sysconf(_SC_PAGESIZE);

	fd = fopen(FILENAME, "r");
	if (fd == NULL)
	{
		glibtop_error_io_r (server, "open (%s)", FILENAME);
	}

	buf->total = 0;
	buf->free = 0;
	while((c = getc(fd)) != EOF)
	{
		if (comment)
		{
			/* skip comments */
			if (c == '\n') comment = 0;
			continue;
		}
		switch(c)
		{
			case '*':
				comment = 1;
				break;
			case '\n':
				device_name[pos] = 0;
				pos = 0;

				if (strncmp("dev=", device_name, 4) == 0)
				{
					result = swapqry(device_name+4, &pginfo);
					if (result == -1)
					{
						glibtop_error_io_r (server, "Cannot read pginfo");
						break;
					}

					buf->total += pginfo.size * pagesize;
					buf->free += pginfo.free * pagesize;;
				}
				break;
			case ' ':
			case '\t':
				break;
			default:
				device_name[pos++] = c;
				break;
		}
	}

	buf->used = buf->total - buf->free;
	fclose(fd);
#ifdef HAVE_VMGETINFO
	result = vmgetinfo((void*)&vminfo, VMINFO, sizeof(vminfo));
#else
	result = _glibtop_vmgetinfo((void*)&vminfo, VMINFO, sizeof(vminfo));
#endif
	if (result == -1)
	{
		glibtop_error_io_r (server, "Cannot read vminfo");
		return;
	}

	buf->pagein = vminfo.pageins;
	buf->pageout = vminfo.pageouts;

	buf->flags = _glibtop_sysdeps_swap;
}
