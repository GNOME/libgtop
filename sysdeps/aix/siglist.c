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

#include <config.h>
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
   { 16, "SIGURG",  "Urgent Socket Condition" },
   { 17, "SIGSTOP", "Stop (signal)" },
   { 18, "SIGSTP",  "Stop (user)" },
   { 19, "SIGCONT", "Continue" },
   { 20, "SIGCHLD", "Child Status Changed" },
   { 21, "SIGTTIN", "Stopped (tty input)" },
   { 22, "SIGTTOU", "Stopped (tty output)" },
   { 23, "SIGIO",   "I/O completed" },
   { 24, "SIGXCPU", "Cpu Limit Exceeded" },
   { 25, "SIGXFSZ", "File Size Limit Exceeded" },
   { 27, "SIGMSG",  "Message" },
   { 28, "SIGWINCH","Window Size Change" },
   { 29, "SIGPWR",  "Power-Fail/Restart" },
   { 30, "SIGUSR1", "User Signal 1" },
   { 31, "SIGUSR2", "User Signal 2" },
   { 32, "SIGPROF", "Profiling Timer Expired" },
   { 33, "SIGDANGER","System Crash Imminent" },
   { 34, "SIGVTALRM","Virtual Timer Expired" },
   { 35, "SIGMIGRATE","Migrate Process" },
   { 36, "SIGPRE",   "Programming Exception" },
   { 37, "SIGVIRT",  "AIX Virtual Time Alarm" },
   { 38, "SIGALRM1", "m:n Condition Variables" },
   { 39, "SIGWAITING","Scheduling" },
   { 59, "SIGCPUFAIL","Predictive De-configuration of Processors" },
   { 60, "SIGKAP",   "Keep Alive" },
   { 61, "SIGRETRACT","Monitor Mode Relinguish" },
   { 62, "SIGSOUND", "Sound Control Completed" },
   { 63, "SIGSAK",   "Secure Attention Key" },
   {  0, NULL, NULL }
};
