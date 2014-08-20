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
#include <glibtop/procaffinity.h>
#include <glibtop/error.h>

#include <glibtop_private.h>

#include <sched.h>


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
  cpu_set_t set;
  size_t i;
  GArray* cpus;

  memset(buf, 0, sizeof *buf);

  if (sched_getaffinity(pid, sizeof set, &set) == -1) {
    glibtop_error_r(server, "sched_getaffinity failed");
    return NULL;
  }

  cpus = g_array_new(FALSE, FALSE, sizeof(guint16));

  for (i = 0; i < MIN(CPU_SETSIZE, (size_t)(server->ncpu + 1)); i++) {
    if (CPU_ISSET(i, &set)) {
      guint16 n = i;
      g_array_append_val(cpus, n);
    }
  }

  buf->number = cpus->len;
  buf->all = (cpus->len == (size_t)(server->ncpu + 1));
  buf->flags = (1 << GLIBTOP_PROC_AFFINITY_NUMBER)
    | (1 << GLIBTOP_PROC_AFFINITY_ALL);

  return (guint16*) g_array_free(cpus, FALSE);
}

