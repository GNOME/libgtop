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
#include <glibtop/error.h>
#include <glibtop/sem_limits.h>

static const unsigned long _glibtop_sysdeps_sem_limits =
(1L << GLIBTOP_IPC_SEMMNI) + (1L << GLIBTOP_IPC_SEMMSL) +
(1L << GLIBTOP_IPC_SEMOPM) + (1L << GLIBTOP_IPC_SEMVMX) +
(1L << GLIBTOP_IPC_SEMAEM);

/* Init function. */

void
_glibtop_init_sem_limits_s (glibtop *server)
{
	server->sysdeps.sem_limits = _glibtop_sysdeps_sem_limits;
}

/* Provides information about sysv sem limits. */

void
glibtop_get_sem_limits_s (glibtop *server, glibtop_sem_limits *buf)
{
	int ret, value;

	glibtop_init_s (&server, 0, 0);

	memset (buf, 0, sizeof (glibtop_sem_limits));

	ret = table (TBL_SEMINFO, SEMINFO_MNI, (char *) &value, 1,
		     sizeof (value));

	if (ret != 1) return;

	buf->flags += (1L << GLIBTOP_IPC_SEMMNI);

	buf->semmni = value;


	ret = table (TBL_SEMINFO, SEMINFO_MSL, (char *) &value, 1,
		     sizeof (value));

	if (ret != 1) return;

	buf->flags += (1L << GLIBTOP_IPC_SEMMSL);

	buf->semmsl = value;


	ret = table (TBL_SEMINFO, SEMINFO_OPM, (char *) &value, 1,
		     sizeof (value));

	if (ret != 1) return;

	buf->flags += (1L << GLIBTOP_IPC_SEMOPM);

	buf->semopm = value;


	ret = table (TBL_SEMINFO, SEMINFO_UME, (char *) &value, 1,
		     sizeof (value));

	if (ret != 1) return;

	buf->flags += (1L << GLIBTOP_IPC_SEMUME);

	buf->semume = value;


	ret = table (TBL_SEMINFO, SEMINFO_VMX, (char *) &value, 1,
		     sizeof (value));

	if (ret != 1) return;

	buf->flags += (1L << GLIBTOP_IPC_SEMVMX);

	buf->semvmx = value;


	ret = table (TBL_SEMINFO, SEMINFO_AEM, (char *) &value, 1,
		     sizeof (value));

	if (ret != 1) return;

	buf->flags += (1L << GLIBTOP_IPC_SEMAEM);

	buf->semaem = value;
}
