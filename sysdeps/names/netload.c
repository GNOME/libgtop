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

#include <glibtop/netload.h>

const char *glibtop_names_netload [GLIBTOP_MAX_NETLOAD] =
{
	"if_flags", "mtu", "subnet", "address", "packets_in",
	"packets_out", "packets_total", "bytes_in", "bytes_out",
	"bytes_total", "errors_in", "errors_out", "errors_total",
	"collisions"
};

const unsigned glibtop_types_netload [GLIBTOP_MAX_NETLOAD] =
{ 
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG
};

const char *glibtop_labels_netload [GLIBTOP_MAX_NETLOAD] =
{
	N_("Interface Flags"),
	N_("MTU"),
	N_("Subnet"),
	N_("Address"),
	N_("Packets In"),
	N_("Packets Out"),
	N_("Packets Total"),
	N_("Bytes In"),
	N_("Bytes Out"),
	N_("Bytes Total"),
	N_("Errors In"),
	N_("Errors Out"),
	N_("Errors Total"),
	N_("Collisions")
};

const char *glibtop_descriptions_netload [GLIBTOP_MAX_NETLOAD] =
{
	N_("Interface Flags"),
	N_("Maximum Transfer Unit"),
	N_("Subnet"),
	N_("Address"),
	N_("Packets In"),
	N_("Packets Out"),
	N_("Packets Total"),
	N_("Bytes In"),
	N_("Bytes Out"),
	N_("Bytes Total"),
	N_("Errors In"),
	N_("Errors Out"),
	N_("Errors Total"),
	N_("Collisions")
};
