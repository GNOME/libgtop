/* Copyright (C) 2007 Joe Marcus Clarke
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
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <config.h>
#include <glibtop/procwd.h>
#include <glibtop/error.h>

#include <glibtop_private.h>

#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/param.h>
#include <sys/user.h>
#include <string.h>

static const unsigned long _glibtop_sysdeps_proc_wd =
(1 << GLIBTOP_PROC_WD_EXE) |
(1 << GLIBTOP_PROC_WD_ROOT) |
(1 << GLIBTOP_PROC_WD_NUMBER);

void
_glibtop_init_proc_wd_s(glibtop *server)
{
	server->sysdeps.proc_wd = _glibtop_sysdeps_proc_wd;
}

#if (__FreeBSD_version >= 800000 && __FreeBSD_version < 800019) || _FreeBSD_version < 700104
static GPtrArray *
parse_output(const char *output, glibtop_proc_wd *buf)
{
	GPtrArray *dirs;
	char     **lines;
	gboolean   nextwd = FALSE;
	gboolean   nextrtd = FALSE;
	gboolean   havertd = FALSE;
	guint      i;
	guint      len;

	dirs = g_ptr_array_sized_new(1);

	lines = g_strsplit(output, "\n", 0);
	len = g_strv_length(lines);

	for (i = 0; i < len && lines[i]; i++) {
		if (strlen(lines[i]) < 2)
			continue;

		if (!strcmp(lines[i], "fcwd")) {
			nextwd = TRUE;
			continue;
		}

		if (!strcmp(lines[i], "frtd")) {
			nextrtd = TRUE;
			continue;
		}

		if (!g_str_has_prefix(lines[i], "n"))
			continue;

		if (nextwd) {
			g_ptr_array_add(dirs, g_strdup(lines[i] + 1));
			nextwd = FALSE;
		}

		if (nextrtd && !havertd) {
			g_strlcpy(buf->root, lines[i] + 1,
				  sizeof(buf->root));
			buf->flags |= (1 << GLIBTOP_PROC_WD_ROOT);
			nextrtd = FALSE;
			havertd = TRUE;
		}
	}

	g_strfreev(lines);

	return dirs;
}
#endif

char**
glibtop_get_proc_wd_s(glibtop *server, glibtop_proc_wd *buf, pid_t pid)
{
	char path[MAXPATHLEN];
#if __FreeBSD_version > 800018 || (__FreeBSD_version < 800000 && __FreeBSD_version >= 700104)
	struct kinfo_file *freep, *kif;
	GPtrArray *dirs;
	size_t len;
	int i;
	int name[4];
#else
	char *output;
#endif

	memset (buf, 0, sizeof (glibtop_proc_wd));

	g_snprintf(path, sizeof(path), "/proc/%u/file", pid);
	if (safe_readlink(path, buf->exe, sizeof(buf->exe)))
		buf->flags |= (1 << GLIBTOP_PROC_WD_EXE);

#if __FreeBSD_version > 800018 || (__FreeBSD_version < 800000 && __FreeBSD_version >= 700104)
	name[0] = CTL_KERN;
	name[1] = KERN_PROC;
	name[2] = KERN_PROC_FILEDESC;
	name[3] = pid;

	if (sysctl(name, 4, NULL, &len, NULL, 0) < 0)
		return NULL;
	freep = kif = g_malloc(len);
	if (sysctl(name, 4, kif, &len, NULL, 0) < 0) {
		g_free(freep);
		return NULL;
	}

	dirs = g_ptr_array_sized_new(1);

	for (i = 0; i < len / sizeof(*kif); i++, kif++) {
		switch (kif->kf_fd) {
			case KF_FD_TYPE_ROOT:
				g_strlcpy(buf->root, kif->kf_path,
					   sizeof(buf->root));
				buf->flags |= (1 << GLIBTOP_PROC_WD_ROOT);
				break;
			case KF_FD_TYPE_CWD:
				g_ptr_array_add(dirs, g_strdup (kif->kf_path));
				break;
		}
	}
	g_free(freep);

	buf->number = dirs->len;
	buf->flags |= (1 << GLIBTOP_PROC_WD_NUMBER);

	g_ptr_array_add(dirs, NULL);

	return (char **)g_ptr_array_free(dirs, FALSE);
#else
	output = execute_lsof(pid);
	if (output != NULL) {
		GPtrArray *dirs;

		dirs = parse_output(output, buf);
		g_free(output);

		buf->number = dirs->len;
		buf->flags |= (1 << GLIBTOP_PROC_WD_NUMBER);

		g_ptr_array_add(dirs, NULL);

		return (char **)g_ptr_array_free(dirs, FALSE);
	}
#endif

	return NULL;
}
