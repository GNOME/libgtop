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

#include <glibtop/cpu.h>

const char *glibtop_names_cpu[GLIBTOP_MAX_CPU] =
{
    "total", "user", "nice", "sys", "idle", "frequency",
    "xcpu_total", "xcpu_user", "xcpu_nice", "xcpu_sys",
    "xcpu_idle", "xcpu_flags"
};

const unsigned glibtop_types_cpu[GLIBTOP_MAX_CPU] =
{
    GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
    GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
    GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
    GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG
};

const char *glibtop_labels_cpu[GLIBTOP_MAX_CPU] =
{
    N_ ("Total CPU Time"),
    N_ ("CPU Time in User Mode"),
    N_ ("CPU Time in User Mode (nice)"),
    N_ ("CPU Time in System Mode"),
    N_ ("CPU Time in the Idle Task"),
    N_ ("Tick Frequency"),
    N_ ("SMP Total CPU Time"),
    N_ ("SMP CPU Time in User Mode"),
    N_ ("SMP CPU Time in User Mode (nice)"),
    N_ ("SMP CPU Time in System Mode"),
    N_ ("SMP CPU Time in the Idle Task")
    N_ ("SMP CPU Flags")
};

const char *glibtop_descriptions_cpu[GLIBTOP_MAX_CPU] =
{
    N_ ("Number of clock ticks since system boot"),
    N_ ("Number of clock ticks the system spent in user mode"),
    N_ ("Number of clock ticks the system spent in user mode (nice)"),
    N_ ("Number of clock ticks the system spent in system mode"),
    N_ ("Number of clock ticks the system spent in the idle task"),
    N_ ("Tick frequency (default is 100)"),
    N_ ("Number of clock ticks since system boot"),
    N_ ("Number of clock ticks the system spent in user mode"),
    N_ ("Number of clock ticks the system spent in user mode (nice)"),
    N_ ("Number of clock ticks the system spent in system mode"),
    N_ ("Number of clock ticks the system spent in the idle task"),
    N_ ("SMP CPU Flags")
};
