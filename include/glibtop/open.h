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

#ifndef __GLIBTOP_OPEN_H__
#define __GLIBTOP_OPEN_H__

#include <glibtop.h>
#include <glibtop/read.h>
#include <glibtop/error.h>

__BEGIN_DECLS

#define GLIBTOP_OPEN_NO_OVERRIDE	1

extern void glibtop_open__l __P((glibtop *, const char *, const unsigned long, const unsigned));

extern void glibtop_open__r __P((glibtop *, const char *, const unsigned long, const unsigned));

__END_DECLS

#endif
