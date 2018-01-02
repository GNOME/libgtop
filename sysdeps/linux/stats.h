#ifndef __STATS_H_
#define __STATS_H_

#include <glib.h>

G_BEGIN_DECLS

typedef struct _stat_entry stat_entry;
struct _stat_entry
{
	char *proc_name;
	char *device_name;
	pid_t pid;
	uid_t uid;
	double sent_value;
	double recv_value;
};
void stat_init(stat_entry *st, char *proc_name, char *device_name, double recv_value, double sent_value, pid_t pid, uid_t uid);
void print_stat(stat_entry *st, int nproc);

G_END_DECLS

#endif