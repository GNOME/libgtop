/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef __GLIBTOP_MACHINE_H__
#define __GLIBTOP_MACHINE_H__

#include <sys/table.h>

/* The following are defined in <sys/table.h>. */

#undef sys
#undef user
#undef idle

BEGIN_LIBGTOP_DECLS

int table (int id, int index, char *addr, int nel, u_int lel);

typedef struct _glibtop_machine		glibtop_machine;

struct _glibtop_machine
{
	uid_t uid, euid;		/* Real and effective user id */
	gid_t gid, egid;		/* Real and effective group id */
	unsigned proctable_entries;	/* Max entries in the proctable */
};

END_LIBGTOP_DECLS

#endif
