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

#ifndef __GLIBTOP_SIGNAL_H__
#define __GLIBTOP_SIGNAL_H__

#include <glibtop.h>

#ifdef HAVE_SYS_SIGNAL_H
#include <sys/signal.h>
#endif

G_BEGIN_DECLS

typedef struct _glibtop_signame	glibtop_signame;

struct _glibtop_signame
{
	const int number;
	const char *name, *label;
};

extern const glibtop_signame glibtop_sys_siglist [];

G_END_DECLS

#endif
