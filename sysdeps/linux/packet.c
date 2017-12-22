#include "packet.h"
#include <glib.h>
packet, args->ip_src, ntohs(tcp->th_sport), args->ip_dst, ntohs(tcp->th_dport), header->len, header->ts
void
Packet_init_in_addr(Packet *pkt,in_addr pkt_sip, unsigned short pkt_sport, in_addr pkt_dip, 
						unsigned short pkt_dport, u_int32_t pkt_len, timeval pkt_ts ,direction pkt_dir = dir_unknown)
{
	pkt->sa_family = AF_INET;
	pkt->sip = pkt_sip;
	pkt->dip = pkt_dip;
	pkt->sport = pkt_sport;
	pkt->dport = pkt_dport;
	pkt->len = pkt_len;
	pkt->time = pkt_ts;
	pkt->dir = pkt_dir;
}

void
Packet_init_in6_addr(Packet *pkt,in6_addr pkt_sip6, unsigned short pkt_sport, in6_addr pkt_dip6, 
						unsigned short pkt_dport, u_int32_t pkt_len, timeval pkt_ts ,direction pkt_dir = dir_unknown)
{
	pkt->sa_family = AF_INET6;
	pkt->sip6 = pkt_sip6;
	pkt->dip6 = pkt_dip6;
	pkt->sport = pkt_sport;
	pkt->dport = pkt_dport;
	pkt->len = pkt_len;
	pkt->time = pkt_ts;
	pkt->dir = pkt_dir;
}
void
Packet_init(Packet *pkt, Packet &old_packet)
{
	pkt->sa_family = old_packet.sa_family;
	pkt->sip = old_packet.sip;
	pkt->dip = old_packet.dip;
	pkt->sip6 = old_packet.sip6;
	pkt->dip6 = old_packet.dip6;
	pkt->sport = old_packet.sport;
	pkt->dport = old_packet.dport;
	pkt->len = old_packet.len;
	pkt->time = old_packet.time;
	pkt->dir = old_packet.dir;
	//hash for matching the process is not yet implemented
}

//check the dir of the packet
gboolean 
is_pkt_outgoing(Packet *pkt)
{
	/*
	*1.check if outgoing
	*IF UNKNOWN---->
	*2.check if the sip is one of the interface ip (check for ipv4 then sip otherwise sip6)-->then dir is outgoing 
	*3.if dip  is one of the interfaces ip then it's incoming
	*4. if neither dip nor sip are ip of interfaces then return false
	*in either cases after setting the dir for unknown RETURN False in case of incoming
	**/
	//NOTE this req function to check all the ip of the interfaces
	//this further requires a function to create a linked list of ip of the interfaces , create this in a new file
}