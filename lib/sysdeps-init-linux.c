/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

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
#include <glibtop/open.h>
#include <glibtop/backend.h>

#include <glibtop/signal.h>

const glibtop_signame glibtop_sys_siglist [] =
{ {  1, "SIGHUP",    N_("Hangup (POSIX)") },
  {  2, "SIGINT",    N_("Interrupt (ANSI)") },
  {  3, "SIGQUIT",   N_("Quit (POSIX)") },
  {  4, "SIGILL",    N_("Illegal instruction (ANSI)") },
  {  5, "SIGTRAP",   N_("Trace trap (POSIX)") },
  {  6, "SIGABRT",   N_("Abort (ANSI)") },
  {  7, "SIGBUS",    N_("BUS error (4.2 BSD)") },
  {  8, "SIGFPE",    N_("Floating-point exception (ANSI)") },
  {  9, "SIGKILL",   N_("Kill, unblockable (POSIX)") },
  { 10, "SIGUSR1",   N_("User-defined signal 1 (POSIX)") },
  { 11, "SIGSEGV",   N_("Segmentation violation (ANSI)") },
  { 12, "SIGUSR2",   N_("User-defined signal 2 (POSIX)") },
  { 13, "SIGPIPE",   N_("Broken pipe (POSIX)") },
  { 14, "SIGALRM",   N_("Alarm clock (POSIX)") },
  { 15, "SIGTERM",   N_("Termination (ANSI)") },
  { 16, "SIGSTKFLT", NULL },
  { 17, "SIGCHLD",   N_("Child status has changed (POSIX)") },
  { 18, "SIGCONT",   N_("Continue (POSIX)") },
  { 19, "SIGSTOP",   N_("Stop, unblockable (POSIX)") },
  { 20, "SIGTSTP",   N_("Keyboard stop (POSIX)") },
  { 21, "SIGTTIN",   N_("Background read from tty (POSIX)") },
  { 22, "SIGTTOU",   N_("Background write to tty (POSIX)") },
  { 23, "SIGURG",    N_("Urgent condition on socket (4.2 BSD)") },
  { 24, "SIGXCPU",   N_("CPU limit exceeded (4.2 BSD)") },
  { 25, "SIGXFSZ",   N_("File size limit exceeded (4.2 BSD)") },
  { 26, "SIGVTALRM", N_("Virtual alarm clock (4.2 BSD)") },
  { 27, "SIGPROF",   N_("Profiling alarm clock (4.2 BSD)") },
  { 28, "SIGWINCH",  N_("Window size change (4.3 BSD, Sun)") },
  { 29, "SIGIO",     N_("I/O now possible (4.2 BSD)") },
  { 30, "SIGPWR",    N_("Power failure restart (System V)") },
  { 31, "SIGUNUSED", NULL },
  {  0, NULL,        NULL },
};

void
_glibtop_open_sysdeps (glibtop *server, const char *program_name,
		       const unsigned long features, const unsigned flags)
{
    glibtop_open_backend_l (server, "glibtop-backend-common",
			    features, NULL);
    glibtop_open_backend_l (server, "glibtop-backend-sysdeps",
			    features, NULL);
}
