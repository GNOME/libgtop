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

int
glibtop_get_proc_data_stat_s (glibtop *server, libgtop_stat_t *stat)
{
    int name [2] = { CTL_LIBGTOP, LIBGTOP_STAT };
    size_t size = sizeof (libgtop_stat_t);

    if (sysctl (name, 2, stat, &size, NULL, 0)) {
	glibtop_warn_io_r (server, "sysctl (libgtop/stat)");
	return -1;
    }

    return 0;
}

int
glibtop_get_proc_data_mem_s (glibtop *server, libgtop_mem_t *mem)
{
    int name [2] = { CTL_LIBGTOP, LIBGTOP_MEM };
    size_t size = sizeof (libgtop_mem_t);

    if (sysctl (name, 2, mem, &size, NULL, 0)) {
	glibtop_warn_io_r (server, "sysctl (libgtop/mem)");
	return -1;
    }

    return 0;
}

int
glibtop_get_proc_data_swap_s (glibtop *server, libgtop_swap_t *swap)
{
    int name [2] = { CTL_LIBGTOP, LIBGTOP_SWAP };
    size_t size = sizeof (libgtop_swap_t);

    if (sysctl (name, 2, swap, &size, NULL, 0)) {
	glibtop_warn_io_r (server, "sysctl (libgtop/swap)");
	return -1;
    }

    return 0;
}
