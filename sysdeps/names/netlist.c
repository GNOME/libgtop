/* Copyright (C) 2004 Benoît Dejean
   This file is part of LibGTop 2.0.

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

#include <glibtop/netlist.h>
#include "libgtop-i18n.h"

const char *glibtop_names_netlist [GLIBTOP_MAX_NETLIST] =
{
	"number"
};

const unsigned glibtop_types_netlist [GLIBTOP_MAX_NETLIST] =
{
	GLIBTOP_TYPE_ULONG
};

const char *glibtop_labels_netlist [GLIBTOP_MAX_NETLIST] =
{
	N_("Number"),
};

const char *glibtop_descriptions_netlist [GLIBTOP_MAX_NETLIST] =
{
	N_("Number of network devices found"),
};

