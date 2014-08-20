/*
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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <config.h>
#include <unistd.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/proclist.h>

#include <glibtop_suid.h>

#include <sys/param.h>
#include <sys/sysctl.h>  

static const unsigned long _glibtop_sysdeps_proclist =
(1L << GLIBTOP_PROCLIST_TOTAL) + (1L << GLIBTOP_PROCLIST_NUMBER) +
(1L << GLIBTOP_PROCLIST_SIZE);

/* Init function. */

void
_glibtop_init_proclist_p (glibtop *server)
{
	server->sysdeps.proclist = _glibtop_sysdeps_proclist;
}

pid_t *
glibtop_get_proclist_p (glibtop *server, glibtop_proclist *buf,
			gint64 which, gint64 arg)
{
	unsigned count, total, i;
	pid_t *pids_chain;
	int mib[4];
	struct kinfo_proc *kp;
	size_t length;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROCLIST), 0);

	memset (buf, 0, sizeof (glibtop_proclist));

	mib [0] = CTL_KERN;
	mib [1] = KERN_PROC;
	mib [2] = (int)(which & GLIBTOP_KERN_PROC_MASK);
	mib [3] = (int)arg;

	if (sysctl (mib, 4, NULL, &length, NULL, 0) < 0) {
		glibtop_warn_io_r (server, "sysctl (proclist)");
		return NULL;
	}
	if ((kp = (struct kinfo_proc *) g_malloc(length)) == NULL) {
		glibtop_warn_io_r (server, "malloc (proclist)");
		return NULL;
	}
	if (sysctl (mib, 4, kp, &length, NULL, 0) < 0) {
		glibtop_warn_io_r (server, "sysctl (proclist)");
		g_free (kp);
		return NULL;
	}

	count = length / sizeof (struct kinfo_proc);
	pids_chain = g_malloc(count * sizeof (unsigned));
	if (pids_chain ==  NULL) {
		glibtop_warn_io_r (server, "g_realloc (proclist)");
		g_free (kp);
		return NULL;
	}

	for (total = 0, i = 0; i < count; i++) {
		if ((which & GLIBTOP_EXCLUDE_IDLE) &&
		    (kp [i].kp_proc.p_stat != SRUN))
			continue;
		if ((which & GLIBTOP_EXCLUDE_SYSTEM) &&
			 (kp [i].kp_eproc.e_pcred.p_ruid == 0))
			continue;
		pids_chain [total++] = (unsigned) kp [i].kp_proc.p_pid;
	}

	g_free (kp);

	buf->number = total;
	buf->size = sizeof (unsigned);
	buf->total = total * sizeof (unsigned);
	buf->flags = _glibtop_sysdeps_proclist;

	return pids_chain;
}

