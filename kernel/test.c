#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <linux/unistd.h>
#include <linux/table.h>

#include <syscall.h>

static inline _syscall3 (int, table, int, type, union table *, tbl, const void *, param);

int
main (void)
{
	union table tbl;
	unsigned count;
	int ret;

	ret = table (TABLE_VERSION, NULL, NULL);

	if (ret == -1) {
		fprintf (stderr, "table(%u): %s\n", TABLE_VERSION, sys_errlist [errno]);
		exit (-errno);
	}

	fprintf (stderr, "Table (%u) = %u\n", TABLE_VERSION, ret);

	for (count = 0; count < 5; count++) {
		ret = table (TABLE_CPU, &tbl, NULL);

		if (ret == -1) {
			fprintf (stderr, "table(%u): %s\n", TABLE_CPU, sys_errlist [errno]);
			exit (-errno);
		}

		fprintf (stderr, "Table (%u) = %lu, %lu, %lu, %lu, %lu, %lu\n",
			 TABLE_CPU, tbl.cpu.total, tbl.cpu.user, tbl.cpu.nice,
			 tbl.cpu.sys, tbl.cpu.idle, tbl.cpu.frequency);
	}

	ret = table (TABLE_MEM, &tbl, NULL);

	if (ret == -1) {
		fprintf (stderr, "table(%u): %s\n", TABLE_MEM, sys_errlist [errno]);
		exit (-errno);
	}

	fprintf (stderr, "Table (%u) = %lu, %lu, %lu, %lu, %lu, %lu\n",
		 TABLE_MEM, tbl.mem.total, tbl.mem.used, tbl.mem.free,
		 tbl.mem.shared, tbl.mem.buffer, tbl.mem.cached);

	ret = table (TABLE_SWAP, &tbl, NULL);

	if (ret == -1) {
		fprintf (stderr, "table(%u): %s\n", TABLE_SWAP, sys_errlist [errno]);
		exit (-errno);
	}

	fprintf (stderr, "Table (%u) = %lu, %lu, %lu\n",
		 TABLE_SWAP, tbl.swap.total, tbl.swap.used, tbl.swap.free);
	
	ret = table (TABLE_LOADAVG, &tbl, NULL);

	if (ret == -1) {
		fprintf (stderr, "table(%u): %s\n", TABLE_LOADAVG, sys_errlist [errno]);
		exit (-errno);
	}

	fprintf (stderr, "Table (%u) = (%lu, %lu, %lu) - %u, %u, %u\n",
		 TABLE_LOADAVG, tbl.loadavg.loadavg [0], tbl.loadavg.loadavg [1],
		 tbl.loadavg.loadavg [2], tbl.loadavg.nr_running,
		 tbl.loadavg.nr_tasks, tbl.loadavg.last_pid);
	
	ret = table (TABLE_UPTIME, &tbl, NULL);

	if (ret == -1) {
		fprintf (stderr, "table(%u): %s\n", TABLE_UPTIME, sys_errlist [errno]);
		exit (-errno);
	}

	fprintf (stderr, "Table (%u) = %lu, %lu\n",
		 TABLE_UPTIME, tbl.uptime.uptime, tbl.uptime.idle);
	
	exit (0);
}
