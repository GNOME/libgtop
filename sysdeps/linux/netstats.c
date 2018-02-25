#include <config.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/dev_handles.h>
#include <glib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <glibtop/net_process.h>
#include <glibtop/stats.h>
#include <glibtop/netsockets.h>
#include <stdio.h>
#include <glibtop/netstats.h>

static const unsigned long _glibtop_sysdeps_netstats = (1 << GLIBTOP_NETSTATS_NUMBER);
static time_t last_refresh_time = 0;
void
_glibtop_init_netstats_s (glibtop *server)
{	//sysdeps.h
	server->sysdeps.netstats = _glibtop_sysdeps_netstats;
	glibtop_init_packet_capture_s ();
}

GArray *
network_stats_get_global_instance_s(glibtop *server, GArray *val)
{
	return network_stats_get_global_instance(NULL);
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
glibtop_init_packet_capture_s ()
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

