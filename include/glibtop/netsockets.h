#ifndef __GLIBTOP_NETSOCKETS_H__
#define __GLIBTOP_NETSOCKETS_H__

#include <glib.h>
#include <sys/types.h>
#include <netinet/in.h>

G_BEGIN_DECLS

#define HASHKEYSIZE 92
typedef struct _global_hashes global_hashes;
struct _global_hashes
{
	GHashTable *inode_table;
	GHashTable *hash_table;
};

global_hashes get_global_hashes_instance();


typedef struct _glibtop_socket glibtop_socket;
struct _glibtop_socket
{
	char *proc_name;
	pid_t pid;
	unsigned long inode;
	struct in6_addr *rem_addr;
	struct in6_addr *laddr;
	int local_port;
	int rem_port;
	int sa_family;
	char *sock_hash;
	glibtop_socket *next;

};

glibtop_socket* glibtop_get_netsockets (char *filename, GHashTable *inode_table, GHashTable *hash_table);
gint match_hash_to_inode(char *hash);
gint match_pid(int inode);

G_END_DECLS
#endif

