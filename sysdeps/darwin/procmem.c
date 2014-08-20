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
#include <glibtop/procmem.h>

#include <glibtop_suid.h>

#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/mach_traps.h>
#include <mach/task.h>
#include <mach/vm_map.h>
#include <mach/shared_memory_server.h>

static const unsigned long _glibtop_sysdeps_proc_mem =
(1L << GLIBTOP_PROC_MEM_SIZE)     + (1L << GLIBTOP_PROC_MEM_VSIZE) +
(1L << GLIBTOP_PROC_MEM_RESIDENT) + (1L << GLIBTOP_PROC_MEM_SHARE) +
(1L << GLIBTOP_PROC_MEM_RSS);

/* Init function. */

void
_glibtop_init_proc_mem_p (glibtop *server)
{
	server->sysdeps.proc_mem = _glibtop_sysdeps_proc_mem;
}


#define	SHARED_TABLE_SIZE	137
#define	TEXT_SEGMENT_START	(GLOBAL_SHARED_TEXT_SEGMENT)
#define TEXT_SEGMENT_END	(GLOBAL_SHARED_TEXT_SEGMENT + SHARED_TEXT_REGION_SIZE)
#define	DATA_SEGMENT_START	(GLOBAL_SHARED_DATA_SEGMENT)
#define DATA_SEGMENT_END	(GLOBAL_SHARED_DATA_SEGMENT + SHARED_DATA_REGION_SIZE)

struct shared_info {
	unsigned obj_id;
	unsigned share_mode;
	unsigned page_count;
	unsigned ref_count;
	unsigned task_ref_count;
	vm_size_t size;
	struct shared_info *next;
};
typedef struct shared_info shared_table[SHARED_TABLE_SIZE];
typedef struct shared_info shared_info;

static void
shared_table_init (shared_table table)
{
	memset (table, 0, sizeof (shared_table));
}

static void
shared_table_free (glibtop *server, shared_table table)
{
	int i;

	for (i = 0; i < SHARED_TABLE_SIZE; i++) {
		shared_info *info = table [i].next;

		while (info) {
			shared_info *next = info->next;
			g_free (info);
			info = next;
		}
	}
}

static void
shared_table_register (glibtop *server, shared_table table,
		       vm_region_top_info_data_t *top, vm_size_t size)
{
	shared_info *info, *last;

	info = last = &table [top->obj_id % SHARED_TABLE_SIZE];
	while (info) {
		if (info->obj_id == top->obj_id) {
			info->task_ref_count++;
			return;
		}
		last = info;
		info = info->next;
	}

	info = g_malloc (sizeof (shared_info));
	if (info) {
		info->obj_id = top->obj_id;
		info->share_mode = top->share_mode;
		info->page_count = top->shared_pages_resident;
		info->ref_count = top->ref_count;
		info->task_ref_count = 1;
		info->size = size;
		info->next = NULL;
		last->next = info;
	}
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_mem_p (glibtop *server, glibtop_proc_mem *buf,
			pid_t pid)
{
	task_basic_info_data_t tinfo;
        mach_msg_type_number_t info_count;
	vm_size_t vsize, resident, private, vprivate, shared;
	kern_return_t retval;
	shared_table stable;
	vm_address_t address;
	mach_port_t task;
	int i, split;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_MEM), 0);

	memset (buf, 0, sizeof (glibtop_proc_mem));

	address = 0;
	split = 0;
	vsize = resident = private = vprivate = shared = 0;
	shared_table_init (stable);

	/* !!! THE FOLLOWING CODE RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

	glibtop_suid_enter (server);
	retval = task_for_pid (mach_task_self (), pid, &task);
	glibtop_suid_leave (server);

	/* !!! END OF SUID ROOT PART !!! */

	if (retval)
		return;

	info_count = TASK_BASIC_INFO_COUNT;
	if (task_info (task, TASK_BASIC_INFO, (task_info_t)&tinfo, &info_count)) {
		glibtop_warn_io_r (server, "task_info (procmem)");
		return;
	}
	vsize = tinfo.virtual_size;
	resident = tinfo.resident_size;

	/* !!! THE FOLLOWING CODE RUNS SUID ROOT - CHANGE WITH CAUTION !!! */

	glibtop_suid_enter (server);
	while (1) {
		vm_region_basic_info_data_64_t basic;
		vm_region_top_info_data_t top;
		mach_port_t object_name;
		vm_size_t size;

		info_count = VM_REGION_BASIC_INFO_COUNT_64;
		if (vm_region_64 (task, &address, &size, VM_REGION_BASIC_INFO,
				  (vm_region_info_t)&basic,
				  &info_count, &object_name))
			break;

		info_count = VM_REGION_TOP_INFO_COUNT;
		if (vm_region_64 (task, &address, &size, VM_REGION_TOP_INFO,
			       (vm_region_info_t)&top,
			       &info_count, &object_name))
			break;

		if (address >= TEXT_SEGMENT_START && address < DATA_SEGMENT_END) {
			if (!split && top.share_mode == SM_EMPTY) {
				if (basic.reserved)
					split = 1;
			}
			if (top.share_mode != SM_PRIVATE) {
				address += size;
				continue;
			}
		}

		switch (top.share_mode) {
		case SM_COW:
			if (top.ref_count == 1) {
				private += top.private_pages_resident * vm_page_size;
				private += top.shared_pages_resident * vm_page_size;
				vprivate += size;
			} else {
				shared_table_register (server, stable, &top, size);
				vprivate += top.private_pages_resident * vm_page_size;
			}
			break;
		case SM_PRIVATE:
			private += top.private_pages_resident * vm_page_size;
			vprivate += size;
			break;
		case SM_SHARED:
			shared_table_register (server, stable, &top, size);
			break;
		}

		address += size;
	}
	glibtop_suid_leave (server);

	/* !!! END OF SUID ROOT PART !!! */

	for (i = 0; i < SHARED_TABLE_SIZE; i++) {
		shared_info *sinfo = &stable[i];

		while (sinfo) {
			if (sinfo->share_mode == SM_SHARED &&
			    sinfo->ref_count == sinfo->task_ref_count) {
				private += sinfo->page_count * vm_page_size;
				vprivate += sinfo->size;
			} else {
				shared += sinfo->page_count * vm_page_size;
			}
			sinfo = sinfo->next;
		}
	}
	shared_table_free (server, stable);

	if (split)
		vsize -= DATA_SEGMENT_END - TEXT_SEGMENT_START;

	buf->size     = vprivate;
	buf->vsize    = vsize;
	buf->resident = resident;
	buf->share    = shared;
	buf->rss      = private;
	buf->flags    = _glibtop_sysdeps_proc_mem;
}
