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

#include <locale.h>

#include <glibtop.h>
     
#include <glibtop/open.h>
#include <glibtop/close.h>
#include <glibtop/xmalloc.h>

#include <glibtop/union.h>
#include <glibtop/sysdeps.h>

void main_prog(int argc, char *argv[]);

#ifdef HAVE_LINUX_TABLE
#include <unistd.h>
#include <linux/unistd.h>
#include <linux/table.h>

#include <syscall.h>

_syscall3 (int, table, int, type, union table *, tbl, const void *, param);
#endif

int     
main (int argc, char *argv[])
{
	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE, GTOPLOCALEDIR);
	textdomain (PACKAGE);

	gh_enter (argc, argv, main_prog);
	exit (0);
}

void
main_prog (int argc, char *argv[])
{
	glibtop_boot_guile ();
#ifdef GLIBTOP_GUILE_NAMES
	glibtop_boot_guile_names ();
#endif

	gh_repl (argc, argv);
}
