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
#include <glibtop/proclist.h>
#include <glibtop/xmalloc.h>

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>

#include <glibtop_private.h>

static const unsigned long _glibtop_sysdeps_proclist =
(1L << GLIBTOP_ARRAY_TOTAL) + (1L << GLIBTOP_ARRAY_NUMBER) +
(1L << GLIBTOP_ARRAY_SIZE);

/* Init function. */

int
glibtop_init_proclist_s (glibtop *server)
{
    server->sysdeps.proclist = _glibtop_sysdeps_proclist;

    return 0;
}

#define BLOCK_COUNT	256
#define BLOCK_SIZE	(BLOCK_COUNT * sizeof (unsigned))

/* Fetch list of currently running processes.
 *
 * IMPORTANT NOTE:
 *   On error, this function MUST return NULL and set buf->flags to zero !
 *   On success, it returnes a pointer to a list of buf->number elements
 *   each buf->size big. The total size is stored in buf->total. */

unsigned *
glibtop_get_proclist_s (glibtop *server, glibtop_array *array,
			int64_t which, int64_t arg)
{
    DIR *proc;
    struct dirent *entry;
    char buffer [BUFSIZ];
    unsigned count, total, pid, mask;
    unsigned pids [BLOCK_COUNT], *pids_chain = NULL;
    unsigned pids_size = 0, pids_offset = 0, new_size;
    struct stat statb;
    int len, ok;

    memset (array, 0, sizeof (glibtop_array));
    mask = which & ~GLIBTOP_KERN_PROC_MASK;
    which &= GLIBTOP_KERN_PROC_MASK;

    /* Check if the user wanted only one process */

    if(which == GLIBTOP_KERN_PROC_PID) {
	pid = arg;

	if(mask) {
#ifdef HAVE_PROCFS_H
	    struct psinfo psinfo;
#else
	    struct prpsinfo psinfo;
#endif
	    if(glibtop_get_proc_data_psinfo_s(server, &psinfo, pid))
		return NULL;
	    if(mask & GLIBTOP_EXCLUDE_IDLE && !psinfo.pr_pctcpu)
		return NULL;
	    if(mask & GLIBTOP_EXCLUDE_SYSTEM && psinfo.pr_flag & SSYS)
		return NULL;
	    if(mask & GLIBTOP_EXCLUDE_NOTTY && psinfo.pr_ttydev == PRNODEV)
		return NULL;
	} else {
	    sprintf(buffer, "/proc/%ld", (long)arg);
	    if(s_stat(buffer, &statb) < 0)
		return NULL;
	}
	if(!(pids_chain = glibtop_calloc_r(server, 1, sizeof(unsigned))))
	    return NULL;
	*pids_chain = pid;
	array->number = 1;
	array->size = sizeof(unsigned);
	array->total = array->number * array->size;
	array->flags = _glibtop_sysdeps_proclist;
	return pids_chain;
    }

    proc = opendir ("/proc");
    if (!proc) return NULL;

    /* read every every entry in /proc */

    for (count = total = 0, entry = readdir (proc);
	 entry; entry = readdir (proc)) {
	ok = 1; len = strlen (entry->d_name);

	/* does it consist entirely of digits? */
#if 0
	/* It does, except for "." and "..". Let's speed up */

	for (i = 0; i < len; i++)
	    if (!isdigit (entry->d_name [i])) ok = 0;
	if (!ok) continue;
#else
	if(entry->d_name[0] == '.')
	    continue;
#endif

	/* convert it in a number */
#if 0
	if (sscanf (entry->d_name, "%u", &pid) != 1) continue;
#else
	pid = (unsigned)atol(entry->d_name);
#endif

#ifdef HAVE_PROCFS_H

	/* Can we skip it based on the request? We have
	   RUID and RGID in struct stat. But we can't do it
	   like this for LP64 process, because stat() will fail.
	   XXX Unimplemented for now */

	if(!mask && which == GLIBTOP_KERN_PROC_RUID) {
	    sprintf (buffer, "/proc/%d", pid);
	    if (s_stat (buffer, &statb)) continue;

	    if (!S_ISDIR (statb.st_mode)) continue;

	    if(statb.st_uid != arg) continue;
	}

	if(mask || which != GLIBTOP_KERN_PROC_ALL) {
	    struct psinfo psinfo;

	    if(glibtop_get_proc_data_psinfo_s(server, &psinfo, pid))
		continue;
	    if(mask) {
		if(mask & GLIBTOP_EXCLUDE_IDLE && !psinfo.pr_pctcpu)
		    continue;
		if(mask & GLIBTOP_EXCLUDE_SYSTEM && psinfo.pr_flag & SSYS)
		    continue;
		if(mask & GLIBTOP_EXCLUDE_NOTTY
		   && psinfo.pr_ttydev == PRNODEV)
		    continue;
	    }
	    switch(which) {
	    case GLIBTOP_KERN_PROC_PGRP:
		if(psinfo.pr_pgid != arg)
		    continue;
		break;
	    case GLIBTOP_KERN_PROC_SESSION:
		if(psinfo.pr_sid != arg)
		    continue;
		break;
	    case GLIBTOP_KERN_PROC_TTY:
		if(psinfo.pr_ttydev != arg)
		    continue;
		break;
	    case GLIBTOP_KERN_PROC_UID:
		if(psinfo.pr_euid != arg)
		    continue;
		break;
	    case GLIBTOP_KERN_PROC_RUID:
		if(psinfo.pr_uid != arg)
		    continue;
		break;
	    default:
		break;
	    }
	}
#endif
	/* Fine. Now we first try to store it in pids. If this buffer is
	 * full, we copy it to the pids_chain. */

	if (count >= BLOCK_COUNT) {
	    /* The following call to glibtop_realloc will be
	     * equivalent to glibtop_malloc () if `pids_chain' is
	     * NULL. We just calculate the new size and copy `pids'
	     * to the beginning of the newly allocated block. */

	    new_size = pids_size + BLOCK_SIZE;

	    pids_chain = glibtop_realloc_r
		(server, pids_chain, new_size);

	    memcpy (pids_chain + pids_offset, pids, BLOCK_SIZE);

	    pids_size = new_size;

	    pids_offset += BLOCK_COUNT;

	    count = 0;
	}

	/* pids is now big enough to hold at least one single pid. */
		
	pids [count++] = pid;

	total++;
    }
	
    s_closedir (proc);

    /* count is only zero if an error occured (one a running Linux system,
     * we have at least one single process). */

    if (!count) return NULL;

    /* The following call to glibtop_realloc will be equivalent to
     * glibtop_malloc if pids_chain is NULL. We just calculate the
     * new size and copy pids to the beginning of the newly allocated
     * block. */
	
    new_size = pids_size + count * sizeof (unsigned);
	
    pids_chain = glibtop_realloc_r (server, pids_chain, new_size);
	
    memcpy (pids_chain + pids_offset, pids, count * sizeof (unsigned));
	
    pids_size = new_size;
	
    pids_offset += BLOCK_COUNT;

    /* Since everything is ok now, we can set array->flags, fill in the
     * remaining fields and return the `pids_chain'. */

    array->flags = _glibtop_sysdeps_proclist;

    array->size = sizeof (unsigned);
    array->number = total;

    array->total = array->number * array->size;

    return pids_chain;
}
