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

#include <glibtop/mem.h>

const char *glibtop_names_mem [GLIBTOP_MAX_MEM] =
{ 
	"total", "used", "free", "shared", "buffer",
	"cached", "user", "locked"
};

const unsigned glibtop_types_mem [GLIBTOP_MAX_MEM] =
{ 
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG
};

const char *glibtop_labels_mem [GLIBTOP_MAX_MEM] =
{
	N_("Total Memory"),
	N_("Used Memory"),
	N_("Free Memory"),
	N_("Shared Memory"),
	N_("Buffers"),
	N_("Cached"),
	N_("User"),
	N_("Locked"),
};

const char *glibtop_descriptions_mem [GLIBTOP_MAX_MEM] =
{
	N_("Total physical memory in kB"),
	N_("Used memory size in kB"),
	N_("Free memory size in kB"),
	N_("Shared memory size in kB"),
	N_("Size of buffers kB"),
	N_("Size of cached memory in kB"),
	N_("Memory used from user processes in kB"),
	N_("Memory in locked pages in kB")
};
