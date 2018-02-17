#include <glibtop/netsockets.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <glibtop/proc_inode_parser.h>
#include <glibtop/procstate.h>
#include <glib.h>
#include <arpa/inet.h>
#include <errno.h>

gint 
match_pid(int inode)
{	
	GHashTable *inode_table = get_global_hashes_instance().inode_table;
	if (g_hash_table_lookup(inode_table, GINT_TO_POINTER(inode)) != NULL)
	{
		return GPOINTER_TO_INT(g_hash_table_lookup(inode_table, GINT_TO_POINTER(inode)));
	}
	return -1;
}

void 
print_table(gpointer key, gpointer value, gpointer user_data)
{
	printf("%s :: %d\n", (char *)key, GPOINTER_TO_INT(value));
}

void 
print_hash(GHashTable *inode_table)
{
	g_hash_table_foreach(inode_table, (GHFunc)print_table, NULL);
}

gint
match_hash_to_inode(char *hash)
{
	GHashTable *hash_table = get_global_hashes_instance().hash_table;
	print_hash(hash_table);
	if (g_hash_table_lookup(hash_table, hash) != NULL)
	{	
		return GPOINTER_TO_INT(g_hash_table_lookup(hash_table, hash));
	}
	return -1;
}

glibtop_socket *
add_socket_list(char *buf, glibtop_socket *list_socket, GHashTable *inode_table, GHashTable *hash_table)
{
	char temp_local_addr[128];
	char temp_rem_addr[128];
	glibtop_socket *temp_socket = g_slice_new(glibtop_socket);
	temp_socket->laddr = g_slice_new(struct in6_addr);
	temp_socket->rem_addr = g_slice_new(struct in6_addr);
	char temp_hash[92];

	if (list_socket != NULL)
		list_socket->next = temp_socket;
	else
		list_socket = temp_socket;

	int matches = sscanf(buf, "%*X: %64[0-9A-Fa-f]:%X %64[0-9A-Fa-f]:%X %*X "
							"%*X:%*X %*X:%*X %*X %*d %*d %ld %*512s\n",
							temp_local_addr,
							&(temp_socket->local_port),
							temp_rem_addr,
							&(temp_socket->rem_port),
							&(temp_socket->inode));
	if (matches != 5)
	{
		fprintf(stderr, "Invalid buf\n");
		exit(0);
	}

	temp_socket->pid = match_pid(temp_socket->inode);
	glibtop_proc_state *proc_buf = g_slice_new(glibtop_proc_state);
	glibtop_get_proc_state(proc_buf, temp_socket->pid);
	temp_socket->proc_name = proc_buf->cmd;

	if (strlen(temp_local_addr) > 8) //it is IPv6
	{
		sscanf(temp_local_addr, "%08X%08X%08X%08X",
					&((temp_socket->laddr)->s6_addr32[0]),
					&((temp_socket->laddr)->s6_addr32[1]),
					&((temp_socket->laddr)->s6_addr32[2]),
					&((temp_socket->laddr)->s6_addr32[3]));
		sscanf(temp_rem_addr, "%08X%08X%08X%08X",
					&((temp_socket->rem_addr)->s6_addr32[0]),
					&((temp_socket->rem_addr)->s6_addr32[1]),
					&((temp_socket->rem_addr)->s6_addr32[2]),
					&((temp_socket->rem_addr)->s6_addr32[3]));
		temp_socket->sa_family = AF_INET6;
		snprintf(temp_hash, HASHKEYSIZE * sizeof(char), "%s:%d-%s:%d", temp_local_addr, temp_socket->local_port, temp_rem_addr, temp_socket->rem_port);
		char lip[50];
		char rip[50];
		inet_ntop(AF_INET6, temp_local_addr, lip, sizeof(lip));
		inet_ntop(AF_INET6,temp_rem_addr, rip, sizeof(rip));
		//snprintf(temp_hash, HASHKEYSIZE * sizeof(char), "%s:%d-%s:%d", lip, temp_socket->local_port, rip, temp_socket->rem_port);
	}
	else//it is IPv4
	{		
		sscanf(temp_local_addr, "%X", (unsigned int *)&(*(temp_socket->laddr)));
		sscanf(temp_rem_addr, "%X", (unsigned int *)&(*(temp_socket->rem_addr)));
		temp_socket->sa_family = AF_INET;
		char lip[128];
		char rip[128];
		inet_ntop(AF_INET, &(*(temp_socket->laddr)), lip, sizeof(lip));
		inet_ntop(AF_INET, &(*(temp_socket->rem_addr)), rip, sizeof(rip));
		snprintf(temp_hash, HASHKEYSIZE * sizeof(char), "%s:%d-%s:%d", lip, temp_socket->local_port, rip, temp_socket->rem_port);
		//snprintf(temp_hash, HASHKEYSIZE * sizeof(char), "%s-%s", lip, rip);
	}
	temp_socket->sock_hash = g_strdup(temp_hash);
	g_hash_table_insert(hash_table, temp_socket->sock_hash, GINT_TO_POINTER(temp_socket->inode));
	temp_socket->next = NULL;
	return temp_socket;
}

glibtop_socket*
glibtop_get_netsockets (char *filename, GHashTable *inode_table, GHashTable *hash_table)
{
	char line[8192];
	traverse_file(g_file_new_for_path("/proc"), 0, "", inode_table);
	FILE *fd = fopen(filename, "r");

	if (fd == NULL)
	{	
		fprintf(stderr, "Error opening file : %s", filename);
		printf("%d\n",errno );
		return NULL;
	}
	//removing the file header
	fgets(line, sizeof(line), fd);
	//to initialise socket_list with the first entry  
	fgets(line, sizeof(line), fd);
	glibtop_socket *socket_list = NULL;
	socket_list = add_socket_list(line, socket_list, inode_table, hash_table);
	glibtop_socket *temp_socket_list = socket_list;
	glibtop_socket *next_socket = temp_socket_list;
	while (!feof(fd))
	{
		fgets(line, sizeof(line), fd);
		next_socket = add_socket_list(line, next_socket, inode_table, hash_table);
	}
	fclose(fd);
	return socket_list;
}

void
global_hashes_init(global_hashes *gh)
{	
	if(gh->inode_table == NULL)
		gh->inode_table  = g_hash_table_new(g_direct_hash, g_direct_equal);
	if(gh->hash_table == NULL)
		gh->hash_table = g_hash_table_new(g_str_hash, g_str_equal);
}

global_hashes 
get_global_hashes_instance()
{
	static global_hashes gh_temp = {NULL,NULL};
	if(gh_temp.inode_table == NULL || gh_temp.hash_table == NULL)
		global_hashes_init(&gh_temp);
	return gh_temp;
}
