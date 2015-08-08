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
#include <glibtop/shm_limits.h>

#include <kvm.h>
#include <rctl.h>
#include <sys/shm.h>

static const struct nlist nlst[] = { {"glibtop_shm_limits"}, {NULL} };

#if GLIBTOP_SOLARIS_RELEASE >=51000
static const unsigned long _glibtop_sysdeps_shm_limits = 0;
#else
# if GLIBTOP_SOLARIS_RELEASE < 50900
static const unsigned long _glibtop_sysdeps_shm_limits =
(1L << GLIBTOP_IPC_SHMMAX) + (1L << GLIBTOP_IPC_SHMMIN);

# else
static const unsigned long _glibtop_sysdeps_shm_limits =
(1L << GLIBTOP_IPC_SHMMAX) + (1L << GLIBTOP_IPC_SHMMIN) +
(1L << GLIBTOP_IPC_SHMMNI) + (1L << GLIBTOP_IPC_SHMSEG);
# endif
#endif

/* Init function. */

void
_glibtop_init_shm_limits_p (glibtop *server)
{
#if GLIBTOP_SOLARIS_RELEASE < 51000

	kvm_t * const kd = server->machine->kd;

	if(kd && !kvm_nlist(kd, nlst))
		server->sysdeps.shm_limits = _glibtop_sysdeps_shm_limits;
	else
		server->sysdeps.shm_limits = 0;
#endif
}

/* Provides information about sysv ipc limits. */

void
glibtop_get_shm_limits_p (glibtop *server, glibtop_shm_limits *buf)
{
#if GLIBTOP_SOLARIS_RELEASE < 51000

	kvm_t * const kd = server->machine->kd;
        glibtop_shm_limits sinfo;


	memset (buf, 0, sizeof (glibtop_shm_limits));

	if(!(server->sysdeps.shm_limits))
		return;

	if(kvm_read(kd, nlst[0].n_value, (void *)&sinfo,
                    sizeof(glibtop_shm_limits)) != sizeof(glibtop_shm_limits))

		return;

	buf->shmmax = sinfo.shmmax;
	buf->shmmni = sinfo.shmmni;
#if GLIBTOP_SOLARIS_RELEASE < 50900
	buf->shmmin = sinfo.shmmin;
	buf->shmseg = sinfo.shmseg;
# endif
#endif
#if GLIBTOP_SOLARIS_RELEASE >= 51000
       rctlblk_t *rblk;
       if ((rblk = malloc(rctlblk_size())) == NULL)
               return;

       if (getrctl("project.max-shm-memory", NULL, rblk, RCTL_FIRST) == -1)
               return;
       else
              buf->shmmax = rctlblk_get_value(rblk);

       if (getrctl("project.max-shm-ids", NULL, rblk, RCTL_FIRST) == -1)
               return;
       else
               buf->shmmni = rctlblk_get_value(rblk);

#endif
	buf->flags = _glibtop_sysdeps_shm_limits;
}
