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

#include <config.h>
#include <glibtop/error.h>
#include <glibtop/procsegment.h>

static const unsigned long _glibtop_sysdeps_proc_segment =
(1 << GLIBTOP_PROC_SEGMENT_TEXT_RSS) +
(1 << GLIBTOP_PROC_SEGMENT_SHLIB_RSS) +
(1 << GLIBTOP_PROC_SEGMENT_DATA_RSS) +
(1 << GLIBTOP_PROC_SEGMENT_DIRTY_SIZE) +
(1 << GLIBTOP_PROC_SEGMENT_START_CODE) +
(1 << GLIBTOP_PROC_SEGMENT_END_CODE) +
(1 << GLIBTOP_PROC_SEGMENT_START_STACK);

#ifndef LOG1024
#define LOG1024		10
#endif

/* these are for getting the memory statistics */
static int pageshift;		/* log base 2 of the pagesize */

/* define pagetok in terms of pageshift */
#define pagetok(size) ((size) << pageshift)

/* Init function. */

void
glibtop_init_proc_segment_s (glibtop *server)
{
	register int pagesize;

	server->sysdeps.proc_segment = _glibtop_sysdeps_proc_segment;

	/* get the page size with "getpagesize" and calculate pageshift
	 * from it */
	pagesize = getpagesize ();
	pageshift = 0;
	while (pagesize > 1) {
		pageshift++;
		pagesize >>= 1;
	}
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_segment_s (glibtop *server, glibtop_proc_segment *buf,
			    pid_t pid)
{
	char input [BUFSIZ], *tmp;
	int nread;
	FILE *f;
	
	glibtop_init_s (&server, GLIBTOP_SYSDEPS_PROC_SEGMENT, 0);

	memset (buf, 0, sizeof (glibtop_proc_segment));

	sprintf (input, "/proc/%d/stat", pid);

	f = fopen (input, "r");
	if (!f) return;
	
	nread = fread (input, 1, BUFSIZ, f);
	
	if (nread < 0) {
		fclose (f);
		return;
	}
	
	input [nread] = 0;
	
	/* This is from guile-utils/gtop/proc/readproc.c */
	
	/* split into "PID (cmd" and "<rest>" */
	tmp = strrchr (input, ')');
	*tmp = '\0';		/* replace trailing ')' with NUL */
	/* parse these two strings separately, skipping the leading "(". */
	sscanf(tmp + 2,		/* skip space after ')' too */
	       "%*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u "
	       "%*d %*d %*d %*d %*d %*d %*u %*u %*d %*u "
	       "%*u %*u %Lu %Lu %Lu", &buf->start_code,
	       &buf->end_code, &buf->start_stack);
	
	fclose (f);

	sprintf (input, "/proc/%d/statm", pid);

	f = fopen (input, "r");
	if (!f) return;

	nread = fread (input, 1, BUFSIZ, f);

	if (nread < 0) {
		fclose (f);
		return;
	}

	input [nread] = 0;

	sscanf (input, "%*d %*d %*d %Lu %Lu %Lu %Lu",
		&buf->text_rss, &buf->shlib_rss,
		&buf->data_rss, &buf->dirty_size);

	buf->text_rss   <<= pageshift;
	buf->shlib_rss  <<= pageshift;
	buf->data_rss   <<= pageshift;
	buf->dirty_size <<= pageshift;

	fclose (f);

	buf->flags = _glibtop_sysdeps_proc_segment;
}
