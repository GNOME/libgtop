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
#include <glibtop/procstate.h>
#include "libgtop-i18n.h"

const char *glibtop_names_proc_state [GLIBTOP_MAX_PROC_STATE] =
{
	"cmd", "state", "uid", "gid", "ruid", "rgid",
    "has_cpu", "processor", "last_processor"
};

const unsigned glibtop_types_proc_state [GLIBTOP_MAX_PROC_STATE] =
{
	GLIBTOP_TYPE_STRING, GLIBTOP_TYPE_CHAR, GLIBTOP_TYPE_INT,
    GLIBTOP_TYPE_INT,    GLIBTOP_TYPE_INT,  GLIBTOP_TYPE_INT,
    GLIBTOP_TYPE_INT,    GLIBTOP_TYPE_INT,  GLIBTOP_TYPE_INT
};

const char *glibtop_labels_proc_state [GLIBTOP_MAX_PROC_STATE] =
{
	N_("Cmd"), N_("State"), N_("UID"), N_("GID"),
    N_("RGid"), N_("RUid"), N_("HasCPU"), N_("Proc"), N_("LProc")
};

const char *glibtop_descriptions_proc_state [GLIBTOP_MAX_PROC_STATE] =
{
	N_("Basename of executable file in call to exec()"),
	N_("Single-Char code for process state (S=sleeping)"),
	N_("UID of process"),
	N_("GID of process"),
	N_("Real UID of process"),
	N_("Real GID of process"),
	N_("Has CPU"),
	N_("Processor"),
	N_("Last Processor"),
};
