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
#include <glibtop/global.h>
#include <glibtop/xmalloc.h>

const char *glibtop_error_strings[GLIBTOP_MAX_ERROR] = {
    N_("No error"),
    N_("Unknown error"),
    N_("Invalid argument"),
    N_("No such parameter"),
    N_("Attempted to modify a read-only value"),
    N_("Parameter size mismatch"),
    N_("Communication with LibGTop server failed"),
    N_("No such process")
};

char *
glibtop_get_error_string_l (glibtop *server, unsigned error_number)
{
    if (error_number >= GLIBTOP_MAX_ERROR)
	error_number = GLIBTOP_ERROR_UNKNOWN;

    return glibtop_strdup_r (server, _(glibtop_error_strings [error_number]));
}
