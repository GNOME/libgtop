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

#include <glibtop/proclist.h>

const char *glibtop_names_proclist [GLIBTOP_MAX_PROCLIST] =
{
	"number", "total", "size"
};

const char *glibtop_types_proclist [GLIBTOP_MAX_PROCLIST] =
{
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG
};

const char *glibtop_labels_proclist [GLIBTOP_MAX_PROCLIST] =
{
	N_("Number of list elements"),
	N_("Total size of list"),
	N_("Size of a single list element")
};

const char *glibtop_descriptions_proclist [GLIBTOP_MAX_PROCLIST] =
{
	N_("Number of list elements"),
	N_("Total size of list"),
	N_("Size of a single list element")
};
