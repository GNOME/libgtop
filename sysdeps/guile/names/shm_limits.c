/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <glibtop.h>
#include <glibtop/shm_limits.h>

#include <guile/gh.h>

SCM
glibtop_guile_names_shm_limits (void)
{
	int i;
	SCM list;
	
	list = gh_list (SCM_UNDEFINED);
	
	for (i = 0; i < GLIBTOP_MAX_SHM_LIMITS; i++)
		list = gh_append2 (list, gh_list (gh_str02scm (glibtop_names_shm_limits [i]),
						  SCM_UNDEFINED));
	
	return list;
}

SCM
glibtop_guile_types_shm_limits (void)
{
	int i;
	SCM list;
	
	list = gh_list (SCM_UNDEFINED);
	
	for (i = 0; i < GLIBTOP_MAX_SHM_LIMITS; i++)
		list = gh_append2 (list, gh_list (gh_str02scm (gettext (glibtop_types_shm_limits [i])),
						  SCM_UNDEFINED));
	
	return list;
}

SCM
glibtop_guile_labels_shm_limits (void)
{
	int i;
	SCM list;
	
	list = gh_list (SCM_UNDEFINED);
	
	for (i = 0; i < GLIBTOP_MAX_SHM_LIMITS; i++)
		list = gh_append2 (list, gh_list (gh_str02scm (gettext (glibtop_labels_shm_limits [i])),
						  SCM_UNDEFINED));
	
	return list;
}

SCM
glibtop_guile_descriptions_shm_limits (void)
{
	int i;
	SCM list;
	
	list = gh_list (SCM_UNDEFINED);
	
	for (i = 0; i < GLIBTOP_MAX_SHM_LIMITS; i++)
		list = gh_append2 (list, gh_list (gh_str02scm (gettext (glibtop_descriptions_shm_limits [i])),
						  SCM_UNDEFINED));
	
	return list;
}
