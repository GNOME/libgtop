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
#include <glibtop/procsegment.h>

const char *glibtop_names_proc_segment[GLIBTOP_MAX_PROC_SEGMENT] =
{
    "text_rss", "shlib_rss", "data_rss", "stack_rss",
    "dirty_size", "start_code", "end_code", "start_data",
    "end_data", "start_brk", "end_brk", "start_stack",
    "start_mmap", "arg_start", "arg_end", "env_start",
    "env_end"
};

const unsigned glibtop_types_proc_segment[GLIBTOP_MAX_PROC_SEGMENT] =
{
    GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
    GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
    GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
    GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
    GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
    GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG
};

const char *glibtop_labels_proc_segment[GLIBTOP_MAX_PROC_SEGMENT] =
{
    N_ ("Text_RSS"), N_ ("ShLib_RSS"), N_ ("Data_RSS"), N_ ("Stack_RSS"),
    N_ ("Dirty Size"), N_ ("Start_Code"), N_ ("End_Code"),
    N_ ("Start_Data"), N_ ("End_Data"), N_ ("Start_Brk"), N_ ("Brk"),
    N_ ("Start_Stack"), N_ ("Start_MMap"), N_ ("Arg_Start"),
    N_ ("Arg_End"), N_ ("Env_Start"), N_ ("Env_End")
};

const char *glibtop_descriptions_proc_segment[GLIBTOP_MAX_PROC_SEGMENT] =
{
    N_ ("Text resident set size"),
    N_ ("Shared-Lib resident set size"),
    N_ ("Data resident set size"),
    N_ ("Stack resident set size"),
    N_ ("Total size of dirty pages"),
    N_ ("Address of beginning of code segment"),
    N_ ("Address of end of code segment"),
    N_ ("Address of beginning of data segment"),
    N_ ("Address of end of data segment"),
    N_ ("Brk_Start"),
    N_ ("Brk_End"),
    N_ ("Address of the bottom of stack segment"),
    N_ ("Start of mmap()ed areas"),
    N_ ("Arg_Start"),
    N_ ("Arg_End"),
    N_ ("Env_Start"),
    N_ ("Env_End")
};
