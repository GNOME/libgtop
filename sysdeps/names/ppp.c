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

#include <glibtop/ppp.h>

const char *glibtop_names_ppp [GLIBTOP_MAX_PPP] =
{
	"state", "bytes_in", "bytes_out"
};

const unsigned glibtop_types_ppp [GLIBTOP_MAX_PPP] =
{ 
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG
};

const char *glibtop_labels_ppp [GLIBTOP_MAX_PPP] =
{
	N_("PPP State"),
	N_("Input bytes"),
	N_("Output bytes")
};

const char *glibtop_descriptions_ppp [GLIBTOP_MAX_PPP] =
{
	N_("PPP State"),
	N_("Number of input bytes"),
	N_("Number of output bytes")
};
