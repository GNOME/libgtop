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

#include <glibtop.h>
#include <glibtop_private.h>

/* Read /proc/<pid>/psinfo. */

int
glibtop_get_proc_data_psinfo_s (glibtop *server, struct psinfo *psinfo, pid_t pid)
{
	int fd;
	char buffer [BUFSIZ];

	sprintf (buffer, "/proc/%d/psinfo", (int) pid);
	fd = open (buffer, O_RDONLY);
	if (fd < 0) {
		glibtop_warn_io_r (server, "open (%s)", buffer);
		return -1;
	}

	if (pread (fd, psinfo, sizeof (struct psinfo), 0) != sizeof (struct psinfo)) {
		close (fd);
		glibtop_warn_io_r (server, "read (%s)", buffer);
		return -1;
	}

	close (fd);
	return 0;
}

int
glibtop_get_proc_data_usage_s (glibtop *server, struct prusage *prusage, pid_t pid)
{
	int fd;
	char buffer [BUFSIZ];

	sprintf (buffer, "/proc/%d/usage", (int) pid);
	fd = open (buffer, O_RDONLY);
	if (fd < 0) {
		glibtop_warn_io_r (server, "open (%s)", buffer);
		return -1;
	}

	if (pread (fd, prusage, sizeof (struct prusage), 0) != sizeof (struct prusage)) {
		close (fd);
		glibtop_warn_io_r (server, "read (%s)", buffer);
		return -1;
	}

	close (fd);
	return 0;
}

int
glibtop_get_proc_credentials_s(glibtop *server, struct prcred *prcred, pid_t pid)
{
	int fd;
	char buffer[BUFSIZ];

	sprintf(buffer, "/proc/%d/prcred", (int)pid);
	if((fd = open(buffer, O_RDONLY)) < 0)
	{
	   	if(errno != EPERM)
		   	glibtop_warn_io_r(server, "open (%s)", buffer);
		return -1;
	}
	if(pread(fd, prcred, sizeof(struct prcred), 0) != sizeof(struct prcred))
	{
	   	close(fd);
		glibtop_warn_io_r(server, "read (%s)", buffer);
		return -1;
	}
	close(fd);
	return 0;
}
