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
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __GLIBTOP_INODEDB_H__
#define __GLIBTOP_INODEDB_H__

#include <glibtop.h>
#include <gdbm.h>

#define GLIBTOP_INODEDB_SYSTEM		1
#define GLIBTOP_INODEDB_USER		2
#define GLIBTOP_INODEDB_CACHE		4

#define GLIBTOP_INODEDB_ALL		7

BEGIN_LIBGTOP_DECLS

typedef struct _glibtop_inodedb		glibtop_inodedb;

typedef struct _glibtop_inodedb_key	glibtop_inodedb_key;

struct _glibtop_inodedb_key
{
	u_int64_t device, inode;
};

struct _glibtop_inodedb
{
	GDBM_FILE system_dbf, user_dbf;
};

#define glibtop_inodedb_open(p1,p2)	glibtop_inodedb_open_s(glibtop_global_server, p1, p2)
#define glibtop_inodedb_lookup(p1,p2,p3) glibtop_inodedb_lookup_s(glibtop_global_server, p1, p2, p3)
#define glibtop_inodedb_close(p1)	glibtop_inodedb_close_s(glibtop_global_server)

glibtop_inodedb *
glibtop_inodedb_open_s (glibtop *server, unsigned databases, unsigned long cache_size);

const char *
glibtop_inodedb_lookup_s (glibtop *server,
			  glibtop_inodedb *inodedb,
			  u_int64_t device, u_int64_t inode);

void
glibtop_inodedb_close_s (glibtop *server, glibtop_inodedb *inodedb);

END_LIBGTOP_DECLS

#endif
