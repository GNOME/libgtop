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

#define _GLIBTOP_INIT_STATE_INIT	0x10000
#define _GLIBTOP_INIT_STATE_OPEN	0x20000
#define _GLIBTOP_INIT_STATE_SERVER	0x40000

#define GLIBTOP_INIT_NO_OPEN		1
#define GLIBTOP_INIT_NO_INIT		2

#define GLIBTOP_FEATURES_NO_SERVER	4
#define GLIBTOP_FEATURES_EXCEPT		8

#define GLIBTOP_METHOD_DIRECT		1
#define GLIBTOP_METHOD_PIPE		2
#define GLIBTOP_METHOD_INET		3
#define GLIBTOP_METHOD_UNIX		4

#define GLIBTOP_ERROR_METHOD_IGNORE	0
#define GLIBTOP_ERROR_METHOD_WARN_ONCE	1
#define GLIBTOP_ERROR_METHOD_WARN	2
#define GLIBTOP_ERROR_METHOD_ABORT	3

#define GLIBTOP_ERROR_METHOD_DEFAULT	GLIBTOP_ERROR_METHOD_WARN_ONCE

extern void glibtop_open_l __P((glibtop *, const char *, const unsigned long, const unsigned));

extern void glibtop_open_p __P((glibtop *, const char *, const unsigned long, const unsigned));
extern void glibtop_open_s __P((glibtop *, const char *, const unsigned long, const unsigned));

__END_DECLS

#endif
