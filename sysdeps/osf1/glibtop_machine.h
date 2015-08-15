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

#ifndef __GLIBTOP_MACHINE_H__
#define __GLIBTOP_MACHINE_H__

#include <sys/table.h>

/* The following are defined in <sys/table.h>. */

#undef sys
#undef user
#undef idle

G_BEGIN_DECLS

int table (int id, int index, char *addr, int nel, u_int lel);

struct _glibtop_machine
{
	uid_t uid, euid;		/* Real and effective user id */
	gid_t gid, egid;		/* Real and effective group id */
	unsigned proctable_entries;	/* Max entries in the proctable */
};

G_END_DECLS

#endif
