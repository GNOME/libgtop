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

#ifndef __GLIBTOP_XMALLOC_H__
#define __GLIBTOP_XMALLOC_H__

#include <glibtop.h>
#include <glibtop/error.h>

__BEGIN_DECLS

#define glibtop_malloc(p1)	glibtop_malloc__r(glibtop_global_server, p1)
#define glibtop_calloc(p1, p2)	glibtop_calloc__r(glibtop_global_server, p1, p2)
#define glibtop_realloc(p1, p2)	glibtop_realloc__r(glibtop_global_server, p1, p2)
#define glibtop_free(p1)	glibtop_free__r(glibtop_global_server, p1)

extern void *glibtop_malloc__r	__P((glibtop *, size_t));
extern void *glibtop_calloc__r	__P((glibtop *, size_t, size_t));
extern void *glibtop_realloc__r	__P((glibtop *, void *, size_t));
extern void glibtop_free__r	__P((glibtop *, void *));

__END_DECLS

#endif
