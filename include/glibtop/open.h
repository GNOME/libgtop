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

#ifndef __GLIBTOP_OPEN_H__
#define __GLIBTOP_OPEN_H__

#include <glibtop.h>

G_BEGIN_DECLS

#define _GLIBTOP_INIT_STATE_INIT	0x10000
#define _GLIBTOP_INIT_STATE_OPEN	0x20000
#define _GLIBTOP_INIT_STATE_SERVER	0x40000
#define _GLIBTOP_INIT_STATE_SYSDEPS	0x80000

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

void
glibtop_open_l (glibtop *server, const char *program_name,
		const unsigned long features, const unsigned flags);

void
glibtop_init_p (glibtop *server, const unsigned long features,
		const unsigned flags);

void
glibtop_open_p (glibtop *server, const char *program_name,
		const unsigned long features, const unsigned flags);

void
glibtop_open_s (glibtop *server, const char *program_name,
		const unsigned long features, const unsigned flags);


G_END_DECLS

#endif
