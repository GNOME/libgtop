#include "netsockets.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include "proc_inode_parser.h"
#include <glibtop/procstate.h>

gpointer 
match_pid(int inode, GHashTable *inode_table)
{
	if(g_hash_table_contains(inode_table, (gconstpointer *)&inode))
	{
		gpointer pid_ptr = g_hash_table_lookup(inode_table,(gconstpointer *)&inode);
	    printf("%d\n", *(int *)pid_ptr );
        return pid_ptr;
    }
	return NULL;
}

glibtop_socket *
add_socket_list(char *buf, glibtop_socket *list_socket,GHashTable *inode_table)
{	
	char temp_local_addr[128];
	char temp_rem_addr[128];
	glibtop_socket *temp_socket = (glibtop_socket *)malloc(sizeof(glibtop_socket));
	temp_socket->local_addr = (struct in6_addr *)malloc(sizeof(in6_addr));
    temp_socket->rem_addr = (struct in6_addr *)malloc(sizeof(in6_addr));
    
    if(list_socket!=NULL)
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
    if(matches !=5)
	{
		fprintf(stderr, "Invalid buf\n");
		exit(0);
	}  
    gpointer pid_return = match_pid(temp_socket->inode, inode_table);
    
    if(pid_return)
    {   
        temp_socket->pid =*(pid_t *)pid_return;
        glibtop_proc_state *buf = (glibtop_proc_state *)malloc(sizeof(glibtop_proc_state));
        glibtop_get_proc_state(buf,temp_socket->pid);
        temp_socket->proc_name = buf->cmd;
    }
    if(strlen(temp_local_addr) >8) //it is IPv6
	{
		sscanf(temp_local_addr, "%08X%08X%08X%08X",
					&((temp_socket->local_addr)->s6_addr32[0]),
				    &((temp_socket->local_addr)->s6_addr32[1]),
					&((temp_socket->local_addr)->s6_addr32[2]),
					&((temp_socket->local_addr)->s6_addr32[3]));
		sscanf(temp_rem_addr, "%08X%08X%08X%08X",
					&((temp_socket->rem_addr)->s6_addr32[0]),
					&((temp_socket->rem_addr)->s6_addr32[1]),
					&((temp_socket->rem_addr)->s6_addr32[2]),
					&((temp_socket->rem_addr)->s6_addr32[3]));
		temp_socket->sa_family = AF_INET6;

	}    
    else //it is IPv4
	{	
		    sscanf(temp_local_addr, "%X", (unsigned int *)&(*(temp_socket->local_addr)));
			sscanf(temp_rem_addr, "%X", (unsigned int *)&(*(temp_socket->rem_addr)));
			temp_socket->sa_family = AF_INET;
	}     
    temp_socket->next = NULL;
    return temp_socket;
}

glibtop_socket*
glibtop_get_netsockets (char *filename,GHashTable *inode_table)
{   
	char line[8192] ;
	traverse_file("/proc",0,"",inode_table);
	FILE *fd = fopen(filename, "r");

	if(fd == NULL)
	{
		fprintf(stderr, "Error opening file : %s",filename);
		return NULL;
	}
    //removing the file header
    fgets(line, sizeof(line),fd);
    //to initialise socket_list with the first entry  
    fgets(line, sizeof(line),fd);
    glibtop_socket *socket_list=NULL;
    socket_list = add_socket_list(line,socket_list,inode_table);
    glibtop_socket *temp_socket_list = socket_list;
    glibtop_socket *next_socket = temp_socket_list;
	while(!feof(fd)){	
        fgets(line, sizeof(line),fd);
		next_socket = add_socket_list(line,next_socket,inode_table);
	}
	return socket_list;
}