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
#include <glibtop.h>
#include <glibtop/sem_limits.h>

#include <kvm.h>
#include <rctl.h>
#include <sys/sem.h>

static const struct nlist nlst[] = { {"glibtop_sem_limits"}, {NULL} };
#if GLIBTOP_SOLARIS_RELEASE < 51000
static const unsigned long _glibtop_sysdeps_sem_limits =
#if GLIBTOP_SOLARIS_RELEASE <= 50700
(1L << GLIBTOP_IPC_SEMMAP) +
#endif
(1L << GLIBTOP_IPC_SEMMNI) + (1L << GLIBTOP_IPC_SEMMNS) +
(1L << GLIBTOP_IPC_SEMMNU) + (1L << GLIBTOP_IPC_SEMMSL) +
(1L << GLIBTOP_IPC_SEMOPM) + (1L << GLIBTOP_IPC_SEMUME) +
(1L << GLIBTOP_IPC_SEMUSZ) + (1L << GLIBTOP_IPC_SEMVMX) +
(1L << GLIBTOP_IPC_SEMAEM);
#else
static const unsigned long _glibtop_sysdeps_sem_limits = 0;
#endif


/* Init function. */

void
_glibtop_init_sem_limits_p (glibtop *server)
{
#if GLIBTOP_SOLARIS_RELEASE < 51000

   	kvm_t *kd = server->machine->kd;

	if(kd && !kvm_nlist(kd, nlst))
		server->sysdeps.sem_limits = _glibtop_sysdeps_sem_limits;
	else
	   	server->sysdeps.sem_limits = 0;
#endif
}

/* Provides information about sysv sem limits. */

void
glibtop_get_sem_limits_p (glibtop *server, glibtop_sem_limits *buf)
{
#if GLIBTOP_SOLARIS_RELEASE < 51000
   	kvm_t *kd = server->machine->kd;
        glibtop_sem_limits sinfo;

	memset (buf, 0, sizeof (glibtop_sem_limits));

	if(!(server->sysdeps.sem_limits))
	   	return;
	if(kvm_read(kd, nlst[0].n_value, (void *)&sinfo,
                    sizeof(glibtop_sem_limits)) != sizeof(glibtop_sem_limits))

	   	return;

#if GLIBTOP_SOLARIS_RELEASE <= 50700
	/* This field don't exist anymore in Solaris 8.
	 * Thanks to Laszlo PETER <Laszlo.Peter@ireland.sun.com>. */
	buf->semmap = sinfo.semmap;
#endif
	buf->semmni = sinfo.semmni;
	buf->semmns = sinfo.semmns;
	buf->semmnu = sinfo.semmnu;
	buf->semmsl = sinfo.semmsl;
	buf->semopm = sinfo.semopm;
	buf->semume = sinfo.semume;
	buf->semusz = sinfo.semusz;
	buf->semvmx = sinfo.semvmx;
	buf->semaem = sinfo.semaem;
#endif

#if GLIBTOP_SOLARIS_RELEASE >= 51000
       rctlblk_t *rblk;
       if ((rblk = malloc(rctlblk_size())) == NULL)
               return;

       if (getrctl("process.max-sem-ops", NULL, rblk, RCTL_FIRST) == -1)
               return;
       else
               buf->semopm = rctlblk_get_value(rblk);

       if (getrctl("process.max-sem-nsems", NULL, rblk, RCTL_FIRST) == -1)
               return;
       else
               buf->semmsl = rctlblk_get_value(rblk);

       if (getrctl("project.max-sem-ids", NULL, rblk, RCTL_FIRST) == -1)
               return;
       else
               buf->semmni = rctlblk_get_value(rblk);
	/* there are only 3 fields, the remaining ones have been obsoleted in S10
	 and no longer have system-wide limits */
#endif
       buf->flags = _glibtop_sysdeps_sem_limits;

}
