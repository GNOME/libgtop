/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

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
#include <glibtop/proctime.h>

const char *glibtop_names_proc_time[GLIBTOP_MAX_PROC_TIME] =
{
    "start_time", "rtime", "utime", "stime", "cutime", "cstime",
    "timeout", "it_real_value", "timeout", "xcpu_utime",
    "xcpu_stime", "xcpu_flags"
};

const unsigned glibtop_types_proc_time[GLIBTOP_MAX_PROC_TIME] =
{
    GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG,
    GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG,
    GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG,
    GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG
};

const char *glibtop_labels_proc_time[GLIBTOP_MAX_PROC_TIME] =
{
    N_ ("Start_Time"), N_ ("RTime"), N_ ("UTime"), N_ ("STime"),
    N_ ("CUTime"), N_ ("CSTime"), N_ ("TimeOut"), N_ ("It_Real_Value"),
    N_ ("Frequency"), N_ ("XCPU_UTime"), N_ ("XCPU_STime"),
    N_ ("XCPU_Flags")
};

const char *glibtop_descriptions_proc_time[GLIBTOP_MAX_PROC_TIME] =
{
    N_ ("Start time of process in seconds since the epoch"),
    N_ ("Real time accumulated by process (should be utime + stime)"),
    N_ ("user-mode CPU time accumulated by process"),
    N_ ("kernel-mode CPU time accumulated by process"),
    N_ ("cumulative utime of process and reaped children"),
    N_ ("cumulative stime of process and reaped children"),
    N_ ("The time (in jiffies) of the process's next timeout"),
    N_ ("The time (in jiffies) before the next SIGALRM is sent "
	"to the process due to an interval timer."),
    N_ ("Tick frequency"),
    N_ ("SMP user-mode CPU time accumulated by process"),
    N_ ("SMP kernel-mode CPU time accumulated by process"),
    N_ ("SMP CPU Flags")
};
