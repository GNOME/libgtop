/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, March 1999.

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

#ifndef __GLIBTOP_PRIVATE_H__
#define __GLIBTOP_PRIVATE_H__

#include <glibtop.h>
#include <glibtop/error.h>

#include <sys/param.h>
#include <procfs.h>
#include <kstat.h>
#include <fcntl.h>

BEGIN_LIBGTOP_DECLS

/* Read /proc/<pid>/psinfo */
int glibtop_get_proc_data_psinfo_s (glibtop *server, struct psinfo *psinfo, pid_t pid);

/* Read /proc/<pid>/usage */
int glibtop_get_proc_data_usage_s (glibtop *server, struct prusage *prusage, pid_t pid);

/* Read /proc<pid>/cred */
int glibtop_get_proc_credentials_s(glibtop *, struct prcred *, pid_t);

/* Reread kstat chains */
void glibtop_get_kstats(glibtop *);

END_LIBGTOP_DECLS

#endif __GLIBTOP_PRIVATE_H__
