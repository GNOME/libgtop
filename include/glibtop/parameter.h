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

#ifndef __GLIBTOP_PARAMETER_H__
#define __GLIBTOP_PARAMETER_H__

#include <glibtop.h>
#include <glibtop/open.h>

__BEGIN_DECLS

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

__END_DECLS

#endif
