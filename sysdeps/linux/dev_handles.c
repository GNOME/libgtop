#include <glibtop/netlist.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <glibtop/dev_handles.h>
#include <glibtop/packet.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <sys/time.h>
#include <glibtop/interface_local_addr.h>
#include <glibtop/connection.h>
#include <glibtop/net_process.h>
#include <glibtop/netsockets.h>
#include <glib.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <glibtop/procstate.h>

int size_ip;
int size_tcp;
int promisc = 0;
char errbuf[PCAP_ERRBUF_SIZE];

void
process_init()
{
	GSList *processes = get_proc_list_instance(NULL);//global process list
	Net_process *unknownTCP = get_unknown_proc_instance(NULL);

	Net_process_init(unknownTCP, 0);
	processes = Net_process_list_init(processes, unknownTCP);
	get_proc_list_instance(processes);
}

struct timeval 
get_curtime(struct timeval val)
{	static struct timeval curtime ;
	if(val.tv_sec)
		curtime = val;
	return curtime;
}

Net_process *get_process_from_inode(unsigned long inode, const char *device_name)
{
	int pid = match_pid(inode);
	GSList *current = get_proc_list_instance(NULL) ;/*global list of all procs*/
	while (current != NULL)
	{
		Net_process *curr_process = (Net_process *)current->data;
		g_assert(curr_process);
		if (pid == curr_process->pid)
			return curr_process;
		current = current->next;
	}
	if (pid!= -1)
	{	
		Net_process *proc = g_slice_new(Net_process);
		Net_process_init(proc, pid);
		GSList *processes = get_proc_list_instance(NULL);
		processes = Net_process_list_init(processes, proc);
		get_proc_list_instance(processes);	//processes = temp
		return proc;
	}
	return NULL;
}

Net_process * 
get_process(Connection *conn, const char *device_name)
{
	unsigned long inode = match_hash_to_inode(Packet_gethash(conn->ref_packet));
	if (inode == -1)
	{	
		char *fname = g_strdup("/proc/self/net/tcp");
		global_hashes test_hash = get_global_hashes_instance();
		test_hash.inode_table = NULL;
		test_hash.hash_table = NULL;
		test_hash = get_global_hashes_instance();
		glibtop_socket *socket_list = glibtop_get_netsockets (fname, test_hash.inode_table, test_hash.hash_table);
		g_free(fname);
		if ((inode = match_hash_to_inode(Packet_gethash(conn->ref_packet))) == -1)
		{
			Packet *reverse_pkt = get_inverted_packet(conn->ref_packet);
			inode = match_hash_to_inode(Packet_gethash(reverse_pkt));
			if (inode == -1)
			{
				//printf("Unknown PROC\n");
				g_slice_free(Packet, reverse_pkt);
				GSList *temp = Conn_list_init(get_unknown_proc_instance(NULL)->proc_connections, conn);
				get_unknown_proc_instance(NULL)->proc_connections = temp; //assigning this connection to unknown TCP 
				return get_unknown_proc_instance(NULL);
			}
			g_slice_free(Packet, conn->ref_packet);
			conn->ref_packet = reverse_pkt;
		}
	}
	/*this proc is present in the hash table*/
	Net_process *proc = get_process_from_inode(inode, device_name);
	if (proc == NULL)
	{
		proc = g_slice_new(Net_process);
		printf("%s not in /proc/net/tcp \n", Packet_gethash(conn->ref_packet));
		Net_process_init(proc, inode);
		//Net_process_list *temp = g_slice_new(Net_process_list);
		GSList *processes = get_proc_list_instance(NULL);
		processes = Net_process_list_init(processes, proc);
		get_proc_list_instance(processes);	//processes = temp
	}
	proc->proc_connections = Conn_list_init(proc->proc_connections, conn);
	return proc;
}

int 
process_ip(u_char *userdata, const struct pcap_pkthdr *header,const u_char *m_packet)
{
	packet_args *args = (packet_args *)userdata;
	struct ip *ip4 = (struct ip *)m_packet;
	args->sa_family = AF_INET;
	args->ip_src = ip4->ip_src;
	args->ip_dst = ip4->ip_dst;
	return 0;
}

int 
process_ip6(u_char *userdata, const struct pcap_pkthdr *header,const u_char *m_packet)
{
	packet_args *args = (packet_args *)userdata;
	const struct ip6_hdr *ip6 = (struct ip6_hdr *)m_packet;
	args->sa_family = AF_INET6;
	args->ip6_src = ip6->ip6_src;
	args->ip6_dst = ip6->ip6_dst;
	return 0;
}
int 
process_tcp(u_char *userdata, const struct pcap_pkthdr *header,const u_char *m_packet)
{
	packet_args *args = (packet_args *)userdata;
	struct hdr_tcp *tcp = (struct hdr_tcp *)(m_packet);
	struct timeval cur = header->ts;
	get_curtime(header->ts);
	Packet *packet = g_slice_new(Packet);
	
	switch(args->sa_family)
	{
	case AF_INET:
		Packet_init_in_addr(packet, args->ip_src, ntohs(tcp->th_sport), args->ip_dst, ntohs(tcp->th_dport), header->len, header->ts, dir_unknown);
		break;

	case AF_INET6:
		Packet_init_in6_addr(packet, args->ip6_src, ntohs(tcp->th_sport), args->ip6_dst, ntohs(tcp->th_dport), header->len, header->ts, dir_unknown);
		break;

	default:
		printf("invalid family\n");
		packet = NULL;
	}

	if (packet != NULL)
	{
		Connection *connection = find_connection(packet);
		if (connection != NULL)
			add_packet_to_connection(connection, packet);
		else
		{	printf("NEW PROC \n");
			Connection *connection = g_slice_new(Connection);
			Connection_init(connection, packet);
			//Add this connection to a connectionlist depending on the process it belongs to
			get_process(connection, args->device);
		}
	}
	return 1;
}

void 
add_callback(packet_handle *handle, enum packet_type type, packet_callback callback)
{
	handle->callback[type] = callback ;
}

packet_handle*
get_interface_handle(char *device, GError **err)
{
	bpf_u_int32 maskp; // subnet mask
	bpf_u_int32 netp; // interface IP
	char filter_exp[] = "ip";
	struct bpf_program fp;
	pcap_t *temp = pcap_open_live(device, BUFSIZ, promisc, 100, errbuf);
	pcap_lookupnet(device, &netp, &maskp, errbuf);
	if(temp == NULL)
	{
		g_set_error(err,
					IF_HANDLE,
					IF_HANDLE_FUNC,
					"failed to open handle for device : %s",
					device);
		return NULL;
	}
	if (pcap_compile(temp, &fp, filter_exp, 0, netp) == -1) 
	{
		fprintf(stderr, "Couldn't parse filter %s: %s\n",
				filter_exp, pcap_geterr(temp));
		exit(EXIT_FAILURE);
	}
	if (pcap_setfilter(temp, &fp) == -1)
	{
		fprintf(stderr, "Couldn't install filter %s: %s\n",
				filter_exp, pcap_geterr(temp));
		exit(EXIT_FAILURE);
	}

	packet_handle *temp_packet_handle = (packet_handle *)malloc(sizeof(packet_handle));
	if (temp_packet_handle != NULL)
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
{	
	glibtop_netlist buf;
	char **devices;
	devices = glibtop_get_netlist (&buf);
	GError **if_error;
	int count=0;
	packet_handle *previous_handle=NULL , *initial_handle = NULL;
	gboolean init_ele = TRUE ; 
	while(count < buf.number)
	{
		packet_handle *new_handle = get_interface_handle(devices[count], if_error);
		local_addr *temp;
		if ((temp = get_device_local_addr(devices[count])) == NULL)
			printf("Failed to get addr for %s\n",devices[count]);
		if (new_handle != NULL)
		{	
			if (init_ele)
			{
				initial_handle = new_handle;
				init_ele = FALSE;
			}
			add_callback(new_handle, packet_ip, process_ip);
			add_callback(new_handle, packet_ip6, process_ip6);
			add_callback(new_handle, packet_tcp, process_tcp);
			if (pcap_setnonblock(new_handle->pcap_handle, 1, errbuf) == -1)
				printf("failed to set to non blocking mode %s\n",devices[count]);
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
	int count = 0;
	packet_handle *temp_handle = handle;
	while (temp_handle != NULL)
	{
		printf("device name : %s linktype: %d \n ", temp_handle->device_name, temp_handle->linktype);
		temp_handle = temp_handle->next;
	}	
}

void
print_interface_local_address()
{
	local_addr *temp = get_local_addr_instance(NULL);
	while (temp != NULL && temp->device_name != NULL)
	{
		printf("%s : %s \n", temp->device_name, temp->ip_text);
		temp = temp->next;
	}
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
	const struct hdr_ip *ip_packet = (struct hdr_ip*)pkt;
	printf("Looking at packet with length %d \n", hdr->len);
	size_ip = IP_HL(ip_packet)*4;
		if (size_ip < 20) {
		printf("   * Invalid IP header length: %u bytes\n", size_ip);
		return;
	}
	u_char *payload = (u_char *)(pkt + sizeof(struct ip));
	if (handle->callback[packet_ip] != NULL)
	{	
		if (handle->callback[packet_ip](handle->userdata, hdr, pkt))
			return ;
	}
	const struct hdr_tcp *tcp;
	switch(ip_packet->ip_p)
	{
	case IPPROTO_TCP:
		printf("exec tcp\n");
		packet_parse_tcp(handle, hdr, payload);
		break;
	//non tcp IP packet support not present currently
	default:
		break;
	}
}

void packet_parse_ip6(packet_handle *handle, const struct pcap_pkthdr *hdr, const u_char *pkt)
{
	const struct ip6_hdr *ip6 = (struct ip6_hdr *)pkt;
	u_char *payload = (u_char *)(pkt + sizeof(ip6));

	if (handle->callback[packet_ip6] != NULL) 
	{
		if (handle->callback[packet_ip6])(handle->userdata, hdr, pkt);
			return;
	}
	switch ((ip6->ip6_ctlun).ip6_un1.ip6_un1_nxt) 
	{
	case IPPROTO_TCP:
		packet_parse_tcp(handle, hdr, payload);
		break;
	default:
		break;
	}
}

void
packet_parse_ethernet(packet_handle * handle, const struct pcap_pkthdr *hdr, const u_char *pkt)
{
	const struct hdr_ethernet *ethernet = (struct hdr_ethernet *)pkt;
	u_char *payload = (u_char *)(pkt +14);	
	switch (ntohs(ethernet->ether_type)) 
	{
	case ETHERTYPE_IP:
 		packet_parse_ip(handle, hdr, payload);
		break;

	case ETHERTYPE_IPV6:
		packet_parse_ip6(handle, hdr, payload);
		break;
	
	}
}

void
packet_pcap_callback(u_char *u_handle, const struct pcap_pkthdr *hdr, const u_char *pkt)
{	
	packet_handle *handle = (packet_handle *)u_handle;
	switch(handle->linktype)
	{
	case (DLT_EN10MB):
		packet_parse_ethernet(handle, hdr, pkt);
		break;
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

