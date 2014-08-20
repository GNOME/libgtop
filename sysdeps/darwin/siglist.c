/*
   This file is part of LibGTop 2.0.

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
#include <unistd.h>

#include <glibtop.h>
#include <glibtop/signal.h>

const glibtop_signame glibtop_sys_siglist [] =
{ {  1, "SIGHUP",    NULL },	/* hangup */
  {  2, "SIGINT",    NULL },	/* interrupt */
  {  3, "SIGQUIT",   NULL },	/* quit */
  {  4, "SIGILL",    NULL },	/* illegal instruction */
  {  5, "SIGTRAP",   NULL },	/* trace trap */
  {  6, "SIGTRAP",   NULL },	/* abort */
  {  7, "SIGEMT",    NULL },	/* EMT instruction */
  {  8, "SIGFPE",    NULL },	/* floating point exception */
  {  9, "SIGKILL",   NULL },	/* kill */
  { 10, "SIGBUS",    NULL },	/* bus error */
  { 11, "SIGSEGV",   NULL },	/* segmentation violation */
  { 12, "SIGSYS",    NULL },	/* bad argument to system call */
  { 13, "SIGPIPE",   NULL },	/* write on a pipe with no one to read it */
  { 14, "SIGALRM",   NULL },	/* alarm clock */
  { 15, "SIGTERM",   NULL },	/* software termination signal from kill */
  { 16, "SIGURG",    NULL },	/* urgent condition on IO channel */
  { 17, "SIGSTOP",   NULL },	/* sendable stop signal not from tty */
  { 18, "SIGTSTP",   NULL },	/* stop signal from tty */
  { 19, "SIGCONT",   NULL },	/* continue a stopped process */
  { 20, "SIGCHLD",   NULL },	/* to parent on child stop or exit */
  { 21, "SIGTTIN",   NULL },	/* to readers pgrp upon background tty read */
  { 22, "SIGTTOU",   NULL },	/* like TTIN for output */
  { 23, "SIGIO",     NULL },	/* input/output possible signal */
  { 24, "SIGXCPU",   NULL },	/* exceeded CPU time limit */
  { 25, "SIGXFSZ",   NULL },	/* exceeded file size limit */
  { 26, "SIGVTALRM", NULL },	/* virtual time alarm */
  { 27, "SIGPROF",   NULL },	/* profiling time alarm */
  { 28, "SIGWINCH",  NULL },	/* window size changes */
  { 29, "SIGWINFO",  NULL },	/* information request */
  { 30, "SIGUSR1",   NULL },	/* user defined signal 1 */
  { 31, "SIGUSR2",   NULL },	/* user defined signal 2 */
  {  0, NULL,        NULL }
};

