/* $Id$ */

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

#include <glibtop.h>
#include <glibtop/procsignal.h>

const char *glibtop_names_proc_signal [GLIBTOP_MAX_PROC_SIGNAL] =
{
	"signal", "blocked", "sigignore", "sigcatch"
};

const unsigned glibtop_types_proc_signal [GLIBTOP_MAX_PROC_SIGNAL] =
{
	GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT,
	GLIBTOP_TYPE_INT
};

const char *glibtop_labels_proc_signal [GLIBTOP_MAX_PROC_SIGNAL] =
{
	N_("Signal"), N_("Blocked"), N_("SigIgnore"), N_("SigCatch")
};

const char *glibtop_descriptions_proc_signal [GLIBTOP_MAX_PROC_SIGNAL] =
{
	N_("Mask of pending signals"),
	N_("Mask of blocked signals"),
	N_("Mask of ignored signals"),
	N_("Mask of caught signals")
};
