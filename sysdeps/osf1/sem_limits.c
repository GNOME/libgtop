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
#include <glibtop/sem_limits.h>

static unsigned long _glibtop_sysdeps_sem_limits =
(1 << GLIBTOP_IPC_SEMMNI) + (1 << GLIBTOP_IPC_SEMMSL) +
(1 << GLIBTOP_IPC_SEMOPM) + (1 << GLIBTOP_IPC_SEMVMX) +
(1 << GLIBTOP_IPC_SEMAEM);

/* Provides information about sysv sem limits. */

void
glibtop_get_sem_limits__r (glibtop *server, glibtop_sem_limits *buf)
{
	int ret, value;

	glibtop_init ();

	memset (buf, 0, sizeof (glibtop_sem_limits));
	
	ret = table (TBL_SEMINFO, SEMINFO_MNI, (char *) &value, 1,
		     sizeof (value)); 

	if (ret != 1) return;

	buf->flags += (1 << GLIBTOP_IPC_SEMMNI);

	buf->semmni = value;
	
	
	ret = table (TBL_SEMINFO, SEMINFO_MSL, (char *) &value, 1,
		     sizeof (value)); 

	if (ret != 1) return;

	buf->flags += (1 << GLIBTOP_IPC_SEMMSL);

	buf->semmsl = value;
	
	
	ret = table (TBL_SEMINFO, SEMINFO_OPM, (char *) &value, 1,
		     sizeof (value)); 

	if (ret != 1) return;

	buf->flags += (1 << GLIBTOP_IPC_SEMOPM);

	buf->semopm = value;
	

	ret = table (TBL_SEMINFO, SEMINFO_UME, (char *) &value, 1,
		     sizeof (value)); 

	if (ret != 1) return;

	buf->flags += (1 << GLIBTOP_IPC_SEMUME);

	buf->semume = value;
	
	
	ret = table (TBL_SEMINFO, SEMINFO_VMX, (char *) &value, 1,
		     sizeof (value)); 

	if (ret != 1) return;

	buf->flags += (1 << GLIBTOP_IPC_SEMVMX);

	buf->semvmx = value;
	
	
	ret = table (TBL_SEMINFO, SEMINFO_AEM, (char *) &value, 1,
		     sizeof (value)); 

	if (ret != 1) return;

	buf->flags += (1 << GLIBTOP_IPC_SEMAEM);

	buf->semaem = value;
}
