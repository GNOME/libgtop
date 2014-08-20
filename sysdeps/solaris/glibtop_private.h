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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef __GLIBTOP_PRIVATE_H__
#define __GLIBTOP_PRIVATE_H__

#include <glibtop.h>

#include <sys/param.h>
#include <kstat.h>
#include <fcntl.h>
#if defined(HAVE_PROCFS_H)
# include <procfs.h>
#elif defined(HAVE_SYS_PROCFS_H)
# include <sys/procfs.h>
#else
# error Cannot compile without <procfs.h> or <sys/procfs.h>
#endif

G_BEGIN_DECLS

#ifdef HAVE_PROCFS_H

/* Read /proc/<pid>/psinfo */
int glibtop_get_proc_data_psinfo_s(glibtop *, struct psinfo *, pid_t pid);

/* Read /proc/<pid>/status */
int glibtop_get_proc_status_s(glibtop *, struct pstatus *, pid_t);
#else
int glibtop_get_proc_data_psinfo_s(glibtop *, struct prpsinfo *, pid_t);
int glibtop_get_proc_status_s(glibtop *, struct prstatus *, pid_t);
#endif

/* Read /proc/<pid>/usage */
int glibtop_get_proc_data_usage_s(glibtop *, struct prusage *, pid_t);

#if LIBGTOP_VERSION_CODE >= 1001002
/* Read /proc/<pid>/cred */
int glibtop_get_proc_credentials_s(glibtop *, struct prcred *, gid_t *, pid_t);
#endif

/* Reread kstat chains */
void glibtop_get_kstats(glibtop *);

G_END_DECLS

#endif /* __GLIBTOP_PRIVATE_H__ */
