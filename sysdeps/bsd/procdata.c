/* Copyright (C) 1998 Joshua Sled
   This file is part of LibGTop 1.0.

   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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
#include <glibtop/procdata.h>

#include <sys/stat.h>


#define BIT_SHIFT(x)		(1L << (x % 64))

static const unsigned long _glibtop_sysdeps_procdata_0 =
BIT_SHIFT(GLIBTOP_PROCDATA_CMD) +
BIT_SHIFT(GLIBTOP_PROCDATA_STATE) +
BIT_SHIFT(GLIBTOP_PROCDATA_UID) +
BIT_SHIFT(GLIBTOP_PROCDATA_PID) +
BIT_SHIFT(GLIBTOP_PROCDATA_PPID) +
BIT_SHIFT(GLIBTOP_PROCDATA_PGRP) +
BIT_SHIFT(GLIBTOP_PROCDATA_SESSION) +
BIT_SHIFT(GLIBTOP_PROCDATA_TTY) +
BIT_SHIFT(GLIBTOP_PROCDATA_TPGID) +
BIT_SHIFT(GLIBTOP_PROCDATA_PRIORITY) +
BIT_SHIFT(GLIBTOP_PROCDATA_NICE) +
BIT_SHIFT(GLIBTOP_PROCDATA_SIGNAL) +
BIT_SHIFT(GLIBTOP_PROCDATA_BLOCKED) +
BIT_SHIFT(GLIBTOP_PROCDATA_SIGIGNORE) +
BIT_SHIFT(GLIBTOP_PROCDATA_SIGCATCH) +
BIT_SHIFT(GLIBTOP_PROCDATA_START_TIME) +
BIT_SHIFT(GLIBTOP_PROCDATA_UTIME) +
BIT_SHIFT(GLIBTOP_PROCDATA_STIME) +
BIT_SHIFT(GLIBTOP_PROCDATA_CUTIME) +
BIT_SHIFT(GLIBTOP_PROCDATA_CSTIME) +
BIT_SHIFT(GLIBTOP_PROCDATA_SIZE) +
BIT_SHIFT(GLIBTOP_PROCDATA_RESIDENT) +
BIT_SHIFT(GLIBTOP_PROCDATA_SHARE) +
BIT_SHIFT(GLIBTOP_PROCDATA_TRS) +
BIT_SHIFT(GLIBTOP_PROCDATA_LRS) +
BIT_SHIFT(GLIBTOP_PROCDATA_DRS) +
BIT_SHIFT(GLIBTOP_PROCDATA_DT) +
BIT_SHIFT(GLIBTOP_PROCDATA_VSIZE) +
BIT_SHIFT(GLIBTOP_PROCDATA_RSS) +
BIT_SHIFT(GLIBTOP_PROCDATA_RSS_RLIM) +
BIT_SHIFT(GLIBTOP_PROCDATA_TIMEOUT) +
BIT_SHIFT(GLIBTOP_PROCDATA_IT_REAL_VALUE);

static const unsigned long _glibtop_sysdeps_procdata_1 =
BIT_SHIFT(GLIBTOP_PROCDATA_K_FLAGS) +
BIT_SHIFT(GLIBTOP_PROCDATA_MIN_FLT) +
BIT_SHIFT(GLIBTOP_PROCDATA_MAJ_FLT) +
BIT_SHIFT(GLIBTOP_PROCDATA_CMIN_FLT) +
BIT_SHIFT(GLIBTOP_PROCDATA_CMAJ_FLT) +
BIT_SHIFT(GLIBTOP_PROCDATA_START_CODE) +
BIT_SHIFT(GLIBTOP_PROCDATA_END_CODE) +
BIT_SHIFT(GLIBTOP_PROCDATA_START_STACK) +
BIT_SHIFT(GLIBTOP_PROCDATA_KSTK_ESP) +
BIT_SHIFT(GLIBTOP_PROCDATA_KSTK_EIP) +
BIT_SHIFT(GLIBTOP_PROCDATA_WCHAN);

/* Provides detailed information about a process. */

void
glibtop_get_procdata_s (glibtop *server, glibtop_procdata *buf, pid_t pid)
{
	char input [BUFSIZ], *tmp;
	struct stat statb;
	int nread;
	FILE *f;

	glibtop_init_r (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_procdata));

	if (pid == 0) {
		/* Client is only interested in the flags. */
		buf->flags [0] = _glibtop_sysdeps_procdata_0;
		buf->flags [1] = _glibtop_sysdeps_procdata_1;
		return;
	}


	sprintf (input, "/proc/%d/stat", pid);

	if (stat (input, &statb)) return;

	buf->uid = statb.st_uid;

	f = fopen (input, "r");
	if (!f) return;

	nread = fread (input, 1, BUFSIZ, f);

	if (nread < 0) {
		fclose (f);
		return;
	}

	input [nread] = 0;

	/* This is from guile-utils/gtop/proc/readproc.c */

	/* split into "PID (cmd" and "<rest>" */
	tmp = strrchr (input, ')');
	*tmp = '\0';		/* replace trailing ')' with NUL */
	/* parse these two strings separately, skipping the leading "(". */
	memset (buf->cmd, 0, sizeof (buf->cmd));
	sscanf (input, "%d (%39c", &buf->pid, buf->cmd);
	sscanf(tmp + 2,		/* skip space after ')' too */
	       "%c %d %d %d %d %d %lu %lu %lu %lu %lu "
	       "%ld %ld %ld %ld %d %d %lu %lu %ld %lu "
	       "%lu %lu %lu %lu %lu %lu %lu %d %d %d %d %lu",
	       &buf->state, &buf->ppid, &buf->pgrp, &buf->session,
	       &buf->tty, &buf->tpgid, &buf->k_flags, &buf->min_flt,
	       &buf->cmin_flt, &buf->maj_flt, &buf->cmaj_flt,
	       &buf->utime, &buf->stime, &buf->cutime, &buf->cstime,
	       &buf->priority, &buf->nice, &buf->timeout,
	       &buf->it_real_value, &buf->start_time, &buf->vsize,
	       &buf->rss, &buf->rss_rlim, &buf->start_code,
	       &buf->end_code, &buf->start_stack, &buf->kstk_esp,
	       &buf->kstk_eip, &buf->signal, &buf->blocked,
	       &buf->sigignore, &buf->sigcatch, &buf->wchan);

	if (buf->tty == 0)
		/* the old notty val, update elsewhere bef. moving to 0 */
		buf->tty = -1;

	fclose (f);

	sprintf (input, "/proc/%d/statm", pid);

	f = fopen (input, "r");
	if (!f) return;

	nread = fread (input, 1, BUFSIZ, f);

	if (nread < 0) {
		fclose (f);
		return;
	}

	input [nread] = 0;

	sscanf (input, "%ld %ld %ld %ld %ld %ld %ld",
		&buf->size, &buf->resident, &buf->share,
		&buf->trs, &buf->lrs, &buf->drs, &buf->dt);

	fclose (f);

	buf->flags [0] = _glibtop_sysdeps_procdata_0;
	buf->flags [1] = _glibtop_sysdeps_procdata_1;
}
