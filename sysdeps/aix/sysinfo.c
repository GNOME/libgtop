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

#include <sys/systemcfg.h>

#include <config.h>
#include <glibtop/sysinfo.h>

static const unsigned long _glibtop_sysdeps_sysinfo =
(1L << GLIBTOP_SYSINFO_CPUINFO);

static glibtop_sysinfo sysinfo;

static void add_info(glibtop_entry* entry, char* label, char* attribute)
{
	char* key;
	char* value;

	key = g_strdup(label);
	value = g_strdup(attribute);

	g_ptr_array_add(entry->labels, key);
	g_hash_table_insert(entry->values, key, value);
}

static void init_sysinfo(void)
{
	int cpu;
	char buf[64];

	memset (&sysinfo, 0, sizeof (glibtop_sysinfo));

	sysinfo.ncpu = MIN(_system_configuration.ncpus, GLIBTOP_NCPU);

	for (cpu = 0; cpu < sysinfo.ncpu ; cpu++)
	{
		/* init cpu data structure */

		sysinfo.cpuinfo[cpu].labels = g_ptr_array_new();
		sysinfo.cpuinfo[cpu].values = g_hash_table_new (NULL, NULL);

		/* add properties */

		switch(_system_configuration.architecture)
		{
			case POWER_RS:
				add_info( &sysinfo.cpuinfo[cpu]
					, "architecture"
					, "Power Classic");
				break;
			case POWER_PC:
				add_info( &sysinfo.cpuinfo[cpu]
					, "architecture"
					, "Power PC");
				break;
			case IA64:
				add_info( &sysinfo.cpuinfo[cpu]
					, "architecture"
					, "Intel IA64");
				break;
			default:
				add_info( &sysinfo.cpuinfo[cpu]
					, "architecture"
					, "Unknown");
				break;
		}

		if ( (_system_configuration.architecture == POWER_RS) ||
		     (_system_configuration.architecture == POWER_PC) )
		{
			switch(_system_configuration.implementation)
			{
				case POWER_RS1:
					add_info( &sysinfo.cpuinfo[cpu]
						, "implementation"
						, "RS1");
					break;
				case POWER_RSC:
					add_info( &sysinfo.cpuinfo[cpu]
						, "implementation"
						, "RSC");
					break;
				case POWER_RS2:
					add_info( &sysinfo.cpuinfo[cpu]
						, "implementation"
						, "RS2");
					break;
				case POWER_601:
					add_info( &sysinfo.cpuinfo[cpu]
						, "implementation"
						, "601");
					break;
				case POWER_603:
					add_info( &sysinfo.cpuinfo[cpu]
						, "implementation"
						, "603");
					break;
				case POWER_604:
					add_info( &sysinfo.cpuinfo[cpu]
						, "implementation"
						, "604");
					break;
				case POWER_620:
					add_info( &sysinfo.cpuinfo[cpu]
						, "implementation"
						, "620");
					break;
				case POWER_630:
					add_info( &sysinfo.cpuinfo[cpu]
						, "implementation"
						, "630");
					break;
				case POWER_A35:
					add_info( &sysinfo.cpuinfo[cpu]
						, "implementation"
						, "A35");
					break;
				case POWER_RS64II:
					add_info( &sysinfo.cpuinfo[cpu]
						, "implementation"
						, "RS64II");
					break;
				case POWER_RS64III:
					add_info( &sysinfo.cpuinfo[cpu]
						, "implementation"
						, "RS64III");
					break;
			}
		}
		else if (_system_configuration.architecture == IA64)
		{
			switch(_system_configuration.implementation)
			{
				case IA64_M1:
					add_info( &sysinfo.cpuinfo[cpu]
						, "implementation"
						, "IA64 M1");
					break;
				case IA64_M2:
					add_info( &sysinfo.cpuinfo[cpu]
						, "implementation"
						, "IA64 M2");
					break;
			}
		}

		sprintf(buf,"%d", _system_configuration.width);
		add_info( &sysinfo.cpuinfo[cpu], "width", buf);

		if (_system_configuration.cache_attrib & 1 )
		{
			/* L1 cache is present */

			sprintf(buf,"%d", _system_configuration.icache_size);
			add_info( &sysinfo.cpuinfo[cpu], "L1 instruction cache size", buf);

			sprintf(buf,"%d", _system_configuration.dcache_size);
			add_info( &sysinfo.cpuinfo[cpu], "L1 data cache size", buf);
		}

		sprintf(buf,"%d", _system_configuration.L2_cache_size);
		add_info( &sysinfo.cpuinfo[cpu], "L2 cache size", buf);
	}

	sysinfo.flags = _glibtop_sysdeps_sysinfo;
}

const glibtop_sysinfo *
glibtop_get_sysinfo_s (glibtop *server)
{
	static int first_time = 1;

	if (first_time)
	{
		init_sysinfo();
		first_time = 0;
	}

	return &sysinfo;
}
