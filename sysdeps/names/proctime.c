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
#include <glibtop/proctime.h>

#define GLIBTOP_PROC_TIME_START_TIME	0
#define GLIBTOP_PROC_TIME_UTIME		1
#define GLIBTOP_PROC_TIME_STIME		2
#define GLIBTOP_PROC_TIME_CUTIME	3
#define GLIBTOP_PROC_TIME_CSTIME	4
#define GLIBTOP_PROC_TIME_TIMEOUT	5
#define GLIBTOP_PROC_TIME_IT_REAL_VALUE	6

const char *glibtop_names_proc_time [GLIBTOP_MAX_PROC_TIME] =
{
	"start_time", "utime", "stime", "cutime", "cstime",
	"timeout", "it_real_value"
};

const char *glibtop_types_proc_time [GLIBTOP_MAX_PROC_TIME] =
{
	GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG,
	GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG,
	GLIBTOP_TYPE_LONG
};

const char *glibtop_labels_proc_time [GLIBTOP_MAX_PROC_TIME] =
{
	N_("Start_Time"), N_("UTime"), N_("STime"), N_("CUTime"),
	N_("CSTime"), N_("TimeOut"), N_("It_Real_Value")
};

const char *glibtop_descriptions_proc_time [GLIBTOP_MAX_PROC_TIME] =
{
	N_("Start time of process in seconds since the epoch"),
	N_("user-mode CPU time accumulated by process"),
	N_("kernel-mode CPU time accumulated by process"),
	N_("cumulative utime of process and reaped children"),
	N_("cumulative stime of process and reaped children"),
	N_("The time (in jiffies) of the process's next timeout"),
	N_("The time (in jiffies) before the next SIGALRM is sent "
	   "to the process due to an interval timer.")
};


