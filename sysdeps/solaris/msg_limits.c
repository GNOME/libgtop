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
#include <glibtop/msg_limits.h>

#include <kvm.h>
#include <rctl.h>
#include <sys/msg.h>

static const struct nlist nlst[] = { {"glibtop_msg_limits"}, {NULL} };
#if GLIBTOP_SOLARIS_RELEASE < 51000
static const unsigned long _glibtop_sysdeps_msg_limits =
#if GLIBTOP_SOLARIS_RELEASE <= 50700
(1L << GLIBTOP_IPC_MSGMAP) +  (1L << GLIBTOP_IPC_MSGSSZ) +
#endif
(1L << GLIBTOP_IPC_MSGPOOL) + (1L << GLIBTOP_IPC_MSGMAX) +
(1L << GLIBTOP_IPC_MSGMNB) + (1L << GLIBTOP_IPC_MSGMNI) +
(1L << GLIBTOP_IPC_MSGTQL);
#else
static const unsigned long _glibtop_sysdeps_msg_limits = 0;
#endif


/* Init function. */

void
_glibtop_init_msg_limits_p (glibtop *server)
{
#if GLIBTOP_SOLARIS_RELEASE < 51000

   	kvm_t *kd = server->machine->kd;

	if(kd && !kvm_nlist(kd, nlst))
		server->sysdeps.msg_limits = _glibtop_sysdeps_msg_limits;
	else
	   	server->sysdeps.msg_limits = 0;
#endif
}

/* Provides information about sysv ipc limits. */

void
glibtop_get_msg_limits_p (glibtop *server, glibtop_msg_limits *buf)
{
#if GLIBTOP_SOLARIS_RELEASE < 51000

   	kvm_t *kd = server->machine->kd;
        glibtop_msg_limits minfo;


	memset (buf, 0, sizeof (glibtop_msg_limits));

	if(!(server->sysdeps.msg_limits))
	   	return;
	if(kvm_read(kd, nlst[0].n_value, (void *)&minfo,
                    sizeof(glibtop_msg_limits)) != sizeof(glibtop_msg_limits))

	   	return;

#if GLIBTOP_SOLARIS_RELEASE <= 50700
	/* These fields don't exist anymore in Solaris 8.
	 * Thanks to Laszlo PETER <Laszlo.Peter@ireland.sun.com>. */
	buf->msgmap = minfo.msgmap;
	buf->msgssz = minfo.msgssz;
#endif
	buf->msgmax = minfo.msgmax;
	buf->msgmnb = minfo.msgmnb;
	buf->msgmni = minfo.msgmni;
	buf->msgtql = minfo.msgtql;
#endif
#if GLIBTOP_SOLARIS_RELEASE >= 51000
       rctlblk_t *rblk;
       if ((rblk = malloc(rctlblk_size())) == NULL)
               return;

       if (getrctl("project.max-msg-qbytes", NULL, rblk, RCTL_FIRST) == -1)
               return;
       else
               buf->msgmnb = rctlblk_get_value(rblk);

       if (getrctl("project.max-msg-ids", NULL, rblk, RCTL_FIRST) == -1)
               return;
       else
               buf->msgmni = rctlblk_get_value(rblk);

       if (getrctl("project.max-msg-messages", NULL, rblk, RCTL_FIRST) == -1)
               return;
       else
               buf->msgtql = rctlblk_get_value(rblk);
       /* this is the maximum size of a system V message, which has been obsoleted as a kernel tunable value now */
       /* and it should always be 65535 instead, so I list it here, if needed, can be removed */	       
       buf->msgmax = 65535;
       
#endif

       buf->msgpool = buf->msgmni * buf->msgmnb >> 10;
       buf->flags = _glibtop_sysdeps_msg_limits;

}
