#ifndef __GLIBTOP_STATS_H_
#define __GLIBTOP_STATS_H_

#include <glib.h>

G_BEGIN_DECLS

typedef struct _network_stats_entry network_stats_entry;
struct _network_stats_entry
{
	char *proc_name;
	char *device_name;
	pid_t pid;
	uid_t uid;
	double sent_value;
	double recv_value;
};
void network_stats_init(network_stats_entry *st, char *proc_name, char *device_name, double recv_value, double sent_value, pid_t pid, uid_t uid);
void network_stats_print_stat(GArray *stat, int nproc);
GArray *network_stats_get_global_instance(GArray *val);

G_END_DECLS

#endif
