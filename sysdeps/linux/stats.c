#include <stdio.h>
#include <glibtop/stats.h>
#include <stdlib.h>
#include <glibtop/dev_handles.h>
#include <glib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <glibtop/net_process.h>
#include <glibtop/netsockets.h>

static time_t last_refresh_time = 0;

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

void 
do_refresh()
{		
	char *fname = g_strdup("/proc/self/net/tcp");
	global_hashes test_hash = get_global_hashes_instance();
	test_hash.inode_table = NULL;
	test_hash.hash_table = NULL;
	test_hash = get_global_hashes_instance();
	glibtop_socket *socket_list = glibtop_get_netsockets (fname, test_hash.inode_table, test_hash.hash_table);
	g_free(fname);
	GSList *curproc = get_proc_list_instance(NULL);
	int nproc = g_slist_length(curproc);
	printf("no of proc:%d",nproc);
	GArray *network_stats_instance = network_stats_get_global_instance(NULL);	
	int n = 0;
	while (curproc != NULL)
	{
		g_assert(curproc->data != NULL);
		float value_sent = 0;
		float value_recv = 0;
		struct timeval t;
		t.tv_sec = 0;
		t = get_curtime(t);
		Net_process_get_kbps(Net_process_list_get_proc(curproc), &value_recv, &value_sent, t);
		uid_t uid = Net_process_list_get_proc(curproc)->uid;
		network_stats_entry temp_stats;	
		network_stats_init(&temp_stats, Net_process_list_get_proc(curproc)->proc_name,
						Net_process_list_get_proc(curproc)->device_name,
						value_recv,
						value_sent,
						Net_process_list_get_proc(curproc)->pid,
						Net_process_list_get_proc(curproc)->uid);
		network_stats_instance = g_array_prepend_val(network_stats_instance, temp_stats);
		network_stats_get_global_instance(network_stats_instance);
		curproc = curproc->next;
		n++;
	}
	network_stats_print_stat(network_stats_get_global_instance(NULL), nproc);
	printf("\n\n\n");
}

void 
glibtop_init_packet_capture ()
{
	char *fname = g_strdup("/proc/self/net/tcp");
	global_hashes test_hash = get_global_hashes_instance();
	glibtop_socket *socket_list = glibtop_get_netsockets (fname, test_hash.inode_table, test_hash.hash_table);
	g_free(fname);
	process_init();
	packet_handle *handles = open_pcap_handles();
	printf("\n PCAP HANDLES \n");
	print_pcap_handles(handles);
	printf("\nLocal Addresses\n");
	print_interface_local_address();
	packet_args *userdata = g_slice_new(packet_args); 
	while(1)
	{	
		//if cature status is true continue the capture else return 
		if (get_capture_status(FALSE))
		{
			for(packet_handle *current_handle = handles; current_handle != NULL; current_handle = current_handle->next)
			{	
				userdata->device = current_handle->device_name;
				userdata->sa_family = AF_UNSPEC;
				
				if (current_handle->pcap_handle == NULL)
					continue;
				int retval = packet_dispatch(current_handle, 0, (u_char *)userdata, sizeof(packet_args))	;
				//printf will be later changed to  gerror
				if (retval < 0)
					printf("Error dispatching for device %s \n ", current_handle->device_name);
			}
			time_t const now = time(NULL);
			if (last_refresh_time + refresh_delay <= now)
			{	
				last_refresh_time = now;
				//error in opening file free later//g_slice_free(glibtop_socket, socket_list); //free the socket details struct 
				do_refresh();
			}
		}
		else
			return ;
	}

}
