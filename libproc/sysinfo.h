#ifndef SYSINFO_H
#define SYSINFO_H

int        loadavg(double *av1, double *av5, double *av15);
int        uptime (double *uptime_secs, double *idle_secs);
unsigned** meminfo(void);

enum meminfo_row { meminfo_main = 0,
		   meminfo_swap };

enum meminfo_col { meminfo_total = 0, meminfo_used, meminfo_free,
		   meminfo_shared, meminfo_buffers, meminfo_cached
};

unsigned read_total_main(void);

#endif /* SYSINFO_H */
