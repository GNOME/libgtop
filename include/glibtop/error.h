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

#ifndef __GLIBTOP_ERROR_H__
#define __GLIBTOP_ERROR_H__

#include <glibtop.h>

__BEGIN_DECLS

#define glibtop_error(p1, args...)	glibtop_error_r(glibtop_global_server , p1 , ## args)
#define glibtop_warn(p1, args...)	glibtop_warn_r(glibtop_global_server , p1 , ## args)

#define glibtop_error_io(p1, args...)	glibtop_error_io_r(glibtop_global_server , p1 , ## args)
#define glibtop_warn_io(p1, args...)	glibtop_warn_io_r(glibtop_global_server , p1 , ## args)

extern void glibtop_error_r __P((glibtop *, char *, ...));
extern void glibtop_warn_r __P((glibtop *, char *, ...));

extern void glibtop_error_io_r __P((glibtop *, char *, ...));
extern void glibtop_warn_io_r __P((glibtop *, char *, ...));

__END_DECLS

#endif
