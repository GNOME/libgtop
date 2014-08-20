/*
   This file is part of LibGTop 2.0.

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
#include <unistd.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procmap.h>

#include <glibtop_suid.h>

#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/vm_map.h>

static const unsigned long _glibtop_sysdeps_proc_map =
(1L << GLIBTOP_PROC_MAP_TOTAL) + (1L << GLIBTOP_PROC_MAP_NUMBER) +
(1L << GLIBTOP_PROC_MAP_SIZE);

static const unsigned long _glibtop_sysdeps_proc_map_entry =
(1L << GLIBTOP_MAP_ENTRY_START) + (1L << GLIBTOP_MAP_ENTRY_END) +
(1L << GLIBTOP_MAP_ENTRY_OFFSET) + (1L << GLIBTOP_MAP_ENTRY_PERM);

/* Init function. */

void
_glibtop_init_proc_map_p (glibtop *server)
{
	server->sysdeps.proc_map = _glibtop_sysdeps_proc_map;
}

/* Provides detailed information about a process. */

glibtop_map_entry *
glibtop_get_proc_map_p (glibtop *server, glibtop_proc_map *buf,	pid_t pid)
{
	glibtop_map_entry *maps;
	mach_port_t task;
	vm_address_t address;
	natural_t nesting_depth;
	int n;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_MAP), 0);

	memset (buf, 0, sizeof (glibtop_proc_map));

	/* !!! THE FOLLOWING CODE RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

	glibtop_suid_enter (server);

	if (task_for_pid (mach_task_self (), pid, &task) != KERN_SUCCESS) {
		glibtop_suid_leave (server);
		glibtop_warn_io_r (server, "task_for_pid (procmap)");
		return NULL;
	}

	maps = NULL;
	n = 0;
	address = 0;
	nesting_depth = 0;
	while (1) {
		vm_region_submap_info_data_64_t info;
		mach_msg_type_number_t info_count;
		vm_size_t size;

		info_count = VM_REGION_SUBMAP_INFO_COUNT_64;
		if (vm_region_recurse_64 (task, &address, &size, &nesting_depth,
			(vm_region_recurse_info_64_t)&info, &info_count))
			break;

		if (info.is_submap) {
			nesting_depth++;
			continue;
		}

		maps = g_realloc (maps, (n + 1) * sizeof (glibtop_map_entry));
		if (!maps) {
			glibtop_suid_leave (server);
			return NULL;
		}

		memset (maps + n, 0, sizeof (glibtop_map_entry));

		maps[n].start	= address;
		maps[n].end	= address + size - 1;
		maps[n].offset	= info.offset;
		if (info.protection & VM_PROT_READ)
			maps[n].perm |= GLIBTOP_MAP_PERM_READ;
		if (info.protection & VM_PROT_WRITE) 
			maps[n].perm |= GLIBTOP_MAP_PERM_WRITE;
		if (info.protection & VM_PROT_EXECUTE)      
			maps[n].perm |= GLIBTOP_MAP_PERM_EXECUTE;
		if (info.share_mode == SM_PRIVATE ||
		    info.share_mode == SM_PRIVATE_ALIASED)
			maps[n].perm |= GLIBTOP_MAP_PERM_PRIVATE;
		if (info.share_mode == SM_SHARED ||
		    info.share_mode == SM_TRUESHARED ||
		    info.share_mode == SM_SHARED_ALIASED)
			maps[n].perm |= GLIBTOP_MAP_PERM_SHARED;

		maps[n].flags	= _glibtop_sysdeps_proc_map_entry;

		address += size;
		n++;
	}

	glibtop_suid_leave (server);

	/* !!! END OF SUID ROOT PART !!! */

	buf->number	= n;
	buf->size	= sizeof (glibtop_map_entry);
	buf->total	= n * sizeof (glibtop_map_entry);
	buf->flags	= _glibtop_sysdeps_proc_map;

	return maps;
}
