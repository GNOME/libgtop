#include <stdio.h>
#include <glibtop/stats.h>
#include <stdlib.h>

void 
network_stats_init(network_stats_entry *st, char *proc_name, char *device_name, double recv_value, double sent_value, pid_t pid, uid_t uid)
{
	st->proc_name = proc_name;
	st->device_name = device_name;
	st->pid = pid;
	st->uid = uid;
	st->sent_value = sent_value;
	st->recv_value = recv_value;
	g_assert(pid >= 0);
}

void
network_stats_print_entry(network_stats_entry *st)
{
	printf("pid :%d \t sent_value:%f \trecv value:%f \tname of process:%s device_name:%s\n", st->pid, st->sent_value, st->recv_value, st->proc_name, st->device_name);
}

void
network_stats_print_stat(GArray *stats,int nproc)
{	GArray *list = stats;
	for(int index = 0; index < nproc; index++)
	{	
		network_stats_entry temp = g_array_index (stats, network_stats_entry, index);
		network_stats_print_entry(&temp);
		if(stats->len>index	)
		stats = g_array_remove_index(stats, index);
	}
	network_stats_get_global_instance(stats);
}
GArray *
network_stats_get_global_instance(GArray *val)
{
	static GArray *global_stats = NULL;
	if (val != NULL)
		{	
			global_stats = val;
		}
	else	if (global_stats == NULL)
	{	
		global_stats = g_array_new (TRUE,FALSE,sizeof(network_stats_entry));
	}			
	return global_stats;
}


