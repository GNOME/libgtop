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
#include <glibtop/procsegment.h>

const char *glibtop_names_proc_segment [GLIBTOP_MAX_PROC_SEGMENT] =
{
	"text_rss", "shlib_rss", "data_rss", "stack_rss",
	"dirty_size", "start_code", "end_code", "start_stack"
};

const unsigned glibtop_types_proc_segment [GLIBTOP_MAX_PROC_SEGMENT] =
{
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG
};

const char *glibtop_labels_proc_segment [GLIBTOP_MAX_PROC_SEGMENT] =
{
	N_("Text_RSS"), N_("ShLib_RSS"), N_("Data_RSS"), N_("Stack_RSS"),
	N_("Dirty Size"), N_("Start_Code"), N_("End_Code"), N_("Start_Stack")
};

const char *glibtop_descriptions_proc_segment [GLIBTOP_MAX_PROC_SEGMENT] =
{
	N_("Text resident set size"),
	N_("Shared-Lib resident set size"),
	N_("Data resident set size"),
	N_("Stack resident set size"),
	N_("Total size of dirty pages"),
	N_("Address of beginning of code segment"),
	N_("Address of end of code segment"),
	N_("Address of the bottom of stack segment")
};
