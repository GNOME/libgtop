#ifndef __PACKET_H_
#define __PACKET_H_

#include <glib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>

G_BEGIN_DECLS

#define HASHKEYSIZE 92
enum direction
{
	dir_unknown,
	dir_incoming,
	dir_outgoing
};

typedef struct _Packet Packet;
struct _Packet
{
	direction dir;
	int sa_family;
	in_addr sip;
	in_addr dip;
	in6_addr sip6;
	in6_addr dip6;
	unsigned short sport;
	unsigned short dport;
	u_int32_t len;
	timeval time;
	char *pkt_hash;
};

void Packet_init_in_addr(Packet *pkt,in_addr pkt_sip, unsigned short pkt_sport, in_addr pkt_dip, 
						unsigned short pkt_dport, u_int32_t pkt_len, timeval pkt_ts, direction pkt_dir = dir_unknown);
void Packet_init_in6_addr(Packet *pkt,in6_addr pkt_sip6, unsigned short pkt_sport, in6_addr pkt_dip6, 
						unsigned short pkt_dport, u_int32_t pkt_len, timeval pkt_ts, direction pkt_dir = dir_unknown);

gboolean is_pkt_outgoing(Packet *pkt);
gboolean packet_match_source(Packet *pkt, Packet *other);
gboolean packet_match(Packet *pkt,Packet *other); 
Packet *get_inverted_packet(Packet *pkt);
char *Packet_gethash(Packet *pkt);
void Packet_set_global_local_addr();

G_END_DECLS

#endif