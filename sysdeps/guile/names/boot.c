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
glibtop_boot_guile_names (void)
{
	gh_new_procedure0_0
		("glibtop-names-cpu", glibtop_guile_names_cpu);
	gh_new_procedure0_0
		("glibtop-names-mem", glibtop_guile_names_mem);
	gh_new_procedure0_0
		("glibtop-names-swap", glibtop_guile_names_swap);
	gh_new_procedure0_0
		("glibtop-names-uptime", glibtop_guile_names_uptime);
	gh_new_procedure0_0
		("glibtop-names-loadavg", glibtop_guile_names_loadavg);
	gh_new_procedure0_0
		("glibtop-names-shm_limits", glibtop_guile_names_shm_limits);
	gh_new_procedure0_0
		("glibtop-names-msg_limits", glibtop_guile_names_msg_limits);
	gh_new_procedure0_0
		("glibtop-names-sem_limits", glibtop_guile_names_sem_limits);
	gh_new_procedure0_0
		("glibtop-names-sysdeps", glibtop_guile_names_sysdeps);
	gh_new_procedure0_0
		("glibtop-names-proclist", glibtop_guile_names_proclist);

	gh_new_procedure0_0
		("glibtop-names-proc_state", glibtop_guile_names_proc_state);
	gh_new_procedure0_0
		("glibtop-names-proc_uid", glibtop_guile_names_proc_uid);
	gh_new_procedure0_0
		("glibtop-names-proc_mem", glibtop_guile_names_proc_mem);
	gh_new_procedure0_0
		("glibtop-names-proc_time", glibtop_guile_names_proc_time);
	gh_new_procedure0_0
		("glibtop-names-proc_signal", glibtop_guile_names_proc_signal);
	gh_new_procedure0_0
		("glibtop-names-proc_kernel", glibtop_guile_names_proc_kernel);
	gh_new_procedure0_0
		("glibtop-names-proc_segment", glibtop_guile_names_proc_segment);

	gh_new_procedure0_0
		("glibtop-types-cpu", glibtop_guile_types_cpu);
	gh_new_procedure0_0
		("glibtop-types-mem", glibtop_guile_types_mem);
	gh_new_procedure0_0
		("glibtop-types-swap", glibtop_guile_types_swap);
	gh_new_procedure0_0
		("glibtop-types-uptime", glibtop_guile_types_uptime);
	gh_new_procedure0_0
		("glibtop-types-loadavg", glibtop_guile_types_loadavg);
	gh_new_procedure0_0
		("glibtop-types-shm_limits", glibtop_guile_types_shm_limits);
	gh_new_procedure0_0
		("glibtop-types-msg_limits", glibtop_guile_types_msg_limits);
	gh_new_procedure0_0
		("glibtop-types-sem_limits", glibtop_guile_types_sem_limits);
	gh_new_procedure0_0
		("glibtop-types-sysdeps", glibtop_guile_types_sysdeps);
	gh_new_procedure0_0
		("glibtop-types-proclist", glibtop_guile_types_proclist);

	gh_new_procedure0_0
		("glibtop-types-proc_state", glibtop_guile_types_proc_state);
	gh_new_procedure0_0
		("glibtop-types-proc_uid", glibtop_guile_types_proc_uid);
	gh_new_procedure0_0
		("glibtop-types-proc_mem", glibtop_guile_types_proc_mem);
	gh_new_procedure0_0
		("glibtop-types-proc_time", glibtop_guile_types_proc_time);
	gh_new_procedure0_0
		("glibtop-types-proc_signal", glibtop_guile_types_proc_signal);
	gh_new_procedure0_0
		("glibtop-types-proc_kernel", glibtop_guile_types_proc_kernel);
	gh_new_procedure0_0
		("glibtop-types-proc_segment", glibtop_guile_types_proc_segment);


	gh_new_procedure0_0
		("glibtop-labels-cpu", glibtop_guile_labels_cpu);
	gh_new_procedure0_0
		("glibtop-labels-mem", glibtop_guile_labels_mem);
	gh_new_procedure0_0
		("glibtop-labels-swap", glibtop_guile_labels_swap);
	gh_new_procedure0_0
		("glibtop-labels-uptime", glibtop_guile_labels_uptime);
	gh_new_procedure0_0
		("glibtop-labels-loadavg", glibtop_guile_labels_loadavg);
	gh_new_procedure0_0
		("glibtop-labels-shm_limits", glibtop_guile_labels_shm_limits);
	gh_new_procedure0_0
		("glibtop-labels-msg_limits", glibtop_guile_labels_msg_limits);
	gh_new_procedure0_0
		("glibtop-labels-sem_limits", glibtop_guile_labels_sem_limits);
	gh_new_procedure0_0
		("glibtop-labels-sysdeps", glibtop_guile_labels_sysdeps);
	gh_new_procedure0_0
		("glibtop-labels-proclist", glibtop_guile_labels_proclist);

	gh_new_procedure0_0
		("glibtop-labels-proc_state", glibtop_guile_labels_proc_state);
	gh_new_procedure0_0
		("glibtop-labels-proc_uid", glibtop_guile_labels_proc_uid);
	gh_new_procedure0_0
		("glibtop-labels-proc_mem", glibtop_guile_labels_proc_mem);
	gh_new_procedure0_0
		("glibtop-labels-proc_time", glibtop_guile_labels_proc_time);
	gh_new_procedure0_0
		("glibtop-labels-proc_signal", glibtop_guile_labels_proc_signal);
	gh_new_procedure0_0
		("glibtop-labels-proc_kernel", glibtop_guile_labels_proc_kernel);
	gh_new_procedure0_0
		("glibtop-labels-proc_segment", glibtop_guile_labels_proc_segment);

	gh_new_procedure0_0
		("glibtop-descriptions-cpu", glibtop_guile_descriptions_cpu);
	gh_new_procedure0_0
		("glibtop-descriptions-mem", glibtop_guile_descriptions_mem);
	gh_new_procedure0_0
		("glibtop-descriptions-swap", glibtop_guile_descriptions_swap);
	gh_new_procedure0_0
		("glibtop-descriptions-uptime", glibtop_guile_descriptions_uptime);
	gh_new_procedure0_0
		("glibtop-descriptions-loadavg", glibtop_guile_descriptions_loadavg);
	gh_new_procedure0_0
		("glibtop-descriptions-shm_limits", glibtop_guile_descriptions_shm_limits);
	gh_new_procedure0_0
		("glibtop-descriptions-msg_limits", glibtop_guile_descriptions_msg_limits);
	gh_new_procedure0_0
		("glibtop-descriptions-sem_limits", glibtop_guile_descriptions_sem_limits);
	gh_new_procedure0_0
		("glibtop-descriptions-sysdeps", glibtop_guile_descriptions_sysdeps);
	gh_new_procedure0_0
		("glibtop-descriptions-proclist", glibtop_guile_descriptions_proclist);

	gh_new_procedure0_0
		("glibtop-descriptions-proc_state", glibtop_guile_descriptions_proc_state);
	gh_new_procedure0_0
		("glibtop-descriptions-proc_uid", glibtop_guile_descriptions_proc_uid);
	gh_new_procedure0_0
		("glibtop-descriptions-proc_mem", glibtop_guile_descriptions_proc_mem);
	gh_new_procedure0_0
		("glibtop-descriptions-proc_time", glibtop_guile_descriptions_proc_time);
	gh_new_procedure0_0
		("glibtop-descriptions-proc_signal", glibtop_guile_descriptions_proc_signal);
	gh_new_procedure0_0
		("glibtop-descriptions-proc_kernel", glibtop_guile_descriptions_proc_kernel);
	gh_new_procedure0_0
		("glibtop-descriptions-proc_segment", glibtop_guile_descriptions_proc_segment);
}
