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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <locale.h>
#include <libintl.h>
#include <stdio.h>

#include <glibtop.h>
#include <glibtop/open.h>
#include <glibtop/close.h>
#include <glibtop/sysdeps.h>

#include <glibtop/parameter.h>

#ifndef PROFILE_COUNT
#define PROFILE_COUNT	1
#endif

int
main (int argc, char *argv [])
{
	glibtop_sysdeps sysdeps;
	unsigned method, count, port;
	char buffer [BUFSIZ];

	count = PROFILE_COUNT;

	setlocale (LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, GTOPLOCALEDIR);
	textdomain (GETTEXT_PACKAGE);

	glibtop_init_r (&glibtop_global_server, 0, GLIBTOP_INIT_NO_OPEN);

	glibtop_get_parameter (GLIBTOP_PARAM_METHOD, &method, sizeof (method));

	printf ("Method = %d\n", method);

	count = glibtop_get_parameter (GLIBTOP_PARAM_COMMAND, buffer, BUFSIZ);
	buffer [count] = 0;

	printf ("Command = '%s'\n", buffer);

	count = glibtop_get_parameter (GLIBTOP_PARAM_HOST, buffer, BUFSIZ);
	buffer [count] = 0;

	glibtop_get_parameter (GLIBTOP_PARAM_PORT, &port, sizeof (port));

	printf ("Host = '%s' - %u\n\n", buffer, port);

	glibtop_init_r (&glibtop_global_server, 0, 0);

	glibtop_get_sysdeps (&sysdeps);

#define FEATURE_CHECK(f) ((sysdeps.features & (1L << GLIBTOP_SYSDEPS_##f)) ? 1 : 0)

	printf ("Sysdeps (0x%08lx):\n\n"
		"\tfeatures:\t\t0x%08lx\n\n"
		"\tcpu:\t\t%d\t0x%08lx\n"
		"\tmem:\t\t%d\t0x%08lx\n"
		"\tswap:\t\t%d\t0x%08lx\n\n"
		"\tuptime:\t\t%d\t0x%08lx\n"
		"\tloadavg:\t%d\t0x%08lx\n\n"
		"\tshm_limits:\t%d\t0x%08lx\n"
		"\tmsg_limits:\t%d\t0x%08lx\n"
		"\tsem_limits:\t%d\t0x%08lx\n\n"
		"\tproclist:\t%d\t0x%08lx\n\n"
		"\tproc_state:\t%d\t0x%08lx\n"
		"\tproc_uid:\t%d\t0x%08lx\n"
		"\tproc_mem:\t%d\t0x%08lx\n"
		"\tproc_time:\t%d\t0x%08lx\n"
		"\tproc_signal:\t%d\t0x%08lx\n"
		"\tproc_kernel:\t%d\t0x%08lx\n"
		"\tproc_segment:\t%d\t0x%08lx\n"
		"\tproc_args:\t%d\t0x%08lx\n"
		"\tproc_map:\t%d\t0x%08lx\n"
		"\tproc_io:\t%d\t0x%08lx\n\n"
		"\tmountlist:\t%d\t0x%08lx\n"
		"\tfsusage:\t%d\t0x%08lx\n\n"
		"\tnetload:\t%d\t0x%08lx\n"
		"\tppp:\t\t%d\t0x%08lx\n\n",
		(unsigned long) sysdeps.flags,
		(unsigned long) sysdeps.features,
		FEATURE_CHECK(CPU),
		(unsigned long) sysdeps.cpu,
		FEATURE_CHECK(MEM),
		(unsigned long) sysdeps.mem,
		FEATURE_CHECK(SWAP),
		(unsigned long) sysdeps.swap,
		FEATURE_CHECK(UPTIME),
		(unsigned long) sysdeps.uptime,
		FEATURE_CHECK(LOADAVG),
		(unsigned long) sysdeps.loadavg,
		FEATURE_CHECK(SHM_LIMITS),
		(unsigned long) sysdeps.shm_limits,
		FEATURE_CHECK(MSG_LIMITS),
		(unsigned long) sysdeps.msg_limits,
		FEATURE_CHECK(SEM_LIMITS),
		(unsigned long) sysdeps.sem_limits,
		FEATURE_CHECK(PROCLIST),
		(unsigned long) sysdeps.proclist,
		FEATURE_CHECK(PROC_STATE),
		(unsigned long) sysdeps.proc_state,
		FEATURE_CHECK(PROC_UID),
		(unsigned long) sysdeps.proc_uid,
		FEATURE_CHECK(PROC_MEM),
		(unsigned long) sysdeps.proc_mem,
		FEATURE_CHECK(PROC_TIME),
		(unsigned long) sysdeps.proc_time,
		FEATURE_CHECK(PROC_SIGNAL),
		(unsigned long) sysdeps.proc_signal,
		FEATURE_CHECK(PROC_KERNEL),
		(unsigned long) sysdeps.proc_kernel,
		FEATURE_CHECK(PROC_SEGMENT),
		(unsigned long) sysdeps.proc_segment,
		FEATURE_CHECK(PROC_ARGS),
		(unsigned long) sysdeps.proc_args,
		FEATURE_CHECK(PROC_MAP),
		(unsigned long) sysdeps.proc_map,
		FEATURE_CHECK(PROC_IO),
		(unsigned long) sysdeps.proc_io,
		FEATURE_CHECK(MOUNTLIST),
		(unsigned long) sysdeps.mountlist,
		FEATURE_CHECK(FSUSAGE),
		(unsigned long) sysdeps.fsusage,
		FEATURE_CHECK(NETLOAD),
		(unsigned long) sysdeps.netload,
		FEATURE_CHECK(PPP),
		(unsigned long) sysdeps.ppp);

	glibtop_close ();

	exit (0);
}
