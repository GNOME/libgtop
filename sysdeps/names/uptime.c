/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

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

#include <glibtop/uptime.h>

const char *glibtop_names_uptime[GLIBTOP_MAX_UPTIME] =
{
    "uptime", "idletime"
};

const unsigned glibtop_types_uptime[GLIBTOP_MAX_UPTIME] =
{
    GLIBTOP_TYPE_DOUBLE, GLIBTOP_TYPE_DOUBLE
};

const char *glibtop_labels_uptime[GLIBTOP_MAX_UPTIME] =
{
    N_ ("Uptime"),
    N_ ("Idletime")
};

const char *glibtop_descriptions_uptime[GLIBTOP_MAX_UPTIME] =
{
    N_ ("Time in seconds since system boot"),
    N_ ("Time in seconds the system spent in the idle task since system boot")
};
