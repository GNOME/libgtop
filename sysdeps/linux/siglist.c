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


#include <glibtop/signal.h>

#include <signal.h>


#if 0 /* comment */
perl -nle 'print "{$1,\t\"$1\",\t\"$2\"}," if m|^#define\s*(SIG[A-Z0-9]+).*?/\*\s*(.*?)\s*\*/|'
	< /usr/include/bits/signum.h
#endif


const glibtop_signame glibtop_sys_siglist [] =
{
	{SIGHUP,	"SIGHUP",       "Hangup (POSIX)."},
	{SIGINT,	"SIGINT",       "Interrupt (ANSI)."},
	{SIGQUIT,       "SIGQUIT",      "Quit (POSIX)."},
	{SIGILL,	"SIGILL",       "Illegal instruction (ANSI)."},
	{SIGTRAP,       "SIGTRAP",      "Trace trap (POSIX)."},
	{SIGABRT,       "SIGABRT",      "Abort (ANSI)."},
	{SIGIOT,	"SIGIOT",       "IOT trap (4.2 BSD)."},
	{SIGBUS,	"SIGBUS",       "BUS error (4.2 BSD)."},
	{SIGFPE,	"SIGFPE",       "Floating-point exception (ANSI)."},
	{SIGKILL,       "SIGKILL",      "Kill, unblockable (POSIX)."},
	{SIGUSR1,	"SIGUSR1",      "User-defined signal 1 (POSIX)."},
	{SIGSEGV,	"SIGSEGV",      "Segmentation violation (ANSI)."},
	{SIGUSR2,	"SIGUSR2",      "User-defined signal 2 (POSIX)."},
	{SIGPIPE,       "SIGPIPE",      "Broken pipe (POSIX)."},
	{SIGALRM,       "SIGALRM",      "Alarm clock (POSIX)."},
	{SIGTERM,       "SIGTERM",      "Termination (ANSI)."},
	{SIGSTKFLT,     "SIGSTKFLT",    "Stack fault."},
	{SIGCLD,	"SIGCLD",       "Same as SIGCHLD (System V)."},
	{SIGCHLD,       "SIGCHLD",      "Child status has changed (POSIX)."},
	{SIGCONT,       "SIGCONT",      "Continue (POSIX)."},
	{SIGSTOP,       "SIGSTOP",      "Stop, unblockable (POSIX)."},
	{SIGTSTP,       "SIGTSTP",      "Keyboard stop (POSIX)."},
	{SIGTTIN,       "SIGTTIN",      "Background read from tty (POSIX)."},
	{SIGTTOU,       "SIGTTOU",      "Background write to tty (POSIX)."},
	{SIGURG,	"SIGURG",       "Urgent condition on socket (4.2 BSD)."},
	{SIGXCPU,       "SIGXCPU",      "CPU limit exceeded (4.2 BSD)."},
	{SIGXFSZ,       "SIGXFSZ",      "File size limit exceeded (4.2 BSD)."},
	{SIGVTALRM,     "SIGVTALRM",    "Virtual alarm clock (4.2 BSD)."},
	{SIGPROF,       "SIGPROF",      "Profiling alarm clock (4.2 BSD)."},
	{SIGWINCH,      "SIGWINCH",     "Window size change (4.3 BSD, Sun)."},
	{SIGPOLL,       "SIGPOLL",      "Pollable event occurred (System V)."},
	{SIGIO,		"SIGIO",	"I/O now possible (4.2 BSD)."},
	{SIGPWR,	"SIGPWR",       "Power failure restart (System V)."},
	{SIGSYS,	"SIGSYS",       "Bad system call."},
	{SIGUNUSED,	"SIGUNUSED",	""},
	{0,		NULL,		NULL}
};
