#ifndef __GLIBTOP_STATS_H_
#define __GLIBTOP_STATS_H_

#include <glib.h>

G_BEGIN_DECLS

typedef struct _network_stats_entry network_stats_entry;
struct _network_stats_entry
{
	//char *device_name;
	pid_t pid;
	//uid_t uid;
	guint sent_value;
	guint recv_value;
};

typedef struct _netstats stats;
struct  _netstats
{
	guint pid;
	guint bytes_sent;
	guint bytes_recv;
};


void network_stats_init(network_stats_entry *st, guint recv_value, guint sent_value, pid_t pid);
void network_stats_print_stat(GArray *stat, int nproc);
GArray *network_stats_get_global_instance(GArray *val);
gboolean get_capture_status(gboolean val);
void invert_capture_status(gboolean *status_ptr);
GPtrArray *get_stats_instance(GPtrArray *val);
gboolean glibtop_init_packet_capture(void);
void init_setup(void);
G_END_DECLS

#endif
