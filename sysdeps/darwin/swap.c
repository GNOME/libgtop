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
#include <glibtop/swap.h>

#include <glibtop_suid.h>

#include <mach/mach_host.h>
#include <mach/host_info.h>
#include <mach/mach_init.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <dirent.h>

static const unsigned long _glibtop_sysdeps_swap =
(1L << GLIBTOP_SWAP_TOTAL) + (1L << GLIBTOP_SWAP_USED) +
(1L << GLIBTOP_SWAP_FREE) + (1L << GLIBTOP_SWAP_PAGEIN) +
(1L << GLIBTOP_SWAP_PAGEOUT);

/* Init function. */

void
_glibtop_init_swap_p (glibtop *server)
{
	server->sysdeps.swap = _glibtop_sysdeps_swap;
}

/* Provides information about swap usage. */

void
glibtop_get_swap_p (glibtop *server, glibtop_swap *buf)
{
	vm_statistics_data_t vm_info;
	mach_msg_type_number_t info_count;
	DIR *dirp;
	struct dirent *dp;
	struct statfs sfs;
	u_int64_t total, used;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_SWAP), 0);

	memset (buf, 0, sizeof (glibtop_swap));

	total = used = 0;

	dirp = opendir ("/private/var/vm");
	if (!dirp) {
		glibtop_warn_io_r (server, "opendir (swap)");
		return;
	}
	while ((dp = readdir (dirp)) != NULL) {
		struct stat sb;
		char fname [MAXNAMLEN];

		if (strncmp (dp->d_name, "swapfile", 8))
			continue;

		strcpy (fname, "/private/var/vm/");
		strcat (fname, dp->d_name);
		if (stat (fname, &sb) < 0)
			continue;

		used += sb.st_size;
	}
	closedir (dirp);

	if (statfs ("/private/var/vm", &sfs) < 0) {
		glibtop_warn_io_r (server, "statfs (swap)");
		return;
	}
	total = (u_int64_t)sfs.f_bfree * sfs.f_bsize + used;

	info_count = HOST_VM_INFO_COUNT;
	if (host_statistics (mach_host_self (), HOST_VM_INFO,
			     (host_info_t) &vm_info, &info_count)) {
		glibtop_warn_io_r (server, "host_statistics (swap)");
		return;
	}

	buf->total    = total;
	buf->used     = used;
	buf->free     = total - used;
	buf->pagein   = vm_info.pageins;
	buf->pageout  = vm_info.pageouts;
	buf->flags    = _glibtop_sysdeps_swap;
}

