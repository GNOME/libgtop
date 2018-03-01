#ifndef __GLIBTOP_DEV_HANDLES_H__
#define __GLIBTOP_DEV_HANDLES_H__

#include <glib.h>
#include <pcap.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <glibtop/interface_local_addr.h>
#include <glibtop/net_process.h>
#include <glibtop/connection.h>
#include <glibtop/netsockets.h>

G_BEGIN_DECLS

#define IF_HANDLE 1
#define IF_HANDLE_FUNC 19
#define refresh_delay 1

typedef struct _packet_handle packet_handle;
//for callbacks
typedef int (*packet_callback)(u_char *userdata, const struct pcap_pkthdr *header,const u_char *m_packet);
enum packet_type
{
	packet_ip,
	packet_ip6,
	packet_tcp,
	packet_udp,
	no_of_packet_types
};

struct _packet_handle
{	char *device_name;
	pcap_t *pcap_handle;
	int linktype;
	packet_callback callback[no_of_packet_types];
	u_char *userdata;
	int userdata_size;
	packet_handle *next;
};

typedef struct _packet_args packet_args;
struct _packet_args
{
	const char* device;
	int sa_family;
	struct in_addr ip_src;
	struct in_addr ip_dst;
	struct in6_addr ip6_src;
	struct in6_addr ip6_dst;
};
/*Defining the headers as those available in netinet/*.h are having issues when parsing for ports*/
#define SNAP_LEN 1518
#define SIZE_ETHERNET 14
#define ETHER_ADDR_LEN	6
/* Ethernet header */
struct hdr_ethernet {
	u_char  ether_dhost[ETHER_ADDR_LEN];
	u_char  ether_shost[ETHER_ADDR_LEN];
	u_short ether_type;
};

/* IP header */
struct hdr_ip {
	u_char  ip_vhl;
	u_char  ip_tos;
	u_short ip_len;
	u_short ip_id;
	u_short ip_off;
	#define IP_RF 0x8000
	#define IP_DF 0x4000
	#define IP_MF 0x2000
	#define IP_OFFMASK 0x1fff
	u_char  ip_ttl;
	u_char  ip_p;
	u_short ip_sum;
	struct  in_addr ip_src,ip_dst;
};

#define IP_HL(ip)	(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)	(((ip)->ip_vhl) >> 4)

/* TCP header */
typedef guint tcp_seq;

struct hdr_tcp {
	u_short th_sport;
	u_short th_dport;
	tcp_seq th_seq;
	tcp_seq th_ack;
	u_char  th_offx2;
#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
	u_char  th_flags;
	#define TH_FIN  0x01
	#define TH_SYN  0x02
	#define TH_RST  0x04
	#define TH_PUSH 0x08
	#define TH_ACK  0x10
	#define TH_URG  0x20
	#define TH_ECE  0x40
	#define TH_CWR  0x80
	#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
	u_short th_win;
	u_short th_sum;
	u_short th_urp;
};

packet_handle * open_pcap_handles();
void print_pcap_handles(packet_handle *handle);
void print_interface_local_address();
local_addr *get_if_local_addr();
int packet_dispatch(packet_handle *handle, int count, u_char *user, int size);
struct timeval get_curtime(struct timeval val );
local_addr* get_global_local_addr();
void handles_set_hash(GHashTable *inode, GHashTable *hash);
void handles_set_process_lists(Net_process_list *procs, Net_process *tcp);
void process_init();
Net_process_list *get_processes();
packet_handle *get_global_packet_handles(packet_handle *val);
G_END_DECLS

#endif
