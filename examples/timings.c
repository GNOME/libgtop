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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <locale.h>
#include <stdio.h>
#include <libintl.h>

#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/close.h>

#include <glibtop/parameter.h>

#include <glibtop/union.h>
#include <glibtop/sysdeps.h>

#include <sys/times.h>
#include <sys/resource.h>

#ifndef PROFILE_COUNT
#define PROFILE_COUNT			100000L
#endif

#ifndef PROFILE_COUNT_EXPENSIVE
#define PROFILE_COUNT_EXPENSIVE		(PROFILE_COUNT / 10)
#endif

#define ELAPSED_UTIME ((unsigned long) elapsed_utime.tv_sec * 1000000 +	(unsigned long) elapsed_utime.tv_usec)
#define ELAPSED_STIME ((unsigned long) elapsed_stime.tv_sec * 1000000 + (unsigned long) elapsed_stime.tv_usec)

#define libgtop_timeradd(tvp, uvp, vvp)						\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;	\
		if ((vvp)->tv_usec >= 1000000) {			\
			(vvp)->tv_sec++;				\
			(vvp)->tv_usec -= 1000000;			\
		}							\
	} while (0)
#define libgtop_timersub(tvp, uvp, vvp)						\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;	\
		if ((vvp)->tv_usec < 0) {				\
			(vvp)->tv_sec--;				\
			(vvp)->tv_usec += 1000000;			\
		}							\
	} while (0)


static void display_self_times(void)
{
	printf("-- Overview using `ps` --------------------"
	       "---------------\n");
	printf("-- this client is %8u ----------------"
	       "---------------\n", (unsigned)getpid());
	printf("-- this server is %8u ----------------"
	       "---------------\n", (unsigned)glibtop_global_server->pid);
	system("ps xjf | egrep 'timings|libgtop_server'");
	printf("-------------------------------------------"
	       "---------------\n");
}


int
main (int argc, char *argv [])
{
	glibtop_union data;
	unsigned c;
	struct rusage total_start, total_end;
	struct rusage rusage_start, rusage_end;
	struct timeval elapsed_utime, elapsed_stime;
	pid_t pid;

	pid = getpid ();

	setlocale (LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, GTOPLOCALEDIR);
	textdomain (GETTEXT_PACKAGE);


	glibtop_init_r (&glibtop_global_server, 0, 0);

	display_self_times();

	printf ("%-12s (%-10s): %7s - %9s - %9s\n",
		"Feature", "Flags", "Count", "utime", "stime");
	printf ("-------------------------------------------"
		"---------------\n");

	getrusage (RUSAGE_SELF, &total_start);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_cpu (&data.cpu);

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("CPU          (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.cpu.flags, PROFILE_COUNT,
		(long double) ELAPSED_UTIME / PROFILE_COUNT,
		(long double) ELAPSED_STIME / PROFILE_COUNT);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT_EXPENSIVE; c++)
		glibtop_get_mem (&data.mem);

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Memory       (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.mem.flags, PROFILE_COUNT_EXPENSIVE,
		(long double) ELAPSED_UTIME / PROFILE_COUNT_EXPENSIVE,
		(long double) ELAPSED_STIME / PROFILE_COUNT_EXPENSIVE);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT_EXPENSIVE; c++)
		glibtop_get_swap (&data.swap);

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Swap         (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.swap.flags, PROFILE_COUNT_EXPENSIVE,
		(long double) ELAPSED_UTIME / PROFILE_COUNT_EXPENSIVE,
		(long double) ELAPSED_STIME / PROFILE_COUNT_EXPENSIVE);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_uptime (&data.uptime);

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Uptime       (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.uptime.flags, PROFILE_COUNT,
		(long double) ELAPSED_UTIME / PROFILE_COUNT,
		(long double) ELAPSED_STIME / PROFILE_COUNT);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_loadavg (&data.loadavg);

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Loadavg      (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.loadavg.flags, PROFILE_COUNT,
		(long double) ELAPSED_UTIME / PROFILE_COUNT,
		(long double) ELAPSED_STIME / PROFILE_COUNT);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT_EXPENSIVE; c++) {
		pid_t* ptr = glibtop_get_proclist (&data.proclist, 0, 0);
		g_free (ptr);
	}

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Proclist     (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.proclist.flags,
		PROFILE_COUNT_EXPENSIVE,
		(long double) ELAPSED_UTIME / PROFILE_COUNT_EXPENSIVE,
		(long double) ELAPSED_STIME / PROFILE_COUNT_EXPENSIVE);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT_EXPENSIVE; c++) {
		char** ptr = glibtop_get_netlist (&data.netlist);
		g_strfreev (ptr);
	}

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Netlist      (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.proclist.flags,
		PROFILE_COUNT_EXPENSIVE,
		(long double) ELAPSED_UTIME / PROFILE_COUNT_EXPENSIVE,
		(long double) ELAPSED_STIME / PROFILE_COUNT_EXPENSIVE);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_state (&data.proc_state, pid);

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Proc_State   (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.proc_state.flags, PROFILE_COUNT,
		(long double) ELAPSED_UTIME / PROFILE_COUNT,
		(long double) ELAPSED_STIME / PROFILE_COUNT);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_uid (&data.proc_uid, pid);

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Proc_Uid     (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.proc_uid.flags, PROFILE_COUNT,
		(long double) ELAPSED_UTIME / PROFILE_COUNT,
		(long double) ELAPSED_STIME / PROFILE_COUNT);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_mem (&data.proc_mem, pid);

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Proc_Mem     (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.proc_mem.flags, PROFILE_COUNT,
		(long double) ELAPSED_UTIME / PROFILE_COUNT,
		(long double) ELAPSED_STIME / PROFILE_COUNT);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT_EXPENSIVE; c++) {
		glibtop_map_entry* entries;
		entries = glibtop_get_proc_map (&data.proc_map, pid);
		g_free (entries);
	}

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Proc_Map     (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.proc_map.flags, PROFILE_COUNT_EXPENSIVE,
		(long double) ELAPSED_UTIME / PROFILE_COUNT_EXPENSIVE,
		(long double) ELAPSED_STIME / PROFILE_COUNT_EXPENSIVE);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_segment (&data.proc_segment, pid);

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Proc_Segment (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.proc_segment.flags, PROFILE_COUNT,
		(long double) ELAPSED_UTIME / PROFILE_COUNT,
		(long double) ELAPSED_STIME / PROFILE_COUNT);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT; c++) {
		char** argv;
		argv = glibtop_get_proc_argv (&data.proc_args, pid, 0);
		g_strfreev(argv);
	}

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Proc_Args    (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.proc_args.flags, PROFILE_COUNT,
		(long double) ELAPSED_UTIME / PROFILE_COUNT,
		(long double) ELAPSED_STIME / PROFILE_COUNT);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_time (&data.proc_time, pid);

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Proc_Time    (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.proc_time.flags, PROFILE_COUNT,
		(long double) ELAPSED_UTIME / PROFILE_COUNT,
		(long double) ELAPSED_STIME / PROFILE_COUNT);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_signal (&data.proc_signal, pid);

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Proc_Signal  (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.proc_signal.flags, PROFILE_COUNT,
		(long double) ELAPSED_UTIME / PROFILE_COUNT,
		(long double) ELAPSED_STIME / PROFILE_COUNT);

	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_kernel (&data.proc_kernel, pid);

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Proc_Kernel  (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.proc_kernel.flags, PROFILE_COUNT,
		(long double) ELAPSED_UTIME / PROFILE_COUNT,
		(long double) ELAPSED_STIME / PROFILE_COUNT);


	getrusage (RUSAGE_SELF, &rusage_start);

	for (c = 0; c < PROFILE_COUNT; c++)
		glibtop_get_proc_io (&data.proc_io, pid);

	getrusage (RUSAGE_SELF, &rusage_end);

	libgtop_timersub (&rusage_end.ru_utime, &rusage_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&rusage_end.ru_stime, &rusage_start.ru_stime,
			  &elapsed_stime);

	printf ("Proc_Io  (0x%08lx): %7lu - %9.2Lf - %9.2Lf\n",
		(unsigned long) data.proc_io.flags, PROFILE_COUNT,
		(long double) ELAPSED_UTIME / PROFILE_COUNT,
		(long double) ELAPSED_STIME / PROFILE_COUNT);


	getrusage (RUSAGE_SELF, &total_end);

	libgtop_timersub (&total_end.ru_utime, &total_start.ru_utime,
			  &elapsed_utime);

	libgtop_timersub (&total_end.ru_stime, &total_start.ru_stime,
			  &elapsed_stime);

	printf ("-------------------------------------------"
		"---------------\n");

	printf ("%-36s %9lu - %9lu\n\n", "TOTAL",
		ELAPSED_UTIME, ELAPSED_STIME);

	printf ("All timings are in clock ticks "
		"(1000000 ticks per second).\n\n");

	display_self_times();

	glibtop_close ();

	exit (0);
}
