#ifndef __GLIBTOP_INTERFACE_LOCAL_ADDR_H__
#define __GLIBTOP_INTERFACE_LOCAL_ADDR_H__

#include <sys/types.h>
#include <ifaddrs.h>
#include <glib.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef  struct _local_addr local_addr;
struct _local_addr
{
	in_addr_t addr; //ip in network Byte order
	struct in6_addr addr6;
	short int sa_family; 
	char *ip_text; //to store ip in text form
	char *device_name;
	local_addr *next;
};

void 		local_addr_init(local_addr *laddr, in_addr_t addr_value, const char *device, local_addr *next_laddr);
void 		local_addr6_init(local_addr *laddr, struct in6_addr addr6_value, const char *device, local_addr *next_laddr);
gboolean 	local_addr_contains(local_addr *laddr, const in_addr_t *n_addr);
gboolean 	local_addr6_contains(local_addr *laddr, const struct in6_addr *n_addr);
local_addr	*get_device_local_addr(const char *device);
local_addr  *get_local_addr_instance(local_addr *val);



#endif
