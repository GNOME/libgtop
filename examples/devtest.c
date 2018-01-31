#include <glibtop/dev_handles.h>
#include <glib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <glibtop/net_process.h>
#include <glibtop/stats.h>
#include <stdlib.h>
#include <glibtop/netsockets.h>
#include <stdio.h>

static time_t last_refresh_time = 0;
/*
int 
size(GSList *plist)
{	
	int i = 1;
	if (plist != NULL)
		{
			i += size(plist->next);
		}
	return i;
}*/

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
	stat_entry *st = (stat_entry *)calloc(nproc, sizeof(stat_entry));
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
		stat_init(&st[n], Net_process_list_get_proc(curproc)->proc_name,
						Net_process_list_get_proc(curproc)->device_name,
						value_recv,
						value_sent,
						Net_process_list_get_proc(curproc)->pid,
						Net_process_list_get_proc(curproc)->uid);
		curproc = curproc->next;
		n++;
	}
	print_stat(st, nproc);
	printf("\n\n\n");
}

int main()
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
	return 0;
}