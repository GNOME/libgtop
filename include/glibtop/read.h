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

#ifndef __GLIBTOP_READ_H__
#define __GLIBTOP_READ_H__

#include <glibtop.h>

G_BEGIN_DECLS

#define glibtop_read(p1, p2)	glibtop_read(glibtop_global_server, p1, p2)

void glibtop_read_l (glibtop *server, size_t size, void *buf);
void glibtop_read_s (glibtop *server, size_t size, void *buf);

G_END_DECLS

#endif
