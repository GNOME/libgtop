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

#include <glibtop/loadavg.h>

const char *glibtop_names_loadavg [GLIBTOP_MAX_LOADAVG] =
{
	"loadavg", "nr_running", "nr_tasks", "last_pid"
};

const unsigned glibtop_types_loadavg [GLIBTOP_MAX_LOADAVG] =
{
	GLIBTOP_TYPE_DOUBLE, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG
};

const char *glibtop_labels_loadavg [GLIBTOP_MAX_LOADAVG] =
{
	N_("Load Average"),
	N_("Running Tasks"),
	N_("Number of Tasks"),
	N_("Last PID")
};

const char *glibtop_descriptions_loadavg [GLIBTOP_MAX_LOADAVG] =
{
	N_("Number of jobs running simultaneously averaged over 1, 5 and 15 minutes"),
	N_("Number of tasks currently running"),
	N_("Total number of tasks"),
	N_("Last PID")
};
