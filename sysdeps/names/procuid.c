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
#include <glibtop/procuid.h>

#define GLIBTOP_PROC_UID_UID		0
#define GLIBTOP_PROC_UID_EUID		1
#define GLIBTOP_PROC_UID_GID		2
#define GLIBTOP_PROC_UID_EGID		3
#define GLIBTOP_PROC_UID_PID		4
#define GLIBTOP_PROC_UID_PPID		5
#define GLIBTOP_PROC_UID_PGRP		6
#define GLIBTOP_PROC_UID_SESSION	7
#define GLIBTOP_PROC_UID_TTY		8
#define GLIBTOP_PROC_UID_TPGID		9
#define GLIBTOP_PROC_UID_PRIORITY	10
#define GLIBTOP_PROC_UID_NICE		11

const char *glibtop_names_proc_uid [GLIBTOP_MAX_PROC_UID] =
{
	"uid", "euid", "gid", "egid", "pid", "ppid", "pgrp",
	"session", "tty", "tpgid", "priority", "nice"
};

const char *glibtop_types_proc_uid [GLIBTOP_MAX_PROC_UID] =
{
	GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT,
	GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT,
	GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT,
	GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT, GLIBTOP_TYPE_INT
};

const char *glibtop_labels_proc_uid [GLIBTOP_MAX_PROC_UID] =
{
	N_("Uid"), N_("EUid"), N_("Gid"), N_("EGid"), N_("Pid"),
	N_("PPid"), N_("PGrp"), N_("Session"), N_("Tty"),
	N_("TPGid"), N_("Priority"), N_("Nice")
};

const char *glibtop_descriptions_proc_uid [GLIBTOP_MAX_PROC_UID] =
{
	N_("User ID"),
	N_("Effective User ID"),
	N_("Group ID"),
	N_("Effective Group ID"),
	N_("Process ID"),
	N_("PID of parent process"),
	N_("Process group ID"),
	N_("Session ID"),
	N_("Full device number of controlling terminal"),
	N_("Terminal process group ID"),
	N_("Kernel scheduling priority"),
	N_("Standard unix nice level of process")
};
