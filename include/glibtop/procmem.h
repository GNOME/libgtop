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

#ifndef __GLIBTOP_PROCMEM_H__
#define __GLIBTOP_PROCMEM_H__

#include <glibtop.h>
#include <glibtop/global.h>

G_BEGIN_DECLS

#define GLIBTOP_PROC_MEM_SIZE		0
#define GLIBTOP_PROC_MEM_VSIZE		1
#define GLIBTOP_PROC_MEM_RESIDENT	2
#define GLIBTOP_PROC_MEM_SHARE		3
#define GLIBTOP_PROC_MEM_RSS		4
#define GLIBTOP_PROC_MEM_RSS_RLIM	5

#define GLIBTOP_MAX_PROC_MEM		6

typedef struct _glibtop_proc_mem	glibtop_proc_mem;

/* Memory section */

struct _glibtop_proc_mem
{
	guint64	flags;
	guint64 size;		/* total # of pages of memory */
	guint64 vsize;		/* number of pages of virtual memory ... */
	guint64 resident;	/* number of resident set
				 * (non-swapped) pages (4k) */
	guint64 share;		/* number of pages of shared (mmap'd) memory */
	guint64 rss;		/* resident set size */
	guint64 rss_rlim;	/* current limit (in bytes) of the rss
				 * of the process; usually 2,147,483,647 */
};

void glibtop_get_proc_mem(glibtop_proc_mem *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_MEM
#define glibtop_get_proc_mem_r		glibtop_get_proc_mem_p
#else
#define glibtop_get_proc_mem_r		glibtop_get_proc_mem_s
#endif

void glibtop_get_proc_mem_l (glibtop *server, glibtop_proc_mem *buf, pid_t pid);

#if GLIBTOP_SUID_PROC_MEM
void _glibtop_init_proc_mem_p (glibtop *server);
void glibtop_get_proc_mem_p (glibtop *server, glibtop_proc_mem *buf, pid_t pid);
#else
void _glibtop_init_proc_mem_s (glibtop *server);
void glibtop_get_proc_mem_s (glibtop *server, glibtop_proc_mem *buf, pid_t pid);
#endif


G_END_DECLS

#endif
