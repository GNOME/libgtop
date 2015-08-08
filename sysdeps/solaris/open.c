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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <config.h>
#include <glibtop/open.h>
#include <glibtop/cpu.h>
#include <glibtop/error.h>

#include <unistd.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/processor.h>

#include <glibtop_private.h>

/* We need to call this when kstat_chain_update() returns new KID.
 * In that case all kstat pointers and data are invalid, so we
 * need to reread everything. The condition shouldn't happen very
 * often.
 */

void
glibtop_get_kstats(glibtop *server)
{
    kstat_ctl_t *kc = server->machine->kc;
    kstat_t *ksp;
    int nproc_same, new_ncpu;

    server->ncpu = new_ncpu = sysconf(_SC_NPROCESSORS_CONF);


    if(!kc)
    {
	server->ncpu = new_ncpu;
	server->machine->vminfo_kstat = NULL;
	server->machine->system = NULL;
	server->machine->syspages = NULL;
	server->machine->bunyip = NULL;
	return;
    }

    do {

	ksp = kstat_lookup(kc, "unix", -1, "vminfo");
	server->machine->vminfo_kstat = ksp;
	if(ksp)
	{
	    kstat_read(kc, ksp, &server->machine->vminfo);
	    /* Don't change snaptime if we only need to reinitialize kstats */
	    if(!(server->machine->vminfo_snaptime))
		server->machine->vminfo_snaptime = ksp->ks_snaptime;
	}

	/* We don't know why was kstat chain invalidated. It could have
	   been because the number of processors changed. The sysconf()
	   man page says that values returned won't change during the
	   life time of a process, but let's hope that's just an error in
	   the documentation. */

	if((nproc_same = new_ncpu) == server->ncpu)
	{
	    int checked, i;
	    char cpu[20];

	    for(i = 0, checked = 0; i < GLIBTOP_NCPU || checked == new_ncpu; ++i)
                if(!server->machine->cpu_stat_kstat[i])

		{
		    sprintf(cpu, "cpu_stat%d", i);
		    if(!(server->machine->cpu_stat_kstat[i] =
			     kstat_lookup(kc, "cpu_stat", -1, cpu)))
		    {
			nproc_same = 0;
			break;
		    }
		    ++checked;
		}
	}

	if(!nproc_same)
	{
	    processorid_t p;
	    int found;
	    char cpu[20];

	    server->ncpu = new_ncpu = MIN(new_ncpu, GLIBTOP_NCPU);

	    for(p = 0, found = 0; p < GLIBTOP_NCPU && found != new_ncpu; ++p)
	    {
		if(p_online(p, P_STATUS) < 0)
		{
		    server->machine->cpu_stat_kstat[p] = NULL;
		    continue;
		}
		sprintf(cpu, "cpu_stat%d", (int)p);
		server->machine->cpu_stat_kstat[p] =
			kstat_lookup(kc, "cpu_stat", -1, cpu);
		++found;
	    }
	}

	server->machine->system   = kstat_lookup(kc, "unix", -1, "system_misc");
	server->machine->syspages = kstat_lookup(kc, "unix", -1, "system_pages");
	server->machine->bunyip   = kstat_lookup(kc, "bunyip", -1, "mempages");

    } while(kstat_chain_update(kc) > 0 &&
	    (new_ncpu = sysconf(_SC_NPROCESSORS_CONF)));

    /* We'll ignore -1 from kstat_chain_update here, since it really
       shouldn't happen */
}

void
glibtop_open_s (glibtop *server, const char *program_name,
		const unsigned long features, const unsigned flags)
{
    kstat_ctl_t *kc;
    kstat_t *ksp;
    kstat_named_t *kn;
    int i, page;
    void *dl;

    server->name = program_name;

    page = sysconf(_SC_PAGESIZE) >> 10;
    for(i = 0; page; ++i, page >>= 1);
    server->machine->pagesize = i - 1;
    server->machine->ticks = sysconf(_SC_CLK_TCK);
    if(server->machine->kc)
    	kstat_close(server->machine->kc);
    server->machine->kc = kc = kstat_open ();

#if 0
    for (ksp = server->machine->kc->kc_chain; ksp != NULL; ksp = ksp->ks_next) {
	if (!strcmp (ksp->ks_class, "vm") && !strcmp (ksp->ks_name, "vminfo")) {
	    server->machine->vminfo_kstat = ksp;
	    kstat_read (server->machine->kc, ksp, &server->machine->vminfo);
	    server->machine->vminfo_snaptime = ksp->ks_snaptime;
	    continue;
	}

	if (!strcmp (ksp->ks_class, "misc") && !strncmp (ksp->ks_name, "cpu_stat", 8)) {
	    int cpu;

	    if ((sscanf (ksp->ks_name+8, "%d", &cpu) != 1) || (cpu > 63))
		continue;

	    if (cpu >= server->ncpu)
		server->ncpu = cpu+1;

	    server->machine->cpu_stat_kstat [cpu] = ksp;
	    continue;
	}
    }

#endif

    if (!kc)
	glibtop_warn_io_r (server, "kstat_open ()");

    server->ncpu = -1;  /* Force processor detection */
    server->machine->vminfo_snaptime = 0;  /* Force snaptime read */
    glibtop_get_kstats(server);

    server->machine->boot = 0;
    if((ksp = server->machine->system) && kstat_read(kc, ksp, NULL) >= 0)
    {
	kn = (kstat_named_t *)kstat_data_lookup(ksp, "boot_time");
	if(kn)
	    switch(kn->data_type)
	    {
#ifdef KSTAT_DATA_INT32
		case KSTAT_DATA_INT32:  server->machine->boot = kn->value.i32;
					break;
		case KSTAT_DATA_UINT32: server->machine->boot = kn->value.ui32;
					break;
		case KSTAT_DATA_INT64:  server->machine->boot = kn->value.i64;
					break;
		case KSTAT_DATA_UINT64: server->machine->boot = kn->value.ui64;
					break;
#else
		case KSTAT_DATA_LONG:      server->machine->boot = kn->value.l;
					   break;
		case KSTAT_DATA_ULONG:     server->machine->boot = kn->value.ul;
					   break;
		case KSTAT_DATA_LONGLONG:  server->machine->boot = kn->value.ll;
					   break;
		case KSTAT_DATA_ULONGLONG: server->machine->boot = kn->value.ull;
					   break;
#endif
	    }
    }

    /* Now let's have a bit of magic dust... */

#if GLIBTOP_SOLARIS_RELEASE >= 50600

    dl = dlopen("/usr/lib/libproc.so", RTLD_LAZY);
    if(server->machine->libproc)
    	dlclose(server->machine->libproc);
    server->machine->libproc = dl;
    if(dl)
    {
       void *func;

       func = dlsym(dl, "Pobjname");		/* Solaris 8 */
       if(!func)
	  func = dlsym(dl, "proc_objname");	/* Solaris 7 */
       server->machine->objname = (void (*)
				 (void *, uintptr_t, const char *, size_t))func;
       server->machine->pgrab = (struct ps_prochandle *(*)(pid_t, int, int *))
	  		       dlsym(dl, "Pgrab");
       server->machine->pfree = (void (*)(void *))dlsym(dl, "Pfree");
       
    }
    else
    {
       server->machine->objname = NULL;
       server->machine->pgrab = NULL;
       server->machine->pfree = NULL;
    }
#endif
    server->machine->me = getpid();
}
