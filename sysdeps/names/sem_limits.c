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

#include <glibtop/sem_limits.h>

const char *glibtop_names_sem_limits [GLIBTOP_MAX_SEM_LIMITS] =
{
	"semmap", "semmni", "semmns", "semmnu", "semmsl",
	"semopm", "semume", "semusz", "semvmx", "semaem"
};

const unsigned glibtop_types_sem_limits [GLIBTOP_MAX_SEM_LIMITS] =
{
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG, GLIBTOP_TYPE_ULONG,
	GLIBTOP_TYPE_ULONG
};

const char *glibtop_labels_sem_limits [GLIBTOP_MAX_SEM_LIMITS] =
{
	N_("Number of entries in semaphore map"),
	N_("Max number of arrays"),
	N_("Max semaphores system wide"),
	N_("Number of undo structures system wide"),
	N_("Max semaphores per array"),
	N_("Max ops per semop call"),
	N_("Max number of undo entries per process"),
	N_("sizeof struct sem_undo"),
	N_("Semaphore max value"),
	N_("Adjust on exit max value")
};

const char *glibtop_descriptions_sem_limits [GLIBTOP_MAX_SEM_LIMITS] =
{
	N_("Number of entries in semaphore map"),
	N_("Max number of arrays"),
	N_("Max semaphores system wide"),
	N_("Number of undo structures system wide"),
	N_("Max semaphores per array"),
	N_("Max ops per semop call"),
	N_("Max number of undo entries per process"),
	N_("sizeof struct sem_undo"),
	N_("Semaphore max value"),
	N_("Adjust on exit max value")
};
