#include <glibtop/interface_local_addr.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <glib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //for inet_ntop
#include <stdio.h>

void 
local_addr_init(local_addr *laddr, in_addr_t addr_value, const char *device, local_addr *next_laddr)
{
	laddr->addr = addr_value;
	laddr->sa_family = AF_INET;
	char ip_buf[16];
	inet_ntop(AF_INET, &addr_value, ip_buf, 15 );	//copy the network part
	laddr->ip_text = g_strdup(ip_buf);
	laddr->device_name = g_strdup(device);
	laddr->next = next_laddr;
}

void 
local_addr6_init(local_addr *laddr, struct in6_addr addr6_value, const char *device, local_addr *next_laddr)
{
	laddr->addr6 = addr6_value;
	laddr->sa_family = AF_INET;
	char ip_buf[128];
	inet_ntop(AF_INET6, &addr6_value, ip_buf, 128 );	//copy the network part
	laddr->ip_text = g_strdup(ip_buf);
	laddr->device_name = g_strdup(device);
	laddr->next = next_laddr;
}

gboolean
local_addr_contains(local_addr *laddr, const in_addr_t *n_addr)
{	
	if((laddr->sa_family == AF_INET) && (laddr->addr == *n_addr))
		return TRUE;
	if(laddr->next != NULL && laddr->device_name != NULL)
		return local_addr_contains(laddr->next, n_addr);
	return FALSE;
}

gboolean
local_addr6_contains(local_addr *laddr, const struct in6_addr *n_addr)
{
	if (((laddr->sa_family) == AF_INET) && ((laddr->addr6).s6_addr == n_addr->s6_addr))
		return TRUE;
	if(laddr->next != NULL && laddr->device_name != NULL)
		return local_addr6_contains(laddr->next, n_addr);
	return FALSE;
}

local_addr*
get_device_local_addr(const char *device)
{
	local_addr *interface_local_addr;
	struct ifaddrs *ifaddr, *ifa;
	if(getifaddrs(&ifaddr) == -1)
	{
		return NULL;
	}
	for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		interface_local_addr = get_local_addr_instance(NULL);
		if(ifa->ifa_addr == NULL)
			continue;
		if(g_strcmp0(ifa->ifa_name, device) !=0)
			continue;
		
		int family = ifa->ifa_addr->sa_family;
		if(family == AF_INET)
		{
			struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
			local_addr *temp = g_slice_new(local_addr);
			local_addr_init(temp, addr->sin_addr.s_addr, device, interface_local_addr);
			get_local_addr_instance(temp); //set global static var of local addr with temp
			printf("Adding local address: %s\n", inet_ntoa(addr->sin_addr)); //just for debugging
		}
		else if(family == AF_INET6)
		{
			struct sockaddr_in6 *addr = (struct sockaddr_in6 *)ifa->ifa_addr;
			local_addr *temp = g_slice_new(local_addr);
			local_addr6_init(temp, addr->sin6_addr, device, interface_local_addr); //CHECK THIS
			get_local_addr_instance(temp);
			char host[128];
			printf("Adding local address: %s\n",inet_ntop(AF_INET6, &addr->sin6_addr, host, sizeof(host))); //just for debugging
		}
	}
	return get_local_addr_instance(NULL);
}

local_addr *
get_local_addr_instance(local_addr *val)
{
	static local_addr *temp_if_addr = NULL;
	if (val != NULL)
		temp_if_addr = val;
	else if (temp_if_addr == NULL)
		{
			temp_if_addr = g_slice_new(local_addr);
			temp_if_addr->device_name = NULL;
		}
	return temp_if_addr;
}