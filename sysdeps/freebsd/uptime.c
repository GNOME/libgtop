/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

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

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/uptime.h>

#include <glibtop/cpu.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_uptime =
(1 << GLIBTOP_UPTIME_UPTIME) + (1 << GLIBTOP_UPTIME_IDLETIME);

static const unsigned long _required_cpu_flags =
(1 << GLIBTOP_CPU_TOTAL) + (1 << GLIBTOP_CPU_IDLE) +
(1 << GLIBTOP_CPU_FREQUENCY);

/* Init function. */

void
glibtop_init_uptime_p (glibtop *server)
{
	server->sysdeps.uptime = _glibtop_sysdeps_uptime;
}

/* Provides uptime and idle time. */

void
glibtop_get_uptime_p (glibtop *server, glibtop_uptime *buf)
{
	glibtop_cpu cpu;

	glibtop_init_p (server, GLIBTOP_SYSDEPS_UPTIME, 0);
	
	memset (buf, 0, sizeof (glibtop_uptime));

	/* We simply calculate it from the CPU usage. */

	glibtop_get_cpu_p (server, &cpu);

	/* Make sure all required fields are present. */

	if ((cpu.flags & _required_cpu_flags) != _required_cpu_flags)
		return;

	/* Calculate values. */

	buf->uptime = (double) cpu.total / (double) cpu.frequency;
	buf->idletime = (double) cpu.idle / (double) cpu.frequency;

	buf->flags = _glibtop_sysdeps_uptime;
}
