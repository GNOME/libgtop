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

#include <glibtop.h>
#include <glibtop/signal.h>

const glibtop_signame glibtop_sys_siglist [] =
{  {  1, "SIGHUP",  "Hangup" },
   {  2, "SIGINT",  "Interrupt" },
   {  3, "SIGQUIT", "Quit" },
   {  4, "SIGILL",  "Illegal Instruction" },
   {  5, "SIGTRAP", "Trace/Breakpoint Trap" },
   {  6, "SIGABRT", "Abort" },
   {  7, "SIGEMT",  "Emulation Trap" },
   {  8, "SIGFPE",  "Arithmetic Exception" },
   {  9, "SIGKILL", "Killed" },
   { 10, "SIGBUS",  "Bus Error" },
   { 11, "SIGSEGV", "Segmentation Fault" },
   { 12, "SIGSYS",  "Bad System Call" },
   { 13, "SIGPIPE", "Broken Pipe" },
   { 14, "SIGALRM", "Alarm Clock" },
   { 15, "SIGTERM", "Terminated" },
   { 16, "SIGUSR1", "User Signal 1" },
   { 17, "SIGUSR2", "User Signal 2" },
   { 18, "SIGCHLD", "Child Status Changed" },
   { 19, "SIGPWR",  "Power-Fail/Restart" },
   { 20, "SIGWINCH","Window Size Change" },
   { 21, "SIGURG",  "Urgent Socket Condition" },
   { 22, "SIGPOLL", "Pollable Event" },
   { 23, "SIGSTOP", "Stoped (signal)" },
   { 24, "SIGTSTP", "Stopped (user)" },
   { 25, "SIGCONT", "Continued" },
   { 26, "SIGTTIN", "Stopped (tty input)" },
   { 27, "SIGTTOU", "Stopped (tty output)" },
   { 28, "SIGVTALRM","Virtual Timer Expired" },
   { 29, "SIGPROF", "Profiling Timer Expired" },
   { 30, "SIGXCPU", "Cpu Limit Exceeded" },
   { 31, "SIGXFSZ", "File Size Limit Exceeded" },
   { 32, "SIGWAITING","No runnable lwp" },
   { 33, "SIGLWP",  "Inter-lwp signal" },
   { 34, "SIGFREEZE","Checkpoint Freeze" },
   { 35, "SIGTHAW", "Checkpoint Thaw" },
   { 36, "SIGCANCEL","Thread Cancelation" },
   { 37, "SIGLOST", "Resource Lost" },
   { 38, "SIGRTMIN","First Realtime Signal" },
   { 39, "SIGRTMIN+1", "Second Realtime Signal" },
   { 40, "SIGRTMIN+2", "Third Realtime Signal" },
   { 41, "SIGRTMIN+3", "Fourth Realtime Signal" },
   { 42, "SIGRTMAX-3", "Fourth Last Realtime Signal" },
   { 43, "SIGRTMAX-2", "Third Last Realtime Signal" },
   { 44, "SIGRTMAX-1", "Second Last Realtime Signal" },
   { 45, "SIGRTMAX", "Last Realtime Signal" },
   {  0, NULL, NULL }
};

/* 
 * Now, just for the fun of it, let's try to be forward and backward
 * compatible. The above list is from Solaris 7. If later releases
 * include new signals, binary from the earlier release won't be
 * able to get the signal names, but it can get the correct numbers.
 * So...
 */

/*
#define MY_PRIVATE_COUNTOF(x) (sizeof(x)/sizeof(x[0]))

glibtop_signame *glibtop_sys_siglist;

static char *unknown = "Unknown";
static glibtop_signame rt_min =
    { 0, "SIGRTMIN", "First (highest-priority) realtime signal" };
static glibtop_signame rt_max =
    { 0, "SIGRTMIN", "Last (lowest-priority) realtime signal" };
static char *rt_desc = "Real time signal %d";

void
glibtop_init_signals(void)
{
    int rtmin, rtmax, sigs, to, i;
    char *bureq, p;

    rtmin = sysconf(_SC_SIGRT_MIN);
    rtmax = sysconf(_SC_SIGRT_MAX);
    sigs = MY_PRIVATE_COUNTOF(siglist);

    glibtop_sys_siglist = (glibtop_signame *)
       			  malloc(rtmax * sizeof(glibtop_signame));
    bureq = malloc((rtmax - rtmin - 1) * (strlen(rt_desc) + 4));
    to = (sigs <= rtmin) ? sigs : rtmin;
    memcpy(glibtop_sys_siglist, siglist, to * sizeof(glibtop_signame));
    for(i = sigs; i < rtmin; ++i)
    {
        glibtop_sys_siglist[i].number = i + 1;
	glibtop_sys_siglist[i].name = glibtop_sys_siglist[i].label = unknown;
    }
    glibtop_sys_siglist[rtmin - 1].number = rtmin;
    glibtop_sys_siglist[rtmin - 1].name = rt_min.name;
    glibtop_sys_siglist[rtmin - 1].label = rt_min.label;
    for(p = bureq, i = rtmin; i < rtmax; ++i)
    {
        glibtop_sys_siglist[i].number = i + 1;
	to = sprintf(p, "%d", i + 1) + 1;
	glibtop_sys_siglist[i].name = p;
	p += to;
	to = sprintf(p, rt_desc, i - rtmin + 2) + 1;
	glibtop_sys_siglist[i].label = p;
	p += to;
    }
    glibtop_sys_siglist[rtmax - 1].number = rtmax;
    glibtop_sys_siglist[rtmax - 1].name = rt_max.name;
    glibtop_sys_siglist[rtmax - 1].label = rt_max.label;
    glibtop_sys_siglist[rtmax].number = 0;
    glibtop_sys_siglist[rtmax].name = glibtop_sys_siglist[rtmax].label = NULL;
}
*/
