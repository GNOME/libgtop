/* Copyright (C) 2007 Benoît Dejean
   This file is part of LibGTop 2.

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
#include <glibtop/procwd.h>
#include <glibtop/error.h>

#include <glibtop_private.h>

#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>


void
_glibtop_init_proc_wd_s(glibtop *server)
{
	server->sysdeps.proc_wd =
	(1 << GLIBTOP_PROC_WD_EXE) +
	(1 << GLIBTOP_PROC_WD_ROOT) +
	(1 << GLIBTOP_PROC_WD_NUMBER);

}

static gboolean is_in(GPtrArray *array, const char *str)
{
	guint i;

	for (i = 0; i != array->len; ++i) {
		if (strcmp(g_ptr_array_index(array, i), str) == 0)
			return TRUE;
	}

	return FALSE;
}


char**
glibtop_get_proc_wd_s(glibtop *server, glibtop_proc_wd *buf, pid_t pid)
{
	GPtrArray *dirs;
	char path[80];
	char dir[256];
	DIR *task;

	memset(buf, 0, sizeof(glibtop_proc_wd));

	g_snprintf(path, sizeof path, "/proc/%u/root", pid);
	if (safe_readlink(path, buf->root, sizeof buf->root))
		buf->flags |= (1 << GLIBTOP_PROC_WD_ROOT);

	g_snprintf(path, sizeof path, "/proc/%u/exe", pid);
	if (safe_readlink(path, buf->exe, sizeof buf->exe))
		buf->flags |= (1 << GLIBTOP_PROC_WD_EXE);

	dirs = g_ptr_array_sized_new(2);

	g_snprintf(path, sizeof path, "/proc/%u/cwd", pid);
	if (safe_readlink(path, dir, sizeof dir))
		g_ptr_array_add(dirs, g_strdup(dir));

	g_snprintf(path, sizeof path, "/proc/%u/task", pid);
	if ((task = opendir(path)) != NULL) {
		struct dirent *sub;
		while ((sub = readdir(task)) != NULL) {
			/* task dirs have numeric name */
			if (!isdigit(sub->d_name[0]))
				continue;
			g_snprintf(path, sizeof path, "/proc/%u/task/%s/cwd", pid, sub->d_name);
			if (safe_readlink(path, dir, sizeof dir) && !is_in(dirs, dir))
				g_ptr_array_add(dirs, g_strdup(dir));
		}
		closedir(task);
	}

	buf->number = dirs->len;
	buf->flags |= (1 << GLIBTOP_PROC_WD_NUMBER);

	g_ptr_array_add(dirs, NULL);

	return (char**) g_ptr_array_free(dirs, FALSE);
}

