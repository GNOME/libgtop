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

#include <glibtop/mem.h>

const char *glibtop_names_mem [GLIBTOP_MAX_MEM] =
{ 
	"total", "used", "free", "shared", "buffer",
	"cached", "user"
};

const char *glibtop_types_mem [GLIBTOP_MAX_MEM] =
{ 
	"unsigned long", "unsigned long", "unsigned long",
	"unsigned long", "unsigned long", "unsigned long",
	"unsigned long"
};

const char *glibtop_labels_mem [GLIBTOP_MAX_MEM] =
{
	N_("Total Memory"),
	N_("Used Memory"),
	N_("Free Memory"),
	N_("Shared Memory"),
	N_("Buffers"),
	N_("Cached"),
	N_("User")
};

const char *glibtop_descriptions_mem [GLIBTOP_MAX_MEM] =
{
	N_("Total physical memory in kB"),
	N_("Used memory size in kB"),
	N_("Free memory size in kB"),
	N_("Shared memory size in kB"),
	N_("Size of buffers kB"),
	N_("Size of cached memory in kB"),
	N_("Memory used from user processes in kB")
};
