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

#ifndef __GLIBTOP_PROCTIME_H__
#define __GLIBTOP_PROCTIME_H__

#include <glibtop.h>
#include <glibtop/cpu.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_PROC_TIME_START_TIME	0
#define GLIBTOP_PROC_TIME_RTIME		1
#define GLIBTOP_PROC_TIME_UTIME		2
#define GLIBTOP_PROC_TIME_STIME		3
#define GLIBTOP_PROC_TIME_CUTIME	4
#define GLIBTOP_PROC_TIME_CSTIME	5
#define GLIBTOP_PROC_TIME_TIMEOUT	6
#define GLIBTOP_PROC_TIME_IT_REAL_VALUE	7
#define GLIBTOP_PROC_TIME_FREQUENCY	8
#define GLIBTOP_PROC_TIME_XCPU_UTIME	9
#define GLIBTOP_PROC_TIME_XCPU_STIME	10

#define GLIBTOP_MAX_PROC_TIME		11

typedef struct _glibtop_proc_time	glibtop_proc_time;

/* Time section */

/**
 * glibtop_proc_time:
 * @start_time: Start time of process in seconds since the epoch.
 * @rtime: Real time accumulated by process (should be @utime + @stime).
 * @utime: User-mode CPU time accumulated by process.
 * @stime: Kernel-mode CPU time accumulated by process.
 * @cutime: Cumulative utime of process and reaped children.
 * @cstime: Cumulative stime of process and reaped children.
 * @timeout: The time (in jiffies) of the process’s next timeout.
 * @it_real_value: The time (in jiffies) before the next <type>SIGALRM</type>
 * is sent to the process due to an interval timer.
 * @frequency: Tick frequency.
 * @xcpu_utime: SMP user-mode CPU time accumulated by process.
 * @xcpu_stime: SMP kernel-mode CPU time accumulated by process 
 *
 * Process time data filled by glibtop_get_proc_time().
 *
 * Under Linux the @start_time value may be wrong due to the information
 * available from the kernel.
 * 
 * The Linux kernel defines <type>INITIAL_JIFFIES</type> which implies a time
 * shift. Because <type>INITIAL_JIFFIES</type> is not user-space defined,
 * we cannot use it to compute an accurate @start_time. On Linux 2.6,
 * <type>INITIAL_JIFFIES</type> is 300 so @start_time is 
 * always 3s different from the real start time of the given process. You 
 * may also get shift results if your system clock is not synchronised 
 * with your hardware clock. See <command>man hwclock</command>.
 */
struct _glibtop_proc_time
{
    /*< private >*/
	guint64	flags;
	/*< public >*/
	guint64 start_time;
	guint64 rtime;
	guint64 utime;
	guint64 stime;
	guint64 cutime;
	guint64 cstime;
	guint64 timeout;
	guint64 it_real_value;
	guint64 frequency;
	guint64 xcpu_utime [GLIBTOP_NCPU];
	guint64 xcpu_stime [GLIBTOP_NCPU];
};


void glibtop_get_proc_time(glibtop_proc_time *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_TIME
#define glibtop_get_proc_time_r	glibtop_get_proc_time_p
#else
#define glibtop_get_proc_time_r	glibtop_get_proc_time_s
#endif

void glibtop_get_proc_time_l (glibtop *server, glibtop_proc_time *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_TIME
void _glibtop_init_proc_time_p (glibtop *server);
void glibtop_get_proc_time_p (glibtop *server, glibtop_proc_time *buf, pid_t pid);
#else
void _glibtop_init_proc_time_s (glibtop *server);
void glibtop_get_proc_time_s (glibtop *server, glibtop_proc_time *buf, pid_t pid);
#endif


G_END_DECLS

#endif
