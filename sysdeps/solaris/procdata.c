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
#include <glibtop_private.h>
#include <glibtop/procuid.h>
#include <glibtop/error.h>

#include <errno.h>

#include "safeio.h"

/*
 * The differences between old and new procfs API are:
 * - old has /proc/<pid> file and ioctl() is used to obtain the data.
 * - new has /proc/<pid>/* files and read() & friends are used to
 *   obtain the data.
 * - names of structures and members are different. It's mostly the
 *   prefix. Old uses `pr' and new uses `ps'.
 *
 * Since almost every line would be in #ifdef, I think it would
 * be more readable to just copy everything twice. This is not a big
 * file, after all.
 */

#ifdef HAVE_PROCFS_H

/* Read /proc/<pid>/psinfo. */

int
glibtop_get_proc_data_psinfo_s (glibtop *server, struct psinfo *psinfo, pid_t pid)
{
	int fd;
	char buffer [BUFSIZ];

	sprintf (buffer, "/proc/%d/psinfo", (int) pid);

	fd = s_open (buffer, O_RDONLY);

	if (fd < 0) {
		glibtop_warn_io_r (server, "open (%s)", buffer);
		return -1;
	}

	if (s_pread (fd, psinfo, sizeof (struct psinfo), 0)
	    != sizeof (struct psinfo))
	{
		s_close (fd);
		glibtop_warn_io_r (server, "pread (%s)", buffer);
		return -1;
	}

	s_close (fd);
	return 0;
}

int
glibtop_get_proc_data_usage_s (glibtop *server, struct prusage *prusage, pid_t pid)
{
	int fd;
	char buffer [BUFSIZ];

	sprintf (buffer, "/proc/%d/usage", (int) pid);

	fd = s_open (buffer, O_RDONLY);

	if (fd < 0) {
		glibtop_warn_io_r (server, "open (%s)", buffer);
		return -1;
	}

	if (s_pread (fd, prusage, sizeof (struct prusage), 0)
	    != sizeof (struct prusage))
	{
		s_close (fd);
		glibtop_warn_io_r (server, "pread (%s)", buffer);
		return -1;
	}

	s_close (fd);
	return 0;
}

#if LIBGTOP_VERSION_CODE >= 1001002
int
glibtop_get_proc_credentials_s(glibtop *server,
			       struct prcred *prcred,
			       gid_t *groups,
			       pid_t pid)
{
	int fd;
	size_t toread;
	char buffer[BUFSIZ];

	sprintf(buffer, "/proc/%d/cred", (int)pid);

	if((fd = s_open(buffer, O_RDONLY)) < 0)
	{
		if(errno != EPERM && errno != EACCES)
			glibtop_warn_io_r(server, "open (%s)", buffer);
		return -1;
	}

	if(s_pread(fd, prcred, sizeof(struct prcred), 0)
	   != sizeof(struct prcred))
	{
		s_close(fd);
		glibtop_warn_io_r(server, "pread (%s)", buffer);
		return -1;
	}

	if(prcred->pr_ngroups >= 0)
	{
		toread = MIN(prcred->pr_ngroups, GLIBTOP_MAX_GROUPS) * sizeof(gid_t);

		if((size_t) s_pread(fd,groups, toread,
				    G_STRUCT_OFFSET(struct prcred, pr_groups))
		   != toread)
			prcred->pr_ngroups = 0;
	}
	s_close(fd);
	return 0;
}
#endif /* LIBGTOP_VERSION_CODE >= 1001002 */

int
glibtop_get_proc_status_s(glibtop *server, struct pstatus *pstatus, pid_t pid)
{
	int fd;
	char buffer[BUFSIZ];

	sprintf(buffer, "/proc/%d/status", (int)pid);
	if((fd = s_open(buffer, O_RDONLY)) < 0)
	{
		if(errno != EPERM && errno != EACCES)
			glibtop_warn_io_r(server, "open (%s)", buffer);
		return -1;
	}
	if(s_pread(fd, pstatus, sizeof(struct pstatus), 0)
	   != sizeof(struct pstatus))
	{
		s_close(fd);
		glibtop_warn_io_r(server, "pread (%s)", buffer);
		return -1;
	}
	s_close(fd);
	return 0;
}

#else /* old API */

int
glibtop_get_proc_data_psinfo_s (glibtop *server,
				struct prpsinfo *psinfo,
				pid_t pid)
{
	int fd;
	char buffer [BUFSIZ];

	sprintf (buffer, "/proc/%d", (int) pid);
	fd = s_open (buffer, O_RDONLY);
	if (fd < 0) {
		if(errno != EPERM && errno != EACCES)
			glibtop_warn_io_r (server, "open (%s)", buffer);
		return -1;
	}

	if(ioctl(fd, PIOCPSINFO, psinfo) < 0)
	{
		s_close (fd);
		glibtop_warn_io_r (server, "ioctl(%s, PIOCPSINFO)", buffer);
		return -1;
	}

	s_close (fd);
	return 0;
}

int
glibtop_get_proc_data_usage_s (glibtop *server,
			       struct prusage *prusage,
			       pid_t pid)
{
	int fd;
	char buffer [BUFSIZ];

	sprintf (buffer, "/proc/%d", (int) pid);
	fd = s_open (buffer, O_RDONLY);
	if (fd < 0) {
		if(errno != EPERM && errno != EACCES)
			glibtop_warn_io_r (server, "open (%s)", buffer);
		return -1;
	}

	if(ioctl(fd, PIOCUSAGE, prusage) < 0)
	{
		s_close (fd);
		glibtop_warn_io_r (server, "ioctl(%s, PIOCUSAGE)", buffer);
		return -1;
	}

	s_close (fd);
	return 0;
}

int
glibtop_get_proc_credentials_s(glibtop *server,
			       struct prcred *prcred,
			       gid_t *groups,
			       pid_t pid)
{
	int fd;
	size_t toread;
	char buffer[BUFSIZ];

	sprintf(buffer, "/proc/%d", (int)pid);
	if((fd = s_open(buffer, O_RDONLY)) < 0)
	{
		if(errno != EPERM && errno != EACCES)
			glibtop_warn_io_r(server, "open (%s)", buffer);
		return -1;
	}
	if(ioctl(fd, PIOCCRED, prcred) < 0)
	{
		s_close(fd);
		glibtop_warn_io_r(server, "ioctl(%s, PIOCCRED)", buffer);
		return -1;
	}
	s_close(fd);
	return 0;
}

int
glibtop_get_proc_status_s(glibtop *server, struct prstatus *pstatus, pid_t pid)
{
	int fd;
	char buffer[BUFSIZ];

	sprintf(buffer, "/proc/%d", (int)pid);
	if((fd = s_open(buffer, O_RDONLY)) < 0)
	{
		if(errno != EPERM && errno != EACCES)
			glibtop_warn_io_r(server, "open (%s)", buffer);
		return -1;
	}
	if(ioctl(fd, PIOCSTATUS, pstatus) < 0)
	{
		s_close(fd);
		glibtop_warn_io_r(server, "ioctl(%s, PIOCSTATUS)", buffer);
		return -1;
	}
	s_close(fd);
	return 0;
}
#endif
