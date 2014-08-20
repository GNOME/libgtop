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
{ {  1, "SIGHUP",	N_("Hangup") },
  {  2, "SIGINT",	N_("Interrupt") },
  {  3, "SIGQUIT",	N_("Quit") },
  {  4, "SIGILL",	N_("Illegal instruction") },
  {  5, "SIGTRAP",	N_("Trace trap") },
  {  6, "SIGABRT",	N_("Abort") },
  {  7, "SIGEMT",	N_("EMT error") },
  {  8, "SIGFPE",	N_("Floating-point exception") },
  {  9, "SIGKILL",	N_("Kill") },
  { 10, "SIGBUS",	N_("Bus error") },
  { 11, "SIGSEGV",	N_("Segmentation violation") },
  { 12, "SIGSYS",	N_("Bad argument to system call") },
  { 13, "SIGPIPE",	N_("Broken pipe") },
  { 14, "SIGALRM",	N_("Alarm clock") },
  { 15, "SIGTERM",	N_("Termination") },
  { 16, "SIGURG",	N_("Urgent condition on socket") },
  { 17, "SIGSTOP",	N_("Stop") },
  { 18, "SIGTSTP",	N_("Keyboard stop") },
  { 19, "SIGCONT",	N_("Continue") },
  { 20, "SIGCHLD",	N_("Child status has changed") },
  { 21, "SIGTTIN",	N_("Background read from tty") },
  { 22, "SIGTTOU",	N_("Background write to tty") },
  { 23, "SIGIO",	N_("I/O now possible") },
  { 24, "SIGXCPU",	N_("CPU limit exceeded") },
  { 25, "SIGXFSZ",	N_("File size limit exceeded") },
  { 26, "SIGVTALRM",	N_("Virtual alarm clock") },
  { 27, "SIGPROF",	N_("Profiling alarm clock") },
  { 28, "SIGWINCH",	N_("Window size change") },
  { 29, "SIGINFO",	N_("Information request") },
  { 30, "SIGUSR1",	N_("User defined signal 1") },
  { 31, "SIGUSR2",	N_("User defined signal 2") },
  {  0, NULL,		NULL },
};
