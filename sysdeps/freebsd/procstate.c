/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Joshua Sled <jsled@xcf.berkeley.edu, July 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <config.h>
#include <glibtop/procstate.h>

static const unsigned long _glibtop_sysdeps_proc_state =
(1 << GLIBTOP_PROC_STATE_CMD) +
(1 << GLIBTOP_PROC_STATE_STATE) +
(1 << GLIBTOP_PROC_STATE_UID) +
(1 << GLIBTOP_PROC_STATE_GID);

/* Provides detailed information about a process. */

void
glibtop_get_proc_state_s (glibtop *server,
			  glibtop_proc_state *buf,
			  pid_t pid)
{
  glibtop_init_r (&server, 0, 0);

  memset (buf, 0, sizeof (glibtop_proc_state));

  if (pid == INVALID_PID) {
    /* Client is only interested in the flags. */
    buf->flags = _glibtop_sysdeps_proc_state;
    return;
  }



  /* Set the flags for the data we're about to return*/
  buf->flags = _glibtop_sysdeps_proc_state;
}
