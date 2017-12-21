#include <glibtop/netlist.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include "dev_handles.h"
#include "packet.h"
#include <netinet/tcp.h>
#include <sys/time.h>

int promisc = 0;
char errbuf[PCAP_ERRBUF_SIZE];
timeval curtime;

int 
process_ip(u_char *userdata, const pcap_pkthdr * /* header */, const u_char *m_packet) 
{
	/*TWIP*/
	return 0;
}
int 
process_ip6(u_char *userdata, const pcap_pkthdr * /* header */, const u_char *m_packet) 
{
	/*WIP*/
	return 0;
}
int 
process_tcp(u_char *userdata, const pcap_pkthdr *header /* header */, const u_char *m_packet) 
{	/*WIP*/
	packet_args *args = (packet_args *)userdata;
	struct tcphdr *tcp = (struct tcphdr *)m_packet;

	curtime = header->ts;
	Packet *packet = (Packet *)malloc(sizeof(Packet));
/*	switch(args->sa_family)
	{
	case AF_INET:
		Packet_init_in_addr(packet, args->ip_src, ntohs(tcp->th_sport), args->ip_dst, ntohs(tcp->th_dport), header->len, header->ts);
		break;

	case AF_INET6:
		Packet_init_in6_addr(packet, args->ip6_src, ntohs(tcp->th_sport), args->ip6_dst, ntohs(tcp->th_dport), header->len, header->ts);
		break;
	}
*/
}

void 
add_callback(packet_handle *handle, enum packet_type type, packet_callback callback)
{
	handle->callback[type] = callback ;
}

packet_handle*
get_interface_handle(char *device, GError **err)
{
	pcap_t *temp = pcap_open_live(device, BUFSIZ, promisc, 100, errbuf);
	if (temp == NULL)
	{
		g_set_error(err,
					IF_HANDLE,
					IF_HANDLE_FUNC,
					"failed to open handle for device : %s",
					device);
		return NULL;
	}

	packet_handle *temp_packet_handle = (packet_handle *)malloc(sizeof(packet_handle));
	if(temp_packet_handle != NULL)
	{	
		temp_packet_handle->pcap_handle = temp;
		temp_packet_handle->device_name = device;
		temp_packet_handle->next = NULL;
		temp_packet_handle->linktype = pcap_datalink(temp);
		for(int i = 0; i < no_of_packet_types; i++)
		{
			temp_packet_handle->callback[i] = NULL;
		}
		return temp_packet_handle;
	}
	return NULL;
}
packet_handle * 
open_pcap_handles()
{	glibtop_netlist buf;
	char **devices;
	devices = glibtop_get_netlist (&buf);
	GError **if_error;
	int count=0;
	packet_handle *previous_handle=NULL , *initial_handle = NULL;
	int init_ele = 1 ; 
	while (count < buf.number) {
		packet_handle *new_handle = get_interface_handle(devices[count], if_error);
		if (new_handle != NULL)
		{	if (init_ele)
			{
				initial_handle = new_handle;
				init_ele = 0;
			}
			add_callback(new_handle, packet_ip, process_ip);
			add_callback(new_handle, packet_ip6, process_ip6);
			add_callback(new_handle, packet_tcp, process_tcp);
			if (previous_handle != NULL)
				previous_handle->next = new_handle;
			previous_handle = new_handle;
		}	
		count++;
	}
	return initial_handle;

}
void 
print_pcap_handles(packet_handle *handle)
{
	glibtop_netlist buf;
	char **devices;
	devices = glibtop_get_netlist (&buf);
	int count=0;
	packet_handle *temp_handle = handle;
	while(temp_handle != NULL)
	{
		printf("device name : %s linktype: %d \n ", temp_handle->device_name, temp_handle->linktype);
		temp_handle = temp_handle->next;
	}	
}

