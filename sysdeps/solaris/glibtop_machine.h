/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, March 1999.

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

#ifndef __GLIBTOP_MACHINE_H__
#define __GLIBTOP_MACHINE_H__

#include <sys/param.h>
#ifdef HAVE_PROCFS_H
#include <procfs.h>
#else
#include <sys/procfs.h>
#endif
#include <fcntl.h>

#include <kstat.h>
#include <kvm.h>
#include <sys/sysinfo.h>

G_BEGIN_DECLS

struct _glibtop_machine
{
    uid_t uid, euid;
    gid_t gid, egid;
    pid_t me;			/* Don't ask why we need this */

    kvm_t *kd;

    kstat_ctl_t *kc;

    kstat_t *vminfo_kstat;
    hrtime_t vminfo_snaptime;
    vminfo_t vminfo;

    kstat_t *cpu_stat_kstat [64];

    kstat_t *system;		/* boot_time & avenrun* where needed */
    kstat_t *syspages;		/* memory usage */
    kstat_t *bunyip;		/* more memory usage */

    int pagesize;		/* in bits to shift, ie. 2^pagesize gives Kb */
    int ticks;			/* clock ticks, as returned by sysconf() */
    unsigned long long boot;	/* boot time, although it's ui32 in kstat */
    void *libproc;		/* libproc handle */
#if GLIBTOP_SOLARIS_RELEASE >= 50600
    void (*objname)(void *, uintptr_t, const char *, size_t);
    struct ps_prochandle *(*pgrab)(pid_t, int, int *);
    void (*pfree)(void *);
#else
    void *filler[3];
#endif
};

G_END_DECLS

#endif /* __GLIBTOP_MACHINE_H__ */
