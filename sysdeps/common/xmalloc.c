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

#include <glibtop/xmalloc.h>

/* Wrappers to malloc, calloc, realloc ... */

void *
glibtop_malloc_r (glibtop *server, size_t size)
{
	void *buf = malloc (size);
	
	if (!buf)
		glibtop_error_io_r (server, "malloc %d bytes", size);
	
	return buf;
}

void *
glibtop_calloc_r (glibtop *server, size_t nmemb, size_t size)
{
	void *buf = calloc (nmemb, size);
	
	if (!buf)
		glibtop_error_io_r (server, "calloc %d blocks (%d bytes each)",
				    nmemb, size);
	
	return buf;
}

void *
glibtop_realloc_r (glibtop *server, void *ptr, size_t size)
{
	void *buf = realloc (ptr, size);
	
	if (!buf)
		glibtop_error_io_r (server, "realloc %d bytes", size);
	
	return buf;
}

char *
glibtop_strdup_r (glibtop *server, const char *string)
{
	return strcpy (glibtop_malloc_r (server, strlen (string) + 1), string);
}

void
glibtop_free_r (glibtop *server, const void *ptr)
{
	if (ptr) free ((void *) ptr);
}
