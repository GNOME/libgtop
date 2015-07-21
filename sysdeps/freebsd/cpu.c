/* Copyright (C) 1998 Joshua Sled
   This file is part of LibGTop 1.0.

   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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
#include <glibtop/error.h>
#include <glibtop/cpu.h>

#include <glibtop_suid.h>

static const unsigned long _glibtop_sysdeps_cpu =
(1L << GLIBTOP_CPU_TOTAL) + (1L << GLIBTOP_CPU_USER) +
(1L << GLIBTOP_CPU_NICE) + (1L << GLIBTOP_CPU_SYS) +
(1L << GLIBTOP_CPU_IDLE) + (1L << GLIBTOP_CPU_FREQUENCY) +
(1L << GLIBTOP_CPU_IRQ);

static const unsigned long _glibtop_sysdeps_cpu_smp =
(1L << GLIBTOP_XCPU_TOTAL) + (1L << GLIBTOP_XCPU_USER) +
(1L << GLIBTOP_XCPU_NICE) + (1L << GLIBTOP_XCPU_SYS) +
(1L << GLIBTOP_XCPU_IDLE) + (1L << GLIBTOP_XCPU_IRQ);

/* Init function. */

void
_glibtop_init_cpu_s (glibtop *server)
{
	server->sysdeps.cpu = _glibtop_sysdeps_cpu;

	if (server->ncpu)
		server->sysdeps.cpu |= _glibtop_sysdeps_cpu_smp;
}

/* Provides information about cpu usage. */

void
glibtop_get_cpu_s (glibtop *server, glibtop_cpu *buf)
{
	long cpts [CPUSTATES];
	long *cp_times = NULL;
	struct clockinfo ci;
	size_t length;
	int ncpu, i;

	memset (buf, 0, sizeof (glibtop_cpu));

	length = sizeof (cpts);
	if (sysctlbyname ("kern.cp_time", cpts, &length, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.cp_time)");
		return;
	}

	/* Get the clockrate data */
	length = sizeof (ci);
	if (sysctlbyname ("kern.clockrate", &ci, &length, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.cockrate)");
		return;
	}

	length = 0;
	if (sysctlbyname ("kern.cp_times", NULL, &length, NULL, 0) == 0) {
		cp_times = g_malloc (length);
		length = sizeof(long) * CPUSTATES * (length / (sizeof(long) * CPUSTATES));
		if (sysctlbyname ("kern.cp_times", cp_times, &length, NULL, 0)) {
			g_free (cp_times);
			cp_times = NULL;
		}
	}

	/* set user time */
	buf->user = cpts [CP_USER];
	/* set nice time */
	buf->nice = cpts [CP_NICE];
	/* set sys time */
	buf->sys = cpts [CP_SYS];
	/* set idle time */
	buf->idle = cpts [CP_IDLE];
	/* set irq */
	buf->irq = cpts [CP_INTR];

	/* set frequency */
	buf->frequency = (ci.stathz ? ci.stathz : ci.hz);
	/* set total */
	buf->total = cpts [CP_USER] + cpts [CP_NICE] \
		+ cpts [CP_SYS] + cpts [CP_IDLE] + cpts [CP_INTR];

	ncpu = server->ncpu + 1;

	for (i = 0; i < ncpu; i++) {
		if (cp_times) {
			buf->xcpu_user[i] = cp_times[i * CPUSTATES + CP_USER];
			buf->xcpu_nice[i] = cp_times[i * CPUSTATES + CP_NICE];
			buf->xcpu_sys[i] = cp_times[i * CPUSTATES + CP_SYS];
			buf->xcpu_idle[i] = cp_times[i * CPUSTATES + CP_IDLE];
			buf->xcpu_irq[i] = cp_times[i * CPUSTATES + CP_INTR];
		} else {
			buf->xcpu_user[i] = cpts [CP_USER] / ncpu;
			buf->xcpu_nice[i] = cpts [CP_NICE] / ncpu;
			buf->xcpu_sys[i] = cpts [CP_SYS] / ncpu;
			buf->xcpu_idle[i] = cpts [CP_IDLE] / ncpu;
			buf->xcpu_irq[i] = cpts [CP_INTR] / ncpu;
		}
		buf->xcpu_total[i] = buf->xcpu_user[i] + buf->xcpu_nice[i] \
				     + buf->xcpu_sys[i] + buf->xcpu_idle[i] \
				     + buf->xcpu_irq[i];
	}

	g_free (cp_times);

	/* Set the flags last. */
	buf->flags = _glibtop_sysdeps_cpu;

	if (ncpu > 1) {
		buf->flags |= _glibtop_sysdeps_cpu_smp;
	}
}
