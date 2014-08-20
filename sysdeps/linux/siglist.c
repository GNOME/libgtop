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
#include <glibtop/signal.h>

#include <signal.h>


#if 0 /* comment */
perl -nle 'print "{$1,\t\"$1\",\t\"$2\"}," if m|^#define\s*(SIG[A-Z0-9]+).*?/\*\s*(.*?)\s*\*/|'
	< /usr/include/bits/signum.h
#endif


const glibtop_signame glibtop_sys_siglist [] =
{
#ifdef SIGHUP
	{SIGHUP,	"SIGHUP",       "Hangup (POSIX)."},
#endif
#ifdef SIGINT
	{SIGINT,	"SIGINT",       "Interrupt (ANSI)."},
#endif
#ifdef SIGQUIT
	{SIGQUIT,       "SIGQUIT",      "Quit (POSIX)."},
#endif
#ifdef SIGILL
	{SIGILL,	"SIGILL",       "Illegal instruction (ANSI)."},
#endif
#ifdef SIGTRAP
	{SIGTRAP,       "SIGTRAP",      "Trace trap (POSIX)."},
#endif
#ifdef SIGABRT
	{SIGABRT,       "SIGABRT",      "Abort (ANSI)."},
#endif
#ifdef SIGIOT
	{SIGIOT,	"SIGIOT",       "IOT trap (4.2 BSD)."},
#endif
#ifdef SIGBUS
	{SIGBUS,	"SIGBUS",       "BUS error (4.2 BSD)."},
#endif
#ifdef SIGFPE
	{SIGFPE,	"SIGFPE",       "Floating-point exception (ANSI)."},
#endif
#ifdef SIGKILL
	{SIGKILL,       "SIGKILL",      "Kill, unblockable (POSIX)."},
#endif
#ifdef SIGUSR1
	{SIGUSR1,	"SIGUSR1",      "User-defined signal 1 (POSIX)."},
#endif
#ifdef SIGSEGV
	{SIGSEGV,	"SIGSEGV",      "Segmentation violation (ANSI)."},
#endif
#ifdef SIGUSR2
	{SIGUSR2,	"SIGUSR2",      "User-defined signal 2 (POSIX)."},
#endif
#ifdef SIGPIPE
	{SIGPIPE,       "SIGPIPE",      "Broken pipe (POSIX)."},
#endif
#ifdef SIGALRM
	{SIGALRM,       "SIGALRM",      "Alarm clock (POSIX)."},
#endif
#ifdef SIGTERM
	{SIGTERM,       "SIGTERM",      "Termination (ANSI)."},
#endif
#ifdef SIGSTKFLT
	{SIGSTKFLT,     "SIGSTKFLT",    "Stack fault."},
#endif
#ifdef SIGCLD
	{SIGCLD,	"SIGCLD",       "Same as SIGCHLD (System V)."},
#endif
#ifdef SIGCHLD
	{SIGCHLD,       "SIGCHLD",      "Child status has changed (POSIX)."},
#endif
#ifdef SIGCONT
	{SIGCONT,       "SIGCONT",      "Continue (POSIX)."},
#endif
#ifdef SIGSTOP
	{SIGSTOP,       "SIGSTOP",      "Stop, unblockable (POSIX)."},
#endif
#ifdef SIGTSTP
	{SIGTSTP,       "SIGTSTP",      "Keyboard stop (POSIX)."},
#endif
#ifdef SIGTTIN
	{SIGTTIN,       "SIGTTIN",      "Background read from tty (POSIX)."},
#endif
#ifdef SIGTTOU
	{SIGTTOU,       "SIGTTOU",      "Background write to tty (POSIX)."},
#endif
#ifdef SIGURG
	{SIGURG,	"SIGURG",       "Urgent condition on socket (4.2 BSD)."},
#endif
#ifdef SIGXCPU
	{SIGXCPU,       "SIGXCPU",      "CPU limit exceeded (4.2 BSD)."},
#endif
#ifdef SIGXFSZ
	{SIGXFSZ,       "SIGXFSZ",      "File size limit exceeded (4.2 BSD)."},
#endif
#ifdef SIGVTALRM
	{SIGVTALRM,     "SIGVTALRM",    "Virtual alarm clock (4.2 BSD)."},
#endif
#ifdef SIGPROF
	{SIGPROF,       "SIGPROF",      "Profiling alarm clock (4.2 BSD)."},
#endif
#ifdef SIGWINCH
	{SIGWINCH,      "SIGWINCH",     "Window size change (4.3 BSD, Sun)."},
#endif
#ifdef SIGPOLL
	{SIGPOLL,       "SIGPOLL",      "Pollable event occurred (System V)."},
#endif
#ifdef SIGIO
	{SIGIO,		"SIGIO",	"I/O now possible (4.2 BSD)."},
#endif
#ifdef SIGPWR
	{SIGPWR,	"SIGPWR",       "Power failure restart (System V)."},
#endif
#ifdef SIGSYS
	{SIGSYS,	"SIGSYS",       "Bad system call."},
#endif
#ifdef SIGUNUSED
	{SIGUNUSED,	"SIGUNUSED",	""},
#endif
	{0,		NULL,		NULL}
};
