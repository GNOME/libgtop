/*
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, March 1999.

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

#ifndef __GLIBTOP_MACHINE_H__
#define __GLIBTOP_MACHINE_H__

#include <procinfo.h>

G_BEGIN_DECLS

struct _glibtop_machine
{
	uid_t uid, euid;
	gid_t gid, egid;

	int kmem_fd;

	off_t cpuinfo_offset;
	off_t sysinfo_offset;
	off_t ifnet_offset;
	off_t loadavg_offset;
	off_t shminfo_offset;
	off_t seminfo_offset;
	off_t msginfo_offset;

	struct cpuinfo *cpuinfo;
	struct procsinfo last_pinfo;
};

G_END_DECLS

#endif /* __GLIBTOP_MACHINE_H__ */
