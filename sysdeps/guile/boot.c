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
#include <glibtop/sysdeps.h>

#include <guile/gh.h>

void
glibtop_boot_guile (void)
{
	gh_new_procedure0_0
		("glibtop-get-cpu", glibtop_guile_get_cpu);
	gh_new_procedure0_0
		("glibtop-get-mem", glibtop_guile_get_mem);
	gh_new_procedure0_0
		("glibtop-get-swap", glibtop_guile_get_swap);
	gh_new_procedure0_0
		("glibtop-get-uptime", glibtop_guile_get_uptime);
	gh_new_procedure0_0
		("glibtop-get-loadavg", glibtop_guile_get_loadavg);
	gh_new_procedure0_0
		("glibtop-get-shm_limits",glibtop_guile_get_shm_limits);
	gh_new_procedure0_0
		("glibtop-get-msg_limits", glibtop_guile_get_msg_limits);
	gh_new_procedure0_0
		("glibtop-get-sem_limits", glibtop_guile_get_sem_limits);
	gh_new_procedure0_0
		("glibtop-get-sysdeps", glibtop_guile_get_sysdeps);
	gh_new_procedure0_0
		("glibtop-get-proclist", glibtop_guile_get_proclist);

	gh_new_procedure1_0
		("glibtop-get-proc_state", glibtop_guile_get_proc_state);
	gh_new_procedure1_0
		("glibtop-get-proc_uid", glibtop_guile_get_proc_uid);
	gh_new_procedure1_0
		("glibtop-get-proc_mem", glibtop_guile_get_proc_mem);
	gh_new_procedure1_0
		("glibtop-get-proc_time", glibtop_guile_get_proc_time);
	gh_new_procedure1_0
		("glibtop-get-proc_signal", glibtop_guile_get_proc_signal);
	gh_new_procedure1_0
		("glibtop-get-proc_kernel", glibtop_guile_get_proc_kernel);
	gh_new_procedure1_0
		("glibtop-get-proc_segment", glibtop_guile_get_proc_segment);

}
