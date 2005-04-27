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

#ifndef __GLIBTOP_PARAMETER_H__
#define __GLIBTOP_PARAMETER_H__

#include <glibtop.h>

G_BEGIN_DECLS

#define GLIBTOP_PARAM_METHOD		1
#define GLIBTOP_PARAM_FEATURES		2
#define GLIBTOP_PARAM_COMMAND		3
#define GLIBTOP_PARAM_HOST		4
#define GLIBTOP_PARAM_PORT		5
#define GLIBTOP_PARAM_ERROR_METHOD	6
#define GLIBTOP_PARAM_REQUIRED		7

#define glibtop_get_parameter(p1,p2,p3)	glibtop_get_parameter_l(glibtop_global_server,p1,p2,p3)
#define glibtop_set_parameter(p1,p2,p3) glibtop_set_parameter_l(glibtop_global_server,p1,p2,p3)

size_t
glibtop_get_parameter_l (glibtop *server, const unsigned parameter,
			 void *data_ptr, size_t data_size);

void
glibtop_set_parameter_l (glibtop *server, const unsigned parameter,
			 const void *data_ptr, size_t data_size);

G_END_DECLS

#endif
