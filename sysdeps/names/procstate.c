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

#include <glibtop.h>
#include <glibtop/procstate.h>

const char *glibtop_names_proc_state [GLIBTOP_MAX_PROC_STATE] =
{
	"cmd", "state", "uid", "gid"
};

const char *glibtop_types_proc_state [GLIBTOP_MAX_PROC_STATE] =
{
	"char *", "char", "int", "int"
};

const char *glibtop_labels_proc_state [GLIBTOP_MAX_PROC_STATE] =
{
	N_("Cmd"), N_("State"), N_("UID"), N_("GID")
};

const char *glibtop_descriptions_proc_state [GLIBTOP_MAX_PROC_STATE] =
{
	N_("Basename of executable file in call to exec()"),
	N_("Single-Char code for process state (S=sleeping)"),
	N_("UID of process"),
	N_("GID of process")
};
