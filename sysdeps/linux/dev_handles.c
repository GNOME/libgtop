#include <glibtop/netlist.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include "dev_handles.h"
#include "packet.h"
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <sys/time.h>
#include "interface_local_addr.h"
#include "connection.h"
#include "process.h"
#include "netsockets.c"
#include <glib.h>
/*GLOBAL HASH TABLE */
local_addr *interface_local_addr;
int promisc = 0;
char errbuf[PCAP_ERRBUF_SIZE];
timeval curtime;
GHashTable *inode_table = g_hash_table_new(g_direct_hash, g_direct_equal);
GHashTable *hash_table = g_hash_table_new(g_str_hash, g_str_equal);

Net_process_list *processes;//global process list
Net_process *unknwownTCP;
void handles_set_hash(GHashTable *inode, GHashTable *hash)
{
	inode_table = inode;
	hash_table = hash;
}

void handles_set_process_lists(Net_process_list *procs, Net_process *tcp)
{
	processes = procs;
	unknwownTCP = tcp;
}

timeval
get_curtime()
{
	return curtime;
}
Net_process *get_process_from_inode(unsigned long inode, char *device_name)
{
	int pid = match_pid(inode, inode_table);
	Net_process_list *current = processes ;/*global list of all procs*/
	while (current != NULL)
	{
		Net_process *curr_process = Net_process_list_get_proc(current);
		g_assert(curr_process);
		if (pid == curr_process->pid)
			return curr_process;
		current = current->next;
	}
	return NULL;
}

Net_process *get_process(Connection *conn, char *device_name, 
						/*GHashTable *hash_inode_table pass the table which is mainained globally */,
 							/*GHashTable *inode_table pass the global table holding inode vs proc name*/)
{
	unsigned long inode = match_hash_to_inode(Packet_gethash(conn->ref_packet),hash_table);
	if (inode == -1)
	{
		Packet *reverse_pkt = get_inverted_packet(conn->ref_packet);
		inode = match_hash_to_inode(Packet_gethash(reverse_pkt), hash_table);
		if (inode == -1)
		{	g_slice_free(Packet, reverse_pkt);
			Conn_list *temp = g_slice_new(Conn_list);
			Conn_list_init(temp, conn, unknownTCP->proc_connections/*define this globally*/);
			unknownTCP->proc_connections = temp;
			return unknownTCP;
		}
		g_slice_free(Packet, conn->ref_packet);
		conn->ref_packet = reverse_pkt;
	}
	/*this proc is present in the hash table*/
	Net_process *proc = get_process_from_inode(inode, device_name, inode_table);
	if (proc == NULL)
	{
		proc = g_slice_new(Net_process);
		Net_process_init(proc, inode,"", Packet_gethash(conn->ref_packet));
		Net_process_list *temp = g_slice_new(Net_process_list);
		Net_process_list_init(temp, proc, processes/*global proc list*/);
		processes = temp;
	}
	Conn_list *temp_list = g_slice_new(Conn_list);
	Conn_list_init(temp_list, conn, proc->proc_connections);
	proc_connections = temp_list;

	return proc;
}
int 
process_ip(u_char *userdata, const struct pcap_pkthdr * /* header */,const u_char *m_packet) /*hash tables pass*/
{
	/*TWIP*/
	return 0;
}
int 
process_ip6(u_char *userdata, const struct pcap_pkthdr * /* header */,const u_char *m_packet) /*hash tables pass*/ 
{
	/*WIP*/
	return 0;
}
int 
process_tcp(u_char *userdata, const struct pcap_pkthdr *header /* header */,const u_char *m_packet) /*hash tables pass*/
{	/*WIP*/
	packet_args *args = (packet_args *)userdata;
	struct tcphdr *tcp = (struct tcphdr *)m_packet;

	curtime = header->ts;
	Packet *packet = g_slice_new(Packet);
	switch(args->sa_family)
	{
	case AF_INET:
		Packet_init_in_addr(packet, args->ip_src, ntohs(tcp->th_sport), args->ip_dst, ntohs(tcp->th_dport), header->len, header->ts);
		break;

	case AF_INET6:
		Packet_init_in6_addr(packet, args->ip6_src, ntohs(tcp->th_sport), args->ip6_dst, ntohs(tcp->th_dport), header->len, header->ts);
		break;
	}

	Connection *connection = find_connection(packet);
	if (connection != NULL)
		add_packet_to_connection(connection, packet);
	else
	{
		Connection *connection = g_slice_new(Connection);
		Connection_init(connection, packet);
		//Add this connection to a connectionlist depending on the process it belongs to
		//write this 
		get_process(connection, args->device,/*hash tables*/hash_table,inode_table);

	}
	g_slice_free(Packet, packet);
	return 1;//just to tell that work's over
}

void 
add_callback(packet_handle *handle ,enum packet_type type ,packet_callback callback)
{
	handle->callback[type] = callback ;
}

packet_handle*
get_interface_handle(char *device, GError **err)
{
	pcap_t *temp = pcap_open_live(device, BUFSIZ, promisc, 100, errbuf);
	if(temp == NULL)
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
	gboolean init_ele = true ; 
	while(count < buf.number){
		packet_handle *new_handle = get_interface_handle(devices[count], if_error);
		if ((interface_local_addr = get_device_local_addr(devices[count])) == NULL)
			printf("Failed to get addr for %s\n",devices[count]);
		if(new_handle != NULL)
		{	if(init_ele)
			{
				initial_handle = new_handle;
				init_ele = false;
			}
			add_callback(new_handle, packet_ip, process_ip);
			add_callback(new_handle, packet_ip6, process_ip6);
			add_callback(new_handle, packet_tcp, process_tcp);
			if(previous_handle != NULL)
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
		printf("device name : %s linktype: %d \n ",temp_handle->device_name, temp_handle->linktype);
		temp_handle = temp_handle->next;
	}	
}

void
print_interface_local_address()
{
	local_addr *temp = interface_local_addr;
	while(temp != NULL)
	{
		printf("%s : %s \n",temp->device_name,temp->ip_text);
		temp = temp->next;
	}
}

gboolean 
local_addr_contains (const in_addr_t &n_addr, local_addr *laddr = interface_local_addr) 
{
  if ((laddr->sa_family == AF_INET) && (n_addr == laddr->addr))
    return true;
  if (laddr->next == NULL)
    return false;
  return local_addr_contains(n_addr, laddr->next);
}

gboolean
local_addr6_contains (const struct in6_addr &n_addr, local_addr *local_addr = interface_local_addr)
{
	if ((laddr->sa_family == AF_INET6) && ((laddr->addr6).s6_addr == n_addr.s6_addr))
		return true;
	if (laddr->next == NULL)
		return false;
	return local_addr_contains(n_addr, laddr->next);
}

local_addr *
get_if_local_addr()
{
	return interface_local_addr;
}

void
packet_parse_tcp(packet_handle *handle, const struct pcap_pkthdr *hdr, const u_char * pkt)
{
	if (handle->callback[packet_tcp] != NULL)
	{
		if(handle->callback[packet_tcp](handle->userdata, hdr, pkt))
			return;
	}
}

void
packet_parse_ip(packet_handle *handle, const struct pcap_pkthdr *hdr, const u_char *pkt)
{
	const struct ip *ip_packet = (struct ip *)pkt;
	//check
	//u_char *payload = (u_char *)packet + sizeof(struct ip);
	/*if (handle->callback[packet_ip] != NULL)
	{	//write this
		if(handle->callback[packet_ip](handle->userdata, hdr, pkt))
			return ;
	}*/
	switch(ip_packet->ip_p)
	{
		case IPPROTO_TCP:
			packet_parse_tcp(handle, hdr, pkt);
			break;
		//non tcp IP packet support not present currently
		default:
			break;
	}
}
void
packet_pcap_callback(u_char *u_handle, const struct pcap_pkthdr *hdr, const u_char *pkt)
{
	packet_handle *handle = (packet_handle *)u_handle;
	switch(handle->linktype)
	{
	case (DLT_NULL):
		packet_parse_ip(handle, hdr, pkt);
		break;
	/*
	case (DLT_EN10MB):
		packet_parse_ethernet(handle, hdr, pkt);
		break;
	*/
	default :
		printf("Unknown linktype\n");
	}
}


int
packet_dispatch(packet_handle *handle, int count, u_char *user, int size)
{
	handle->userdata = user;
	handle->userdata_size = size;
	return pcap_dispatch(handle->pcap_handle, -1, packet_pcap_callback, (u_char *)handle);
}

local_addr*
get_global_local_addr()
{
	return interface_local_addr;
}