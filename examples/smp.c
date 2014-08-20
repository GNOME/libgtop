/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, September 1998.

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

#include <locale.h>
#include <libintl.h>
#include <math.h>
#include <stdio.h>

#include <glibtop.h>
#include <glibtop/cpu.h>

int
main (int argc, char *argv [])
{
	glibtop_cpu cpu;
	unsigned long frequency;
	double total, user, nice, sys, idle;
	double b_total, b_user, b_nice, b_sys, b_idle;
	double s_total, s_user, s_nice, s_sys, s_idle;
	char separator [BUFSIZ], buffer [BUFSIZ];
	int ncpu, i;

	glibtop_init();

	glibtop_get_cpu (&cpu);

	ncpu = glibtop_global_server->ncpu ? glibtop_global_server->ncpu : 1;

	frequency = (unsigned long) cpu.frequency;

	total = ((unsigned long) cpu.total) ? ((double) cpu.total) : 1.0;
	user  = ((unsigned long) cpu.user)  ? ((double) cpu.user)  : 1.0;
	nice  = ((unsigned long) cpu.nice)  ? ((double) cpu.nice)  : 1.0;
	sys   = ((unsigned long) cpu.sys)   ? ((double) cpu.sys)   : 1.0;
	idle  = ((unsigned long) cpu.idle)  ? ((double) cpu.idle)  : 1.0;

	s_total = s_user = s_nice = s_sys = s_idle = 0.0;

	b_total = total / ncpu;
	b_user  = user  / ncpu;
	b_nice  = nice  / ncpu;
	b_sys   = sys   / ncpu;
	b_idle  = idle  / ncpu;

	memset (separator, '-', 91);
	separator [92] = '\0';

	sprintf (buffer, "Ticks (%ld per second):", frequency);

	printf ("\n\n%-26s %12s %12s %12s %12s %12s\n%s\n", buffer,
		"Total", "User", "Nice", "Sys", "Idle", separator);

	printf ("CPU          (0x%08lx): %12.0f %12.0f %12.0f %12.0f %12.0f\n\n",
		(unsigned long) cpu.flags, total, user, nice, sys, idle);

	for (i = 0; i < glibtop_global_server->ncpu; i++) {
		printf ("CPU %3d      (0x%08lx): %12lu %12lu %12lu %12lu %12lu\n", i,
			(unsigned long) cpu.flags,
			(unsigned long) cpu.xcpu_total [i],
			(unsigned long) cpu.xcpu_user  [i],
			(unsigned long) cpu.xcpu_nice  [i],
			(unsigned long) cpu.xcpu_sys   [i],
			(unsigned long) cpu.xcpu_idle  [i]);

		s_total += fabs (((double) cpu.xcpu_total [i]) - b_total);
		s_user  += fabs (((double) cpu.xcpu_user  [i]) - b_user);
		s_nice  += fabs (((double) cpu.xcpu_nice  [i]) - b_nice);
		s_sys   += fabs (((double) cpu.xcpu_sys   [i]) - b_sys);
		s_idle  += fabs (((double) cpu.xcpu_idle  [i]) - b_idle);
	}

	printf ("%s\n\n\n", separator);

	printf ("%-26s %12s %12s %12s %12s %12s\n%s\n", "Percent:",
		"Total (%)", "User (%)", "Nice (%)", "Sys (%)",
		"Idle (%)", separator);

	printf ("CPU          (0x%08lx): %12.3f %12.3f %12.3f %12.3f %12.3f\n\n",
		(unsigned long) cpu.flags, (double) total * 100.0 / total,
		(double) user  * 100.0 / total,
		(double) nice  * 100.0 / total,
		(double) sys   * 100.0 / total,
		(double) idle  * 100.0 / total);

	for (i = 0; i < glibtop_global_server->ncpu; i++) {
		double p_total, p_user, p_nice, p_sys, p_idle;

		p_total = ((double) cpu.xcpu_total [i]) * 100.0 / total;
		p_user  = ((double) cpu.xcpu_user  [i]) * 100.0 / user;
		p_nice  = ((double) cpu.xcpu_nice  [i]) * 100.0 / nice;
		p_sys   = ((double) cpu.xcpu_sys   [i]) * 100.0 / sys;
		p_idle  = ((double) cpu.xcpu_idle  [i]) * 100.0 / idle;

		printf ("CPU %3d      (0x%08lx): %12.3f %12.3f %12.3f %12.3f %12.3f\n",
			i, (unsigned long) cpu.flags, p_total, p_user, p_nice,
			p_sys, p_idle);
	}

	printf ("%s\n%-26s %12.3f %12.3f %12.3f %12.3f %12.3f\n\n", separator,
		"Spin:", s_total * 100.0 / total, s_user * 100.0 / user,
		s_nice * 100.0 / nice, s_sys * 100.0 / sys, s_idle * 100.0 / idle);

	exit (0);
}
