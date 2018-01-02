#include "dev_handles.h"
#include <glib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include "net_process.h"
#include "stats.h"
#include <stdlib.h>
#include "netsockets.h"

static time_t last_refresh_time = 0;
GHashTable *inode_table ;
GHashTable *hash_table ;
Net_process_list *processes = g_slice_new(Net_process_list);//global process list
Net_process *unknwownTCP = g_slice_new(Net_process);

int 
size(Net_process_list *plist)
{
	int i = 1;
	if (plist->next != NULL)
		i += size(plist->next);
	return i;
}
void
process_init()
{
	Net_process_init(unknwownTCP, 0, "", "unknwownTCP");
	Net_process_list_init(processes, unknwownTCP, NULL);
}

void 
do_refresh()
{	g_hash_table_destroy(inode_table);
	g_hash_table_destroy(hash_table);
	inode_table = g_hash_table_new(g_direct_hash, g_direct_equal);
	hash_table = g_hash_table_new(g_str_hash, g_str_equal);
	
	char *fname = g_strdup("/proc/net/tcp");
	glibtop_socket *socket_list = glibtop_get_netsockets (fname, inode_table, hash_table);
	g_free(fname);

	handles_set_hash(inode_table, hash_table);

	Net_process_list *curproc = processes;
	int nproc = size(processes);
	stat_entry *st = (stat_entry *)calloc(nproc, sizeof(stat_entry));
	int n = 0;
	while(curproc != NULL)
	{
		g_assert(Net_process_list_get_proc(curproc) != NULL);
		float value_sent = 0;
		float value_recv = 0;
		Net_process_get_kbps(Net_process_list_get_proc(curproc), &value_recv, &value_sent, get_curtime());
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
}

int main()
{	
	char *fname = g_strdup("/proc/net/tcp");
	inode_table = g_hash_table_new(g_direct_hash, g_direct_equal);
	hash_table = g_hash_table_new(g_str_hash, g_str_equal);
	glibtop_socket *socket_list = glibtop_get_netsockets (fname, inode_table, hash_table);
	g_free(fname);
	handles_set_hash(inode_table, hash_table);
	process_init();
	handles_set_process_lists(processes, unknwownTCP);
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
			
			if (current_handle->pcap_handle == NULL || g_strcmp0(current_handle->device_name,"lo") == 0 )
				continue;
			int retval = packet_dispatch(current_handle, 0, (u_char *)userdata, sizeof(packet_args))	;
			//printf will be later changed to  gerror
			if (retval < 0)
				printf("Error dispatching for device %s \n ", current_handle->device_name);
		}
		printf("Refreshing");
		time_t const now = time(NULL);
		if (last_refresh_time + refresh_delay <= now)
		{	
			last_refresh_time = now;
			g_slice_free(glibtop_socket, socket_list); //free the socket details struct 
			do_refresh();
		}
	}
	return 0;
}