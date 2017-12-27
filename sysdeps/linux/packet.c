#include "packet.h"
#include <glib.h>
packet, args->ip_src, ntohs(tcp->th_sport), args->ip_dst, ntohs(tcp->th_dport), header->len, header->ts
void
Packet_init_in_addr(Packet *pkt,in_addr pkt_sip, unsigned short pkt_sport, in_addr pkt_dip, 
						unsigned short pkt_dport, u_int32_t pkt_len, timeval pkt_ts ,direction pkt_dir)
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
						unsigned short pkt_dport, u_int32_t pkt_len, timeval pkt_ts ,direction pkt_dir) 
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
	//this further requires a function to craete a global linked list of ip of the interfaces , create this in a new file
	g_assert(get_if_local_addr() !=  NULL);
	switch(pkt->dir)
	{
	case dir_outgoing:
		return true;
	case dir_incoming:
		return false;
	case dir_unknown:
		gboolean is_local;
		if (pkt->sa_family == AF_INET)
			is_local = local_addr_contains(pkt->sip.s_addr);
		else
			is_local = local_addr6_contains(pkt->sip6);
		
		if (is_local)
		{
			pk->dir = outgoing;
		}
		else
		{
			if (pkt->sa_family == AF_INET)
				is_local = local_addr_contains(pkt->dip.s_addr);
			else
				is_local = local_addr6_contains(pkt->dip6);

			if(!is_local)
			{
				printf("neither dip nor sip are local\n");
				return false;
			}
			pkt->dir = dir_incoming;
			return false;
		}
	}
	return false;
}

gboolean
packet_match_source(Packet *pkt, Packet *other)
{
	return (pkt->sport == other->sport) &&
			(pk->sa_family == AF_INET) ? (pk->sip.s_addr == other->sip.s_addr) :
										(pk->sip6.s6_addr == other->sip6.s6_addr);

}

gboolean
packet_match(Packet *pkt, Packet *other) 
{
	return (pkt->sa_family == other->sa_family) && 
			(pk->sport == other->sport) &&
			(pk->dport == other->dport) &&
			(pk->sa_family == AF_INET) ? ((pk->sip.s_addr == other->sip.s_addr) && (pk->dip.s_addr == other->dip.s_addr)):
										((pk->sip6.s6_addr == other->sip6.s6_addr) && (pk->dip6.s6_addr == other->dip6.s6_addr));
}

gboolean  )
direction 
invert(direction dir)
{
	if (dir == dir_incoming)
		return dir_outgoing;
	else if (dir_outgoing)
		return dir_incoming;
	else
		return dir_unknown;
}

Packet *
get_inverted_packet(Packet *pkt)
{
	direction inverted_dir = invert(pkt->dir);
	Packet *inverted_packet = g_slice_new (Packet);
	if (pkt->sa_family == AF_INET)
		Packet_init_in_addr(inverted_packet, pkt->sip, pkt->sport, pkt->dip, pkt->dport, 
							pkt->len, pkt->ts , inverted_dir);
	else
		Packet_init_in6_addr(inverted_packet, pkt->sip6, pkt->sport, pkt->dip6, pkt->dport,
							pkt->len, pkt->ts, inverted_dir);
	return inverted_packet; 
		
}


