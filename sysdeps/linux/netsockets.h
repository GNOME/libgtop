#ifndef __NETSOCKETS_H__
#define __NETSOCKETS_H__

#include <glib.h>
#include <sys/types.h>
#include <netinet/in.h>

G_BEGIN_DECLS

#define HASHKEYSIZE 92
typedef struct _glibtop_socket glibtop_socket;
struct _glibtop_socket
{
	char *proc_name;
	pid_t pid;
	unsigned long inode;
	in6_addr *rem_addr;
	in6_addr *local_addr;
	int local_port;
	int rem_port;
	int sa_family;
	char *sock_hash;
	glibtop_socket *next;

};

glibtop_socket* glibtop_get_netsockets (char *filename, GHashTable *inode_table, GHashTable *hash_table);
gint match_hash_to_inode(char *hash, GHashTable *hash_table);
G_END_DECLS
#endif

