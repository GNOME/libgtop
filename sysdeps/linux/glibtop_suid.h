/* Copyright (C) 2018 Benoît Dejean

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

#ifndef __GLIBTOP_SUID_H__
#define __GLIBTOP_SUID_H__

G_BEGIN_DECLS

#include <glibtop_machine.h>

static inline void glibtop_suid_enter (glibtop *server) {
	glibtop_debug ("uid=%d euid=%d gid=%d egid=%d", getuid(), geteuid(), getgid(), getegid());
	setreuid (server->machine->uid, server->machine->euid);
	glibtop_debug ("uid=%d euid=%d gid=%d egid=%d", getuid(), geteuid(), getgid(), getegid());
};

static inline void glibtop_suid_leave (glibtop *server) {
	glibtop_debug ("uid=%d euid=%d gid=%d egid=%d", getuid(), geteuid(), getgid(), getegid());
	if (setreuid (server->machine->euid, server->machine->uid))
		_exit (1);
	glibtop_debug ("uid=%d euid=%d gid=%d egid=%d", getuid(), geteuid(), getgid(), getegid());
};

void
glibtop_init_p (glibtop *server, const unsigned long features,
		const unsigned flags);
void
glibtop_open_p (glibtop *server, const char *program_name,
		const unsigned long features,
		const unsigned flags);

G_END_DECLS

#endif
