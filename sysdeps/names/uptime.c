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

#include <glibtop/uptime.h>

const char *glibtop_names_uptime [GLIBTOP_MAX_UPTIME] =
{
	"uptime", "idletime"
};

const unsigned glibtop_types_uptime [GLIBTOP_MAX_UPTIME] =
{
	GLIBTOP_TYPE_DOUBLE, GLIBTOP_TYPE_DOUBLE
};

const char *glibtop_labels_uptime [GLIBTOP_MAX_UPTIME] =
{
	N_("Uptime"),
	N_("Idletime")
};

const char *glibtop_descriptions_uptime [GLIBTOP_MAX_UPTIME] =
{
	N_("Time in seconds since system boot"),
	N_("Time in seconds the system spent in the idle task since system boot")
};
