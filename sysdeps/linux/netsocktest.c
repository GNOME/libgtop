#include "netsockets.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <glib.h>

int main()
{	
	global_hashes inode_hash = get_global_hashes_instance();
	char *fname = g_strdup("/proc/net/tcp");
	glibtop_socket *socket_list = glibtop_get_netsockets (fname, inode_hash.inode_table, inode_hash.hash_table);
	while (socket_list != NULL) {
		char lip[128];
		char rip[128];
		inet_ntop(AF_INET, &(*(socket_list->local_addr)), lip, sizeof(lip));
		inet_ntop(AF_INET, &(*(socket_list->rem_addr)), rip, sizeof(rip));
		printf("proc_name:%s inode: %lu pid: %d local_addr: %s rem_addr: %s local_port: %d rem_port: %d sa_family: %d \n\n",
						socket_list->proc_name,
						socket_list->inode,
						socket_list->pid,  
						lip,
						rip,
						socket_list->local_port,
						socket_list->rem_port,
						socket_list->sa_family);
		socket_list = socket_list->next;
	}
	return 0;
}

