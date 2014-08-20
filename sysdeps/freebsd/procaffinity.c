/* Copyright (C) 2007 Joe Marcus Clarke <marcus@FreeBSD.org>
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
#include <glibtop/procaffinity.h>
#include <glibtop/error.h>

#include <glibtop_private.h>

#include <sys/param.h>
#if __FreeBSD_version > 800024
#include <sys/cpuset.h>
#endif

void
_glibtop_init_proc_affinity_s(glibtop *server)
{
  server->sysdeps.proc_affinity =
    (1 << GLIBTOP_PROC_AFFINITY_NUMBER) |
    (1 << GLIBTOP_PROC_AFFINITY_ALL);

}


guint16 *
glibtop_get_proc_affinity_s(glibtop *server, glibtop_proc_affinity *buf, pid_t pid)
{
#if __FreeBSD_version > 800024
  id_t id;
  cpulevel_t level;
  cpuwhich_t which;
  cpuset_t mask;
  size_t i;
  GArray* cpus;

  memset(buf, 0, sizeof *buf);

  which = CPU_WHICH_PID;
  level = CPU_LEVEL_WHICH;
  id = pid;

  if (cpuset_getaffinity(level, which, id, sizeof(mask), &mask) != 0) {
    glibtop_error_r(server, "cpuset_getaffinity failed");
    return NULL;
  }

  cpus = g_array_new(FALSE, FALSE, sizeof(guint16));

  for (i = 0; i < MIN(CPU_SETSIZE, (size_t)(server->ncpu + 1)); i++) {
    if (CPU_ISSET(i, &mask)) {
      guint16 n = i;
      g_array_append_val(cpus, n);
    }
  }

  buf->number = cpus->len;
  buf->all = (cpus->len == (size_t)(server->ncpu + 1));
  buf->flags = (1 << GLIBTOP_PROC_AFFINITY_NUMBER)
    | (1 << GLIBTOP_PROC_AFFINITY_ALL);

  return (guint16*) g_array_free(cpus, FALSE);
#else
  memset(buf, 0, sizeof *buf);

  return NULL;
#endif
}

