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
#include <glibtop/procmem.h>
#include "libgtop-i18n.h"

#define GLIBTOP_PROC_MEM_SIZE		0
#define GLIBTOP_PROC_MEM_VSIZE		1
#define GLIBTOP_PROC_MEM_RESIDENT	2
#define GLIBTOP_PROC_MEM_SHARE		3
#define GLIBTOP_PROC_MEM_RSS		4
#define GLIBTOP_PROC_MEM_RSS_RLIM	5

const char *glibtop_names_proc_mem [GLIBTOP_MAX_PROC_MEM] =
{
	"size", "vsize", "resident", "share", "rss", "rss_rlim"
};

const unsigned glibtop_types_proc_mem [GLIBTOP_MAX_PROC_MEM] =
{
	GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG,
	GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG, GLIBTOP_TYPE_LONG
};

const char *glibtop_labels_proc_mem [GLIBTOP_MAX_PROC_MEM] =
{
	N_("Size"), N_("Virtual"), N_("Resident"), N_("Share"),
	N_("Resident Set Size"), N_("Resident Set Size Limit")
};

const char *glibtop_descriptions_proc_mem [GLIBTOP_MAX_PROC_MEM] =
{
	N_("Total # of pages of memory"),
	N_("Number of pages of virtual memory"),
	N_("Number of resident set (non-swapped) pages"),
	N_("Number of pages of shared (mmap'd) memory"),
	N_("Number of pages the process has in real memory, minus 3 "
	   "for administrative purposes. This is just the pages which "
	   "count towards text, data, or stack space. This does not "
	   "include pages which have not been demand-loaded in, or which "
	   "are swapped out."),
	N_("Current limit in bytes on the rss of the process "
	   "(usually 2,147,483,647).")
};
