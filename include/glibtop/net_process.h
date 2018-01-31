#ifndef __GLIBTOP_NET_PROCESS_H_
#define __GLIBTOP_NET_PROCESS_H_

#include <glib.h>
#include <sys/types.h>
#include <glibtop/connection.h>
G_BEGIN_DECLS

#define CONNTIMEOUT 50
#define PERIOD 5

typedef struct _Net_process Net_process;
struct _Net_process
{
	unsigned long inode;
	uid_t uid;
	char *proc_name;
	char *device_name;
	pid_t pid;
	u_int64_t bytes_sent;
	u_int64_t bytes_recv;
	GSList *proc_connections; 
};

typedef struct _Net_process_list Net_process_list;
struct _Net_process_list
{
	Net_process *val;
	Net_process_list *next;
};

void Net_process_init(Net_process *proc, unsigned long pid, char *device_name_val, char *proc_name_val);
int Net_process_get_last_packet_time(Net_process *proc);
void Net_process_get_total(Net_process *proc, u_int64_t *recvd, u_int64_t *sent);
float to_kb(u_int64_t bytes);
float to_kbps(u_int64_t bytes);
void Net_process_get_kbps(Net_process *proc, float *recvd, float *sent, struct timeval currtime);
uid_t Net_process_get_uid(Net_process *proc);
GSList *Net_process_list_init(GSList *plist, Net_process *proc);
Net_process *Net_process_list_get_proc(GSList *plist);
GSList *get_proc_list_instance(GSList *val);
Net_process *get_unknown_proc_instance(Net_process *val);
G_END_DECLS
#endif
