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

stats *
get_stats_instance(void )
{
	static stats *temp_stats = NULL;
	if(temp_stats == NULL)
	{
		temp_stats = g_slice_new(stats);
		temp_stats->pid=0;
		temp_stats->bytes_recv = 0;
		temp_stats->bytes_sent = 0;
		temp_stats->process_name = g_strdup("default");
	}
    return temp_stats;
}

void
invert_capture_status(gboolean *status_ptr)
{
	*status_ptr = (*status_ptr == FALSE)? TRUE : FALSE;
}

/**
*NOTE (DBus): this function is used to change the capture status when a relevant
*	   		  signal is processed by the callback in the dbus interface.
*      		  source of signal emission is any application which tries to consume
*      		  the libgtop API through the DBus
*
*NOTE (Daemon): if later a daemon is proposed to be used over DBus this won't be
*	   			an issue since by default status is kept to be active
*/
gboolean
get_capture_status(gboolean val)
{
	static gboolean active = TRUE;
	//if val is true invert the capture status
	//if false capture status is unchanged
	if(val)
		invert_capture_status(&active);
	return active;
}