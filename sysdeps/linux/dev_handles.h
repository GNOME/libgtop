#ifndef __DEV_HANDLES_H__
#define __DEV_HANDLES_H__

#include <glib.h>
#include <pcap.h>
#include <netinet/in.h>
G_BEGIN_DECLS

#define IF_HANDLE 1
#define IF_HANDLE_FUNC 19
typedef struct _packet_handle packet_handle;
//for callbacks
typedef int (*packet_callback)(u_char* , const pcap_pkthdr*, const u_char *);
enum packet_type
{
	packet_ip,
	packet_ip6,
	packet_tcp,
	packet_udp,
	no_of_packet_types
};

struct  _packet_handle
{	char *device_name;
	pcap_t *pcap_handle;
	int linktype;
	packet_callback callback[no_of_packet_types];
	packet_handle *next;
};

typedef struct _packet_args packet_args;
struct _packet_args
{
	const char* device;
	int sa_family;
	in_addr ip_src;
	in_addr ip_dst;
	in6_addr ip6_src;
	in6_addr ip6_dst;
};

packet_handle * open_pcap_handles();
void print_pcap_handles(packet_handle *handle);
void print_interface_local_address();
local_addr *get_if_local_addr();
G_END_DECLS

#endif