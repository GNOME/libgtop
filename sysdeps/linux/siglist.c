/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   LibGTop is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
   License for more details.

   You should have received a copy of the GNU Library General Public
   License along with LibGTop; see the file COPYING.LIB.  If not, write
   to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <glibtop.h>
#include <glibtop/signal.h>

const glibtop_signame glibtop_sys_siglist [] =
{ {  1, "SIGHUP",    NULL },	/* Hangup (POSIX).  */
  {  2, "SIGINT",    NULL },	/* Interrupt (ANSI).  */
  {  3, "SIGQUIT",   NULL },	/* Quit (POSIX).  */
  {  4, "SIGILL",    NULL },	/* Illegal instruction (ANSI).  */
  {  5, "SIGTRAP",   NULL },	/* Trace trap (POSIX).  */
  {  6, "SIGABRT",   NULL },	/* Abort (ANSI).  */
  {  7, "SIGBUS",    NULL },	/* BUS error (4.2 BSD).  */
  {  8, "SIGFPE",    NULL },	/* Floating-point exception (ANSI).  */
  {  9, "SIGKILL",   NULL },	/* Kill, unblockable (POSIX).  */
  { 10, "SIGUSR1",   NULL },	/* User-defined signal 1 (POSIX).  */
  { 11, "SIGSEGV",   NULL },	/* Segmentation violation (ANSI).  */
  { 12, "SIGUSR2",   NULL },	/* User-defined signal 2 (POSIX).  */
  { 13, "SIGPIPE",   NULL },	/* Broken pipe (POSIX).  */
  { 14, "SIGALRM",   NULL },	/* Alarm clock (POSIX).  */
  { 15, "SIGTERM",   NULL },	/* Termination (ANSI).  */
  { 16, "SIGSTKFLT", NULL },	/* ??? */
  { 17, "SIGCHLD",   NULL },	/* Child status has changed (POSIX).  */
  { 18, "SIGCONT",   NULL },	/* Continue (POSIX).  */
  { 19, "SIGSTOP",   NULL },	/* Stop, unblockable (POSIX).  */
  { 20, "SIGTSTP",   NULL },	/* Keyboard stop (POSIX).  */
  { 21, "SIGTTIN",   NULL },	/* Background read from tty (POSIX).  */
  { 22, "SIGTTOU",   NULL },	/* Background write to tty (POSIX).  */
  { 23, "SIGURG",    NULL },	/* Urgent condition on socket (4.2 BSD).  */
  { 24, "SIGXCPU",   NULL },	/* CPU limit exceeded (4.2 BSD).  */
  { 25, "SIGXFSZ",   NULL },	/* File size limit exceeded (4.2 BSD).  */
  { 26, "SIGVTALRM", NULL },	/* Virtual alarm clock (4.2 BSD).  */
  { 27, "SIGPROF",   NULL },	/* Profiling alarm clock (4.2 BSD).  */
  { 28, "SIGWINCH",  NULL },	/* Window size change (4.3 BSD, Sun).  */
  { 29, "SIGIO",     NULL },	/* I/O now possible (4.2 BSD).  */
  { 30, "SIGPWR",    NULL },	/* Power failure restart (System V).  */
  { 31, "SIGUNUSED", NULL },
  {  0, NULL,        NULL },
};
