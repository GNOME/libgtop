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
#include <glibtop/procuid.h>
#include "libgtop-i18n.h"

const char *glibtop_names_proc_uid [GLIBTOP_MAX_PROC_UID] =
{
	"uid", "euid", "gid", "egid", "suid", "sgid",
	"fsuid", "fsgid", "pid", "ppid", "pgrp",
	"session", "tty", "tpgid", "priority", "nice",
	"ngroups", "groups"
};

const unsigned glibtop_types_proc_uid [GLIBTOP_MAX_PROC_UID] =
{
	GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT,
	GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT,
	GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT,
	GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT,
	GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT,
	GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT
};

const char *glibtop_labels_proc_uid [GLIBTOP_MAX_PROC_UID] =
{
	N_("Uid"), N_("EUid"), N_("Gid"), N_("EGid"), N_("SUid"),
	N_("SGid"), N_("FSUid"), N_("FSGid"), N_("Pid"),
	N_("PPid"), N_("PGrp"), N_("Session"), N_("Tty"),
	N_("TPGid"), N_("Priority"), N_("Nice"), N_("NGroups"), N_("Groups")
};

const char *glibtop_descriptions_proc_uid [GLIBTOP_MAX_PROC_UID] =
{
	N_("User ID"),
	N_("Effective User ID"),
	N_("Group ID"),
	N_("Effective Group ID"),
	N_("Set User ID"),
	N_("Set Group ID"),
	N_("Filesystem User ID"),
	N_("Filesystem Group ID"),
	N_("Process ID"),
	N_("PID of parent process"),
	N_("Process group ID"),
	N_("Session ID"),
	N_("Full device number of controlling terminal"),
	N_("Terminal process group ID"),
	N_("Kernel scheduling priority"),
	N_("Standard unix nice level of process"),
	N_("Number of additional process groups"),
	N_("Array of additional process groups")
};
